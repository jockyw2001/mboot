#!/usr/bin/python
"""
./app_append.py BINDIR, APP_ELF_PATH, APP_BIN_PATH, APP_ZIP_PATH, POSTFIX, has_trunk_head/no_trunk_head bfn/bfe/tvbfr app_head_size
"""

import sys
import os
import re
from os.path import join
from subprocess import Popen, PIPE
import hashlib
import struct
import array
from struct import pack, unpack

PREFIX            = '  [APP_APPEND]'
CONFIG_PATH       = '.config'
SBOOT_ELF         = 'sboot.elf'
FLASH             = 'flash.bin'
FLASH_OUT         = 'flash_with_%s.bin'
LOADER_OUT        = 'bfn_loader.bin'
MIU_OUT           = 'bfn_miu.bin'
APP_OUT           = 'bfn_app.bin'
TRUNKMBOOT_SOURCE = 'mboot.bin'
BFE_BOOT_OUT      = 'bfe_boot.bin'
BFE_APP_OUT       = 'bfe_app.bin'
BFE_USB_LOADER_OUT= 'bfe_usb_upgrade_loader.bin'
TEE_LOADER_OUT    = 'tee_loader.dat'
REE_LOADER_OUT    = 'ree_loader.bin'
ARM_SBOOT_BASE    = 0x20000000


def get_nmap(NM, path):
    out = Popen([NM, path], stdout=PIPE).communicate()[0]
    return dict([(m[2], (m[0], m[1]))
        for m in re.findall('([0-9a-zA-Z]{8})\s*(.)\s*(.+)', out)])


def get_app_info(READELF, path, postfix):
    out = Popen([READELF, '-h', path], stdout=PIPE).communicate()[0]
    entry = int(re.search(r'Entry point address:\s*(0x\w+)', out).group(1), 0)

    out = Popen([READELF, '-S', path], stdout=PIPE).communicate()[0]

    if postfix == 'ecos':
        regex = '(\[\s*\d+\])\s+\.rom_vector(\S+)\s+(\S+)\s+(\S+)'
    else: #uboot
        regex = '(\[\s*\d+\])\s+(\S+)\s+(\S+)\s+(\S+)'

    for m in re.finditer(regex, out):
        addr = int(m.group(4), 16)
        if addr != 0:
            break
    return entry, addr

def check_hash2_addr(path, size, app_ram_start):
    out = open(path, 'rb').read()
    hash2_addr = int(re.search(r'CONFIG_HASH2_START_ADDRESS=\s*(0x\w+)', out).group(1), 0)

    if hash2_addr <  (size+app_ram_start):
        print '  [ERROR] HASH2_START_ADDRESS=0x%08x which should bigger than 0x%08x' % (hash2_addr, size+app_ram_start)
        exit(1)
    return 0

