import os
import sys
import struct
from struct import pack, unpack

currdir = os.path.dirname(os.path.realpath(__file__))

if len(sys.argv) < 3:
    print >> sys.stderr, '\n\tUsage:\n\t\t python %s <tool folder> <vendor>\n\n' % sys.argv[0]
    sys.exit(1)

secure_dir = sys.argv[1]
vendor = sys.argv[2]

if not os.path.exists(secure_dir):
    print >> sys.stderr, '\n\t%s not found\n\n' % secure_dir
    sys.exit(1)

all_ree_data = [ f for f in os.listdir(os.path.join(currdir, vendor, 'REE')) if f.startswith('data_')]
all_ree_data.sort()

# construct KLdata.bin
outdata_REE = ""

for f in all_ree_data:
    with open(os.path.join(currdir, vendor, 'REE', f), 'rb') as fi:
        outdata_REE += fi.read()

with open(os.path.join(currdir, vendor, "KLdata_REE.bin"), 'wb') as outfile:
    header = pack('<IIII', len(all_ree_data), 0, 0, 0)
    outfile.write(header)
    outfile.write(outdata_REE)
