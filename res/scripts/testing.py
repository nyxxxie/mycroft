import mcore
import mcore_file
import mcore_project

print("PATH:")
for p in sys.path:
    print("\tpathentry - %s" % (p))

print("SCOPE:")
print("-----------------------------------------------------")
print(dir())
print("-----------------------------------------------------")
print(dir(mcore))
print("-----------------------------------------------------")
print(dir(mcore_file))
print(dir(mcore_file.mfile))
print("-----------------------------------------------------")
print(dir(mcore_project))
print(dir(mcore_project.mproject))
print("-----------------------------------------------------")
