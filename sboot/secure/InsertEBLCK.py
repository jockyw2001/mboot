import sys
from struct import unpack, pack
from array import array


fields = ('HASH1_ESB_TYPE', 'HASH1_ESB_KDIV', '_var_HASH1_version', '_var_LDR_run_base', '_var_LDR_size')


def fill_keys(flash_path, eblck1_path, eblck2_path):
    with open(flash_path, 'rb') as fp:
        flash = fp.read()

    with open(eblck1_path, 'rb') as fp:
        eblck1 = fp.read()

    with open(eblck2_path, 'rb') as fp:
        eblck2 = fp.read()

    flash = flash[:0x6FC0] + eblck1 + flash[0x6FD0:]
    flash = flash[:0x6FD0] + eblck2 + flash[0x6FE0:]

    s, e = 0x7000 - len(fields) * 4, 0x7000
    values = list(unpack('<' + 'I' * len(fields), array('c', flash[s:e])))

#    print 'Params from flash image:'
#    for n, v in enumerate(values):
#        print '%20s: 0x%08x' % (fields[n], v)

    values[0] = 0x1 # HASH1_ESB_TYPE. Encrypted flag
    values[1] = 0x0 # HASH1_ESB_KDIV. Select
    f = pack('<' + 'I' * len(fields), *values)

    flash = flash[:s] + f + flash[e:]

    open(flash_path, 'wb').write(flash)


def main(argv):
    flash_path = argv[1] 
    eblck1_path = argv[2]
    eblck2_path = argv[3]

    fill_keys(flash_path, eblck1_path, eblck2_path)


if __name__ == '__main__':
    main(sys.argv)
