#!/usr/bin/python3

with open("largesparsefile", "wb") as f:
    f.seek(0x100000000)
    f.write(b"hello!")
