
import socket
import Image, StringIO

s = socket.socket()
s.connect(('192.168.1.2', 1212))
s.settimeout(20)
while 1:
    s.send("a\r\n")

    l = s.recv(8)

    print l, 
    
    l = int(l)

    img = s.recv(l)
    
    ll = len(img)

    print ll 
    while ll != l:
        tmp = s.recv(128)
        if not tmp: break
        ll += len(tmp)
        img += tmp

    print len(img)

print "finished"

