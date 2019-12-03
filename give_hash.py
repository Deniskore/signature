import sys
import hashlib

if len(sys.argv) <= 1:
    print("Provide string argument")
    exit(-1)
else:
    s = sys.argv[1]
    result = ""
    for i in s:
        result = result + hashlib.sha256(i.encode("utf-8")).hexdigest() + "\n"
    print(result)