def gen_bin(argv, flash, header, app_zip, dst, entry, zip_size, size, HEADER_SIZE, app_bin_path):
    if argv[7] == 'bfe':
        #for generate bfe_usb_upgrade_loader.bin
        loader_bin = flash[_ld_LDR_load_start - _ld_load_start:_ld_LDR_load_end - _ld_load_start]
        if os.path.exists(join(bindir,BFE_USB_LOADER_OUT)):
            os.remove(join(bindir,BFE_USB_LOADER_OUT))
        loader_bin = loader_bin + '\0' * (0x20000-len(loader_bin)) #128K bytes alignment
        open(join(bindir,BFE_USB_LOADER_OUT), 'wb').write(loader_bin + open(app_bin_path, 'rb').read())

        #for generate bfe_boot.bin
        bfe_boot_bin = flash[_ld_HASH0_run_start - _ld_load_start:_ld_LDR_load_end - _ld_load_start]
        if os.path.exists(join(bindir, BFE_BOOT_OUT)):
            os.remove(join(bindir, BFE_BOOT_OUT))
        open(join(bindir, BFE_BOOT_OUT), 'wb').write(bfe_boot_bin)
        sboot_nmap = get_nmap(os.environ['NM'], join(bindir, SBOOT_ELF))
        if '_ld_TEE_LDR_load_start' in sboot_nmap:
            #append REE loader signature to bfe_boot.bin
            bfe_boot_bin = open(join(bindir, BFE_BOOT_OUT), 'rb').read()
            ree_bin_sig = '\0'*272
            bfe_boot_bin = bfe_boot_bin + ree_bin_sig
            open(join(bindir, BFE_BOOT_OUT), 'wb').write(bfe_boot_bin)

        #for generate bfe_app.bin
        bfe_app_bin = header + app_zip
        if os.path.exists(join(bindir, BFE_APP_OUT)):
            os.remove(join(bindir, BFE_APP_OUT))
        open(join(bindir, BFE_APP_OUT), 'wb').write(bfe_app_bin)

        print PREFIX, 'Generate bin/bfe_boot.bin, bin/bfe_app.bin, bin/bfe_usb_upgrade_loader.bin'

    elif argv[7] == 'bfn':
        #for generate bfn_miu.bin
        miu_bin = flash[_ld_HASH1_run_start - _ld_load_start:_ld_HASH1_run_end - _ld_load_start]

        if argv[12] == '5':
            miu_header = '\0' * 0x100
            miu_bin = miu_header + miu_bin +  '\0' * ((4-len(miu_bin)%4)%4) + 'PARE'  #4 byte alignment + ENDTAG "PARE"
            miu_header = pack('<4cII','P','A','R','M', 0 , len(miu_bin)) + miu_header[12:] #0x100 header: TAG"PARM" + reserved(4) + size(3) + reserved(left)
            miu_bin = miu_header + miu_bin[0X100:]

        if os.path.exists(join(bindir, MIU_OUT)):
            os.remove(join(bindir, MIU_OUT))
        if len(argv) > 8 and argv[8] == 'bfn_miu_with_hash0':
            print PREFIX,'add hash0 to bfn_miu.bin'
            rsa_bin = flash[0:0x300] #hash0 size 0x300
            open(join(bindir, MIU_OUT), 'wb').write(miu_bin + rsa_bin)
        else:
            open(join(bindir, MIU_OUT), 'wb').write(miu_bin)

        #add BIN0 and BINE tag to binary
        header = pack('<IIIII',0x304E4942, dst, entry, zip_size+4, size)
        header += '\0' * (HEADER_SIZE - len(header))
        app_zip += 'BINE'

        flash_app = (header + app_zip)
        if os.path.exists(join(bindir, APP_OUT)):
            os.remove(join(bindir,APP_OUT))
        open(join(bindir, APP_OUT), 'wb').write(flash_app)

        # regenerate the APP digest for BFN/BFE
        h = hashlib.sha256(header + app_zip)
        l = len(h.digest())
        print PREFIX, 'Regenerate APP Digest:', h.hexdigest()
        flash = flash[:AppDigest] + h.digest()[::-1] + flash[AppDigest+l:]
        flash = flash[:len(flash) - len(header) - len(app_zip) + 4] # extra 4 bytes for LDRE
        open(join(bindir, FLASH), 'wb').write(flash)

        #for generate bfn_loader.bin
        loader_bin = flash[_ld_LDR_load_start - _ld_load_start:_ld_LDR_load_end - _ld_load_start]
        if os.path.exists(join(bindir,LOADER_OUT)):
            os.remove(join(bindir,LOADER_OUT))
        open(join(bindir,LOADER_OUT), 'wb').write(loader_bin)

        sboot_nmap = get_nmap(os.environ['NM'], join(bindir, SBOOT_ELF))
        if '_ld_TEE_LDR_load_start' in sboot_nmap:
            #append REE loader signature to bfn_loader.bin
            bfn_loader_bin = open(join(bindir, LOADER_OUT), 'rb').read()
            ree_bin_sig = '\0'*272
            bfn_loader_bin = bfn_loader_bin + ree_bin_sig
            open(join(bindir, LOADER_OUT), 'wb').write(bfn_loader_bin)

        print PREFIX, 'Generate bin/bfn_miu.bin, bin/bfn_loader.bin, bin/bfn_app.bin'

        #for regenerate flash_with_xxx.bin
        flash = flash[:_ld_APP_load_start - _ld_load_start] + header + app_zip #append new header&app for bfn case
        flash = flash[:_ld_LDR_load_start - _ld_load_start] + 'LDR0' + flash[_ld_LDR_load_start - _ld_load_start+4:] #tag of hash2 for bfn case
        flash = flash[:_ld_HASH1_run_start - _ld_load_start] + miu_bin + flash[_ld_HASH1_run_end - _ld_load_start:] #update new

        out_path = join(bindir, FLASH_OUT % (postfix))
        if os.path.exists(out_path):
            os.remove(out_path)
        open(out_path, 'wb').write(flash)
        print PREFIX, 'Regenerate ' + out_path
    else:
        return #boot from other

    

