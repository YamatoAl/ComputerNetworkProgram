import SocketServer
class UCHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        remote = self.client_address
        data, skt = self.request
        print data
        skt.sendto(data.upper(), remote)
myaddr = ('127.0.0.1', 2345)
myserver = SocketServer.UDPServer(myaddr, UCHandler)
myserver.serve_forever()
