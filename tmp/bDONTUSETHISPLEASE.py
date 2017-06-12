x = open("LISTFILES.txt","r")
f = x.readlines()
n = open("LISTFILESCMAKELISTS.txt","w")
for line in f:
    n.write(("  FILE %s %s"%(line.strip("\n"),line.strip("\n")))+"\n")
x.close()
n.close()