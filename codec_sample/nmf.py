#!/usr/bin/env python

import sys
import struct

if __name__=='__main__':

    fin = open(sys.argv[1], 'rb')
    fout = open(sys.argv[2], 'wb')
    fin.seek(166)

    while 1:
        label = struct.unpack('i', fin.read(4))[0]
        if (label != 4):
            break

        fin.read(16)
        al = struct.unpack('i', fin.read(4))[0]
        hl = struct.unpack('i', fin.read(4))[0]
        fin.read(hl)
        buf = fin.read(al - hl)
        fout.write(buf)

    fin.close()
    fout.close()