def main(argv):

    global _ld_HASH0_run_start, _ld_HASH1_run_start, _ld_HASH1_run_end , _ld_LDR_load_start, _ld_LDR_load_end
    global _ld_load_start, AppDigest, bindir, _ld_APP_load_start, postfix

    bindir, app_elf_path, app_bin_path, app_zip_path, postfix = argv[1:6]
    enable_tee_ldr = argv[10]
    HEADER_SIZE = int(argv[11], 16)

    if len(argv) > 6:
        if argv[6] == 'has_trunk_head':
            boot_from = argv[7]
            app_bin = 0x0
            sboot_nmap = get_nmap(os.environ['NM'], join(bindir, SBOOT_ELF))
            if 'EmbeddedTeeRsaPubMStar' in sboot_nmap:
                app_bin = open(join(bindir,TRUNKMBOOT_SOURCE), 'rb').read()[0x40000:]
            elif '_ld_TEE_LDR_load_start' in sboot_nmap:
                app_bin = open(join(bindir,TRUNKMBOOT_SOURCE), 'rb').read()[0x40000:]
            else:
                app_bin = open(join(bindir,TRUNKMBOOT_SOURCE), 'rb').read()[0x20000:]

            if boot_from == 'bfn':
                app_bin_start   = 0x508
                app_base_offset = 0x504
            elif boot_from == 'bfe':
                app_bin_start   = 0x400
                app_base_offset = 0x400
            else:
                return # boot from other

            app_base = app_bin[app_bin_start:app_bin_start+0x4]
            app_base, = struct.unpack("I",app_base)
            app_base = app_base - app_base_offset

            app_entry_off = app_bin[app_bin_start+0x4:app_bin_start+0x8]
            app_entry_off, = struct.unpack("I",app_entry_off)
            app_entry_off  = app_entry_off + app_base_offset

            app_size = len(app_bin)
            #app_size = (app_size + 31) / 32  * 32 # jh.huang review

            chunk_header = None
            if boot_from == 'bfn':
                app_bin += 'BINE'
                chunk_header = pack('<IIII', 0x304E4942, app_base, app_entry_off, app_size+4)
            elif boot_from =='bfe':
                chunk_header = pack('<IIII', app_base, app_entry_off, app_size+4, 0xFFFFFFFF)
            else:
                return # boot from other

            app_bin = chunk_header + app_bin[16:]
            if boot_from == 'bfn':
                print 'Generate bfn_app.bin(with trunk header)'
                open(join(bindir,APP_OUT), 'wb').write(app_bin)
            elif boot_from =='bfe':
                print 'Generate bfe_app.bin(with trunk header)'
                open(join(bindir,BFE_APP_OUT), 'wb').write(app_bin)
            else:
                return # boot from other
            return 0

    READELF = os.environ['READELF']
    NM = os.environ['NM']

    sboot_nmap = get_nmap(NM, join(bindir, SBOOT_ELF))
    _ld_load_start = int('0x' + sboot_nmap['_ld_load_start'][0], 16)

    if argv[7] != "tvbfr":
        _ld_APP_load_start = int('0x' + sboot_nmap['_ld_APP_load_start'][0], 16)

        _ld_LDR_load_start = int('0x' + sboot_nmap['_ld_LDR_load_start'][0], 16)
        _ld_LDR_load_end = int('0x' + sboot_nmap['_ld_LDR_load_end'][0], 16)

        _ld_LDR_run_base = int('0x' + sboot_nmap['_ld_LDR_run_base'][0], 16)
        _ld_LDR_run_start = int('0x' + sboot_nmap['_ld_LDR_run_start'][0], 16)
        _ld_LDR_run_end = int('0x' + sboot_nmap['_ld_LDR_run_end'][0], 16)

        AppDigest = int('0x' + sboot_nmap['AppDigest'][0], 16)

        _ld_HASH1_run_start = int('0x' + sboot_nmap['_ld_HASH1_run_start'][0], 16)
        _ld_HASH1_run_end = int('0x' + sboot_nmap['_ld_HASH1_run_end'][0], 16)

        _ld_HASH0_run_start = int('0x' + sboot_nmap['_ld_HASH0_run_start'][0], 16)

    FLASH_BASE_ADDR = _ld_load_start#_ld_HASH0_run_start - 0x2000

    if enable_tee_ldr == 'y':
        _ld_REE_LDR_load_start = int('0x' + sboot_nmap['_ld_REE_LDR_load_start'][0], 16)
        _ld_REE_LDR_load_end = int('0x' + sboot_nmap['_ld_REE_LDR_load_end'][0], 16)
        _ld_REE_LDR_run_base = int('0x' + sboot_nmap['_ld_REE_LDR_run_base'][0], 16)
        _ld_TEE_LDR_load_start = int('0x' + sboot_nmap['_ld_TEE_LDR_load_start'][0], 16)
        _ld_TEE_LDR_load_end = int('0x' + sboot_nmap['_ld_TEE_LDR_load_end'][0], 16)

    app_entry, app_ram_start = get_app_info(READELF, app_elf_path, postfix)

    if len(argv) > 9:
        if argv[9] == 'arm' and postfix == 'ecos':
            # arm .rom_vector address is 0x01020000, but sboot base address is 0x20000000
            app_entry     = app_entry     | ARM_SBOOT_BASE # add sboot base address
            app_ram_start = app_ram_start | ARM_SBOOT_BASE # add sboot base address

    print PREFIX, 'Address:'

    if argv[7] != "tvbfr":
        print '    _ld_APP_load_start:  0x%08x' % _ld_APP_load_start
        print '    _ld_HASH0_run_start  0x%08x' % _ld_HASH0_run_start
        print '    _ld_HASH1_run_start: 0x%08x' % _ld_HASH1_run_start
        print '    _ld_HASH1_run_end:   0x%08x' % _ld_HASH1_run_end
        print '    _ld_LDR_load_start:  0x%08x' % _ld_LDR_load_start
        print '    _ld_LDR_load_end:    0x%08x' % _ld_LDR_load_end
        print '    HEADER_SIZE:         0x%08x' % HEADER_SIZE
    
    print '    app_ram_start:       0x%08x' % app_ram_start
    print '    app_entry:           0x%08x' % app_entry
    print '    FLASH_BASE_ADDR:     0x%08x' % FLASH_BASE_ADDR

    if argv[7] != "tvbfr":
        dst = app_ram_start - HEADER_SIZE
        size = os.stat(app_bin_path).st_size + HEADER_SIZE
        size = (size + 31) / 32  * 32
        zip_size = os.stat(app_zip_path).st_size + HEADER_SIZE
        entry = app_entry - dst
        app_zip = open(app_zip_path, 'rb').read()
        print PREFIX, 'dst=0x%08x entry=0x%08x' % (dst, entry)
        print PREFIX, 'size=0x%08x(%d) zip_size=0x%08x(%d)' % (
                size, size, zip_size, zip_size)
        if postfix == 'ecos':
            check_hash2_addr(CONFIG_PATH, size, app_ram_start)

        header = pack('<IIII', dst, entry, zip_size, size)
    else:
        size = os.stat(app_bin_path).st_size
        zip_size = os.stat(app_zip_path).st_size
        app_zip = open(app_zip_path, 'rb').read()
        header = pack('<IIIIIIII', FLASH_BASE_ADDR+0x10000+HEADER_SIZE, app_ram_start, app_ram_start+size, FLASH_BASE_ADDR+0x10000+HEADER_SIZE+zip_size, app_entry,0,0,0xB007)

    header += '\0' * (HEADER_SIZE - len(header))

    print PREFIX, 'header:', len(header)
    flash = open(join(bindir, FLASH), 'rb').read()
    if argv[7] != "tvbfr":
        # Replace the APP digest
        h = hashlib.sha256(header + app_zip)
        l = len(h.digest())
        print PREFIX, 'APP Digest:', h.hexdigest()
        if enable_tee_ldr == 'y':
            AppDigest = _ld_REE_LDR_load_start + (AppDigest - _ld_REE_LDR_run_base) - FLASH_BASE_ADDR
            flash = flash[:AppDigest] + h.digest()[::-1] + flash[AppDigest+l:]
        else:
            AppDigest = _ld_LDR_load_start + (AppDigest - _ld_LDR_run_base) - FLASH_BASE_ADDR
            flash = flash[:AppDigest] + h.digest()[::-1] + flash[AppDigest+l:]

# added for TEE keybank encryption
    if 'EmbeddedAESTeeDecryptionKey' in sboot_nmap:
        # TEE keybank encryption key
        Uniform_aes_start = int('0x' + sboot_nmap['EmbeddedAESTeeDecryptionKey'][0], 16)
        Uniform_aes = open(join('..', 'tee_loader', 'secure', 'AES_TEE_Uniform.bin'), 'rb').read()
        Uniform_aes_start_offset = _ld_LDR_load_start + (Uniform_aes_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:Uniform_aes_start_offset] + Uniform_aes + flash[Uniform_aes_start_offset+16:]
        # REE keybank encryption key
        Uniform_aes_start = int('0x' + sboot_nmap['EmbeddedAESReeDecryptionKey'][0], 16)
        Uniform_aes = open(join('secure', 'AES_REE_Uniform.bin'), 'rb').read()
        Uniform_aes_start_offset = _ld_LDR_load_start + (Uniform_aes_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:Uniform_aes_start_offset] + Uniform_aes + flash[Uniform_aes_start_offset+16:]
# end added for TEE keybank encryption

# added for ns-uboot
    if 'EmbeddedTeeRsaPubMStar' in sboot_nmap:
        TEE_RSA_pub_start = int('0x' + sboot_nmap['EmbeddedTeeRsaPubMStar'][0], 16)
        tee_rsa_pub = open(join('..', 'tee_loader', 'secure', 'RSAcus_pub.bin'), 'rb').read()
        pub_key_size = len(tee_rsa_pub)
        print '    pub_key_size:         0x%08x' % pub_key_size
        TEE_RSA_pub_start_offset = _ld_LDR_load_start + (TEE_RSA_pub_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:TEE_RSA_pub_start_offset] + tee_rsa_pub + flash[TEE_RSA_pub_start_offset+pub_key_size:]

    if 'EmbeddedTeeRsaPubCA' in sboot_nmap:
        TEE_RSA_pub_start = int('0x' + sboot_nmap['EmbeddedTeeRsaPubCA'][0], 16)
        tee_rsa_pub = open(join('..', 'tee_loader', 'secure', 'RSAcus_pub.bin'), 'rb').read()
        pub_key_size = len(tee_rsa_pub)
        print '    pub_key_size:         0x%08x' % pub_key_size
        TEE_RSA_pub_start_offset = _ld_LDR_load_start + (TEE_RSA_pub_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:TEE_RSA_pub_start_offset] + tee_rsa_pub + flash[TEE_RSA_pub_start_offset+pub_key_size:]

    if 'EmbeddedReeRsaPubMStar' in sboot_nmap:
        REE_RSA_pub_start = int('0x' + sboot_nmap['EmbeddedReeRsaPubMStar'][0], 16)
        ree_rsa_pub = open(join('secure', 'RSAree_pub.bin'), 'rb').read()
        pub_key_size = len(tee_rsa_pub)
        REE_RSA_pub_start_offset = _ld_LDR_load_start + (REE_RSA_pub_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:REE_RSA_pub_start_offset] + ree_rsa_pub + flash[REE_RSA_pub_start_offset+pub_key_size:]

    if 'EmbeddedReeRsaPubCA' in sboot_nmap:
        REE_RSA_pub_start = int('0x' + sboot_nmap['EmbeddedReeRsaPubCA'][0], 16)
        ree_rsa_pub = open(join('secure', 'RSAree_pub.bin'), 'rb').read()
        pub_key_size = len(tee_rsa_pub)
        REE_RSA_pub_start_offset = _ld_LDR_load_start + (REE_RSA_pub_start - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:REE_RSA_pub_start_offset] + ree_rsa_pub + flash[REE_RSA_pub_start_offset+pub_key_size:]

    if '_gWarmbootTable' in sboot_nmap:
        _gWarmbootTable = int('0x' + sboot_nmap['_gWarmbootTable'][0], 16)
        _gWarmbootTableValue = pack('<III', _ld_LDR_run_base, (_ld_LDR_run_end-_ld_LDR_run_start), _ld_LDR_run_start)

        WARMBOOT_TABLE_offset = _ld_LDR_load_start + (_gWarmbootTable - _ld_LDR_run_base) - FLASH_BASE_ADDR
        flash = flash[:WARMBOOT_TABLE_offset] + _gWarmbootTableValue + flash[WARMBOOT_TABLE_offset+12:]

    # secure range info in hash1
    if '_ld_TEE_LDR_run_base' in sboot_nmap and '_ld_TEE_LDR_run_end' in sboot_nmap:
        miu_bus_addr_offset = 0x20000000
        secure_range_addr_align = 0x1000
        secure_range_start_ba = int('0x' + sboot_nmap['_ld_TEE_LDR_run_base'][0], 16)
        secure_range_end_ba = int('0x' + sboot_nmap['_ld_TEE_LDR_run_end'][0], 16)
        secure_range_addr_offset = 0xFB8
        if secure_range_start_ba < miu_bus_addr_offset or secure_range_end_ba < miu_bus_addr_offset or secure_range_start_ba%secure_range_addr_align!=0 or secure_range_end_ba%secure_range_addr_align!=0:
            print "FAIL to insert secure range info in %s" % __file__
            exit(1)
        flash = flash[:_ld_HASH1_run_start - _ld_load_start + secure_range_addr_offset] + pack('<II', secure_range_start_ba-miu_bus_addr_offset, secure_range_end_ba-miu_bus_addr_offset) + flash[_ld_HASH1_run_start - _ld_load_start + secure_range_addr_offset + 8:]
