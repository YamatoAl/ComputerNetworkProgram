import SocketServer
class UCHandler(SocketServer.BaseRequestHandler):
   def handle(self):
      print "Connected:", self.client_address
      while 1:
         data = self.request.recv(1024)
         if data == "\r\n":
            break
            print data[:-2]
         self.request.send(data.upper())
myaddr = ('127.0.0.1', 2345)
myserver = SocketServer.TCPServer(myaddr, UCHandler)
#myserver = SocketServer.ThreadingTCPServer(myaddr, UCHandler)
myserver.serve_forever()

