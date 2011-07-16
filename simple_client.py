
import socket
import Image, StringIO

s = socket.socket()
s.connect(('192.168.1.2', 1212))
s.settimeout(2)
while 1:
    s.send("a\r\n")

    b = s.recv(20)

    [l, b] = b.split(":")
    print l

    img = s.recv(int(l))

print "finished"

