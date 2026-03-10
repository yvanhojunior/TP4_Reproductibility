#!/usr/bin/env python3
import sys

def checksum(s):
    v = 0
    for i, c in enumerate(s):
        v += (i + 1) * (i + 1) * ord(c)
        v = v % 1000000000
    v = v + len(s)
    s = str(v) * 12
    return s[0:12]

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./checksum.py <string>")
        sys.exit(1)
    print(checksum(sys.argv[1]))
