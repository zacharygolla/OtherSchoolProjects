from pwn import *

host = '127.0.0.1'
port = 19999

r = remote(host, port)

d = r.recv()
print d

while ("FLAG" not in d):
    s=d.split("'")[-2]
    c = chr(int(s,2))
    print c
    r.send(c+'\n')
    d = r.recv()
    print d
