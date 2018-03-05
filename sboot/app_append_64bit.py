#!/usr/bin/python
"""
./app_append.py BINDIR, APP_ELF_PATH, APP_BIN_PATH, APP_ZIP_PATH, POSTFIX, has_trunk_head/no_trunk_head  bfn/bfe app_head_size
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
        for m in re.findall('([0-9a-zA-Z]{16})\s*(.)\s*(.+)', out)])


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

def gen_bin(boot_from, flash, header, app_zip, dst, entry, zip_size, size, HEADER_SIZE, app_bin_path):
    if boot_from == 'bfe':
        #for generate bfe_usb_upgrade_loader.bin
        loader_bin = open(join(bindir, FLASH), 'rb').read()[_ld_LDR_load_start - _ld_load_start:_ld_LDR_load_end - _ld_load_start]
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

    elif boot_from == 'bfn':
        #for generate bfn_miu.bin
        miu_bin = flash[_ld_HASH1_run_start - _ld_load_start:_ld_HASH1_run_end - _ld_load_start]
        miu_header = '\0' * 0x100
        miu_bin = miu_header + miu_bin +  '\0' * ((4-len(miu_bin)%4)%4) + 'PARE'  #4 byte alignment + ENDTAG "PARE"
        miu_header = pack('<4cII','P','A','R','M', 0 , len(miu_bin)) + miu_header[12:] #0x100 header: TAG"PARM" + reserved(4) + size(3) + reserved(left)
        miu_bin = miu_header + miu_bin[0X100:]

        if os.path.exists(join(bindir,MIU_OUT)):
            os.remove(join(bindir,MIU_OUT))
        open(join(bindir,MIU_OUT), 'wb').write(miu_bin)

        #add BIN0 and BINE tag to binary
        header = pack('<IIIII',0x304E4942, dst, entry, zip_size+4, size)
        header += '\0' * (HEADER_SIZE - len(header))
        app_zip += 'BINE'

        flash_app = (header + app_zip)
        if os.path.exists(join(bindir, APP_OUT)):
            os.remove(join(bindir,APP_OUT))
        open(join(bindir, APP_OUT), 'wb').write(flash_app)

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


def add_chunk_header(app_name, boot_from):
    sboot_nmap = get_nmap(os.environ['NM'], join(bindir, SBOOT_ELF))
    app_bin = 0x0
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
        return #boot from other

    app_base = app_bin[app_bin_start:app_bin_start+0x4]
    app_base, = struct.unpack("I",app_base)
    app_base = app_base - app_base_offset

    app_entry_off = app_bin[app_bin_start+0x4:app_bin_start+0x8]
    app_entry_off, = struct.unpack("I",app_entry_off)
    app_entry_off = app_entry_off + app_base_offset

    # For secure boot, the keybank size can not be counted in app_size
    #app_size = len(app_bin)
    #app_size = (app_size + 31) / 32  * 32

    app_size = app_bin[app_bin_start+0x8:app_bin_start+0xC]
    app_size, = struct.unpack("I", app_size)
    app_size = app_size + app_base_offset

    chunk_header = None
    if boot_from == 'bfn':
        app_bin = app_bin[:app_size] + 'BINE' + app_bin[app_size + 4:]
        chunk_header = pack('<IIII', 0x304E4942, app_base, app_entry_off, app_size + 4)
    elif boot_from == 'bfe':
        chunk_header = pack('<IIII', app_base, app_entry_off, app_size, 0xFFFFFFFF)
    else:
        return #boot from other

    app_bin = chunk_header + app_bin[16:]
    open(join(bindir,app_name), 'wb').write(app_bin)
    return app_size


def main(argv):

    global _ld_HASH0_run_start, _ld_HASH1_run_start, _ld_HASH1_run_end , _ld_LDR_load_start, _ld_LDR_load_end
    global _ld_load_start, AppDigest, _ld_LDR_run_base, _ld_REE_LDR_run_base, _ld_REE_LDR_load_start, bindir
    global _ld_APP_load_start, postfix

    bindir, app_elf_path, app_bin_path, app_zip_path, postfix = argv[1:6]
    HEADER_SIZE = int(argv[11], 16)
    READELF = os.environ['READELF']
    NM = os.environ['NM']

    sboot_nmap = get_nmap(NM, join(bindir, SBOOT_ELF))
    _ld_load_start = int('0x' + sboot_nmap['_ld_load_start'][0], 16)
    _ld_APP_load_start = int('0x' + sboot_nmap['_ld_APP_load_start'][0], 16)

    _ld_LDR_load_start = int('0x' + sboot_nmap['_ld_LDR_load_start'][0], 16)
    _ld_LDR_load_end = int('0x' + sboot_nmap['_ld_LDR_load_end'][0], 16)

    _ld_LDR_run_base = int('0x' + sboot_nmap['_ld_LDR_run_base'][0], 16)
    if '_ld_TEE_LDR_load_start' in sboot_nmap:
        _ld_REE_LDR_run_base = int('0x' + sboot_nmap['_ld_REE_LDR_run_base'][0], 16)
        _ld_REE_LDR_load_start = int('0x' + sboot_nmap['_ld_REE_LDR_load_start'][0], 16)
        _ld_TEE_LDR_load_start = int('0x' + sboot_nmap['_ld_TEE_LDR_load_start'][0], 16)
        _ld_TEE_LDR_load_end = int('0x' + sboot_nmap['_ld_TEE_LDR_load_end'][0], 16)
    _ld_LDR_run_start = int('0x' + sboot_nmap['_ld_LDR_run_start'][0], 16)
    _ld_LDR_run_end = int('0x' + sboot_nmap['_ld_LDR_run_end'][0], 16)
    AppDigest = int('0x' + sboot_nmap['AppDigest'][0], 16)

    _ld_HASH1_run_start = int('0x' + sboot_nmap['_ld_HASH1_run_start'][0], 16)
    _ld_HASH1_run_end = int('0x' + sboot_nmap['_ld_HASH1_run_end'][0], 16)

    _ld_HASH0_run_start = int('0x' + sboot_nmap['_ld_HASH0_run_start'][0], 16)
    FLASH_BASE_ADDR = _ld_load_start#_ld_HASH0_run_start - 0x2000

    if len(argv) > 6:
        if argv[6] == 'has_trunk_head':
            if argv[7] == 'bfn':
                print 'Generate bfn_app.bin(with chunk header)'
                add_chunk_header(APP_OUT, 'bfn')
            elif argv[7] == 'bfe':
                print 'Generate bfe_app.bin(with chunk header)'
                add_chunk_header(BFE_APP_OUT, 'bfe')
            else:
                return #boot from other
            return 0


    print 'generate flash_wth_uboot.bin, bfn_app, bfn_loader.bin, bfn_app.bin'

    app_entry, app_ram_start = get_app_info(READELF, app_elf_path, postfix)

    if len(argv) > 9:
        if argv[9] == 'arm' and postfix == 'ecos':
            # arm .rom_vector address is 0x01020000,but sboot base address is 0x20000000
            app_entry     = app_entry     | ARM_SBOOT_BASE # add sboot base address
            app_ram_start = app_ram_start | ARM_SBOOT_BASE # add sboot base address

    print PREFIX, 'Address:'
    print '    _ld_APP_load_start:  0x%08x' % _ld_APP_load_start
    print '    _ld_HASH0_run_start: 0x%08x' % _ld_HASH0_run_start
    print '    _ld_HASH1_run_start: 0x%08x' % _ld_HASH1_run_start
    print '    _ld_HASH1_run_end:   0x%08x' % _ld_HASH1_run_end
    print '    app_ram_start:       0x%08x' % app_ram_start
    print '    app_entry:           0x%08x' % app_entry
    print '    _ld_LDR_load_start:  0x%08x' % _ld_LDR_load_start
    print '    _ld_LDR_load_end:    0x%08x' % _ld_LDR_load_end
    print '    HEADER_SIZE:         0x%08x' % HEADER_SIZE
    print '    FLASH_BASE_ADDR:     0x%08x' % FLASH_BASE_ADDR

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
        secure_info_bin_size = 0x10000 # this value equals Makefile.mboot MBOOT_ALIGNMENT_SIZE
        header = pack('<IIIII', dst, entry, zip_size, size, zip_size)
    else:
        header = pack('<IIII', dst, entry, zip_size, size)
    header += '\0' * (HEADER_SIZE - len(header))

    print PREFIX, 'header:', len(header)
    flash = open(join(bindir, FLASH), 'rb').read()

    # Replace the APP digest
    h = hashlib.sha256(header + app_zip)
    l = len(h.digest())
    print PREFIX, 'APP Digest:', h.hexdigest()
    if '_ld_TEE_LDR_load_start' in sboot_nmap:
        AppDigest = _ld_REE_LDR_load_start + (AppDigest - _ld_REE_LDR_run_base) - FLASH_BASE_ADDR
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

# added for Secure_Boot
    if 'EmbeddedMBootRsaPub' in sboot_nmap:
        EmbeddedMBootRsaPub = int('0x' + sboot_nmap['EmbeddedMBootRsaPub'][0], 16)
        MBoot_RSA_pub = open(join('secure', 'RSAboot_pub.bin'), 'rb').read()
        pub_key_size = len(MBoot_RSA_pub)

        if '_ld_TEE_LDR_load_start' in sboot_nmap:
            MBoot_RSA_pub_start = _ld_REE_LDR_load_start + (EmbeddedMBootRsaPub - _ld_LDR_run_base) - FLASH_BASE_ADDR
        else:
            MBoot_RSA_pub_start = _ld_LDR_load_start + (EmbeddedMBootRsaPub - _ld_LDR_run_base) - FLASH_BASE_ADDR
        print 'MBoot_RSA_pub_start ->', hex(MBoot_RSA_pub_start)
        flash = flash[:MBoot_RSA_pub_start] + MBoot_RSA_pub + flash[MBoot_RSA_pub_start + pub_key_size:]

    if 'EmbeddedMBootAes' in sboot_nmap:
        EmbeddedMBootAes = int('0x' + sboot_nmap['EmbeddedMBootAes'][0], 16)
        MBoot_Aes = open(join('secure', 'AESree.bin'), 'rb').read()
        key_size = len(MBoot_Aes)

        if '_ld_TEE_LDR_load_start' in sboot_nmap:
            MBoot_Aes_start = _ld_REE_LDR_load_start + (EmbeddedMBootAes - _ld_LDR_run_base) - FLASH_BASE_ADDR
        else:
            MBoot_Aes_start = _ld_LDR_load_start + (EmbeddedMBootAes - _ld_LDR_run_base) - FLASH_BASE_ADDR
        print 'MBoot_Aes_start ->', hex(MBoot_Aes_start)
        flash = flash[:MBoot_Aes_start] + MBoot_Aes + flash[MBoot_Aes_start + key_size:]
# end added for Secure_Boot

# Extract TEE_loader.bin
    if '_ld_TEE_LDR_signature' in sboot_nmap:
        tee_loader_bin = flash[(_ld_TEE_LDR_load_start-FLASH_BASE_ADDR):(_ld_TEE_LDR_load_end-FLASH_BASE_ADDR)]
        with open(join(bindir,'tee_loader.bin'), 'w') as output:
            output.write(tee_loader_bin) 
# End extract TEE_loader.bin

# Replace AppDigest for BFN
    if len(argv) > 7 and argv[7] == 'bfn':
        #add BIN0 and BINE tag to binary
        headerBFN = pack('<IIIII',0x304E4942, dst, entry, zip_size+4, size)
        headerBFN += '\0' * (HEADER_SIZE - len(headerBFN))
        app_zipBFN = app_zip + 'BINE'

        # regenerate the APP digest for BFN
        h = hashlib.sha256(headerBFN + app_zipBFN)
        l = len(h.digest())
        print PREFIX, 'Regenerate APP Digest:', h.hexdigest()
        flash = flash[:AppDigest] + h.digest()[::-1] + flash[AppDigest+l:]
# End Replace AppDigest for BFN

    flash = flash[:_ld_APP_load_start - FLASH_BASE_ADDR] + header + app_zip

    out_path = join(bindir, FLASH_OUT % postfix)
    print PREFIX, 'Generate', out_path
    open(out_path, 'wb').write(flash)

    if len(argv) > 7:
        gen_bin(argv[7] , flash, header, app_zip, dst, entry, zip_size, size, HEADER_SIZE, app_bin_path)

if __name__ == '__main__':
    # Use default locale : traditional C English
    os.environ['LC_ALL'] = 'C'
    main(sys.argv)
