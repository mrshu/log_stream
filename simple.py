import socket
import sys

import base64
import StringIO
import wx
import time


s = socket.socket()
s.connect(('192.168.1.3', 1212))


class Panel1(wx.Panel):
    """ class Panel1 creates a panel with an image on it, inherits wx.Panel """
    def __init__(self, parent, id):
        # create the panel
        wx.Panel.__init__(self, parent, id)
        # show the bitmap, (5, 5) are upper left corner coordinates
        self.sb = wx.StaticBitmap(self, -1, pos=(5, 5), #size=(160, 120))
        size=(640, 480))
        
        self.bmp = wx.Bitmap('bmp.jpg')

        #self.Bind(wx.EVT_PAINT, self.OnPaint)


        self.timer = wx.Timer(self, 1001)
        self.Bind(wx.EVT_TIMER, self.update, id=1001)
        self.timer.Start(1)
        self.i = 0
        self.lasttime = time.time()



    def update(self, event):
        #print "loading"
        s.send("a\r\n")

        l = s.recv(8)
        
        #print l
        l = int(l)
        img = s.recv(l)


        ll = len(img)

        while ll != l:
            tmp = s.recv(128)
            if not tmp: break
            ll += len(tmp)
            img += tmp
        
        img = StringIO.StringIO(img)
        try:
            i = wx.ImageFromStream(img)
            self.bmp = wx.BitmapFromImage(i)
        except:
            return

        self.sb.SetBitmap(self.bmp)
        self.i += 1
        
        if (self.lasttime+5.0) <= time.time():
            print "FPS:", self.i/5.0
            self.lasttime = time.time()
            self.i = 0

        self.Refresh()
        wx.Yield()



app = wx.PySimpleApp()
frame1 = wx.Frame(None, -1, "Socket test", size = (400, 300))
Panel1(frame1, -1)
frame1.Show(1)
app.MainLoop()