# end added for ns-uboot

# Extract TEE_loader.bin
    if '_ld_TEE_LDR_signature' in sboot_nmap:
        tee_loader_bin = flash[(_ld_TEE_LDR_load_start-FLASH_BASE_ADDR):(_ld_TEE_LDR_load_end-FLASH_BASE_ADDR)]
        with open(join(bindir,'tee_loader.bin'), 'w') as output:
            output.write(tee_loader_bin) 
# End extract TEE_loader.bin

    if argv[7] != "tvbfr":
        flash = flash[:_ld_APP_load_start - FLASH_BASE_ADDR] + header + app_zip
    else:
        flash = flash + header + app_zip

    out_path = join(bindir, FLASH_OUT % postfix)
    print PREFIX, 'Generate', out_path

    if argv[7] != "tvbfr":
        if enable_tee_ldr == 'y':
            size = len(flash[_ld_REE_LDR_load_start - FLASH_BASE_ADDR:])
            print 'REE_loader + APP size:  0x%08x' % size
            print '_ld_REE_LDR_load_start: 0x%08x' % (_ld_REE_LDR_load_start - FLASH_BASE_ADDR)
            open(join(bindir, FLASH), 'wb').write(flash[:_ld_REE_LDR_load_end-FLASH_BASE_ADDR])
        else:
            open(join(bindir, FLASH), 'wb').write(flash[:_ld_LDR_load_end-FLASH_BASE_ADDR])

        open(out_path, 'wb').write(flash)

        if len(argv) > 7:
            gen_bin(argv , flash, header, app_zip, dst, entry, zip_size, size, HEADER_SIZE, app_bin_path)
    else:
        open(out_path, 'wb').write(flash)

if __name__ == '__main__':
    # Use default locale : traditional C English
    os.environ['LC_ALL'] = 'C'
    main(sys.argv)
