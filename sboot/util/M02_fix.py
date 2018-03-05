#!/usr/bin/python

import os
import sys
import struct
from struct import pack, unpack

IN_FILE = sys.argv[1]

if not os.path.isfile(IN_FILE):
    sys.exit("%s not found" % IN_FILE)

if not IN_FILE.endswith('.bin'):
    sys.exit('File extension is not .bin')

content = open(IN_FILE, 'rb').read()

content = content[:0x15b8]+pack('<II', 0x0, 0x0)+content[0x15c0:]

open(IN_FILE, 'wb').write(content)
