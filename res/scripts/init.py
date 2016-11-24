import sys
import importtest

print("Entry script test:")
for p in sys.path:
    print("\tpathentry - %s" % (p))

importtest.hi()
