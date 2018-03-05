#!/usr/bin/python
import sys
import os
import re
import subprocess
from struct import pack, unpack

APP_RAM_START = 0x875f0180
APP_ENTRY = 0x875f0600
SBOOT_BIN = 'sboot.bin'
APP_BIN = 'u-boot.bin'

HEADER_SIZE = 0x100
SBOOT_ELF = 'bin/sboot.elf'
FLASH = 'bin/flash.bin'
FLASH_OUT = 'bin/flash_with_uboot.bin'
LOADER_OUT = 'bin/bfn_loader.bin'
MIU_OUT = 'bin/bfn_miu.bin'
APP_OUT = 'bin/bfn_app.bin'

def main():
    NM = os.environ['NM']
    if not NM:
        sys.exit(1)

    uboot_bin_fn = os.path.join(sys.argv[1], APP_BIN)

    out = subprocess.Popen([NM, SBOOT_ELF], stdout=subprocess.PIPE).communicate()[0]
    sboot_nmap = dict([(m[2], (m[0], m[1])) for m in re.findall('([0-9a-zA-Z]{8})\s*(.)\s*(.+)', out)])

    _ld_LDR_load_start = int('0x' + sboot_nmap['_ld_LDR_load_start'][0], 16)
    _ld_LDR_load_end = int('0x' + sboot_nmap['_ld_LDR_load_end'][0], 16)
    print '  [BFN][LOADER] _ld_LDR_load_start==0x%08x _ld_LDR_load_end=0x%08x' % (_ld_LDR_load_start, _ld_LDR_load_end)

    loader_bin = open(FLASH, 'rb').read()[_ld_LDR_load_start - 0x94000000:_ld_LDR_load_end - 0x94000000]
    if os.path.exists(LOADER_OUT):
        os.remove(LOADER_OUT)
    open(LOADER_OUT, 'wb').write(loader_bin)

    _ld_HASH1_run_start = int('0x' + sboot_nmap['_ld_HASH1_run_start'][0], 16)
    _ld_HASH1_run_end = int('0x' + sboot_nmap['_ld_HASH1_run_end'][0], 16)
    print '  [BFN][MIU] _ld_HASH1_run_start==0x%08x _ld_HASH1_run_end=0x%08x' % (_ld_HASH1_run_start, _ld_HASH1_run_end)

    miu_bin = open(FLASH, 'rb').read()[_ld_HASH1_run_start - 0x94000000:_ld_HASH1_run_end - 0x94000000]
    if os.path.exists(MIU_OUT):
        os.remove(MIU_OUT)
    open(MIU_OUT, 'wb').write(miu_bin)

    dst = APP_RAM_START - HEADER_SIZE
    size = os.stat(uboot_bin_fn).st_size + HEADER_SIZE
    size = (size / 32 + 1) * 32
    entry = APP_ENTRY - dst
    app_bin = open(uboot_bin_fn, 'rb').read()

    header = pack('<IIIII', dst, size, 0, size, entry)
    header += '\0' * (HEADER_SIZE - len(header))

    print '  [BFN][APP] header:', len(header)
    flash = (header + app_bin)

    if os.path.exists(APP_OUT):
        os.remove(APP_OUT)
    open(APP_OUT, 'wb').write(flash)

if __name__ == '__main__':
    main()
