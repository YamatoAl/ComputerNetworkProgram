import CGIHTTPServer, BaseHTTPServer
httpd = BaseHTTPServer.HTTPServer(('', 8888),
      CGIHTTPServer.CGIHTTPRequestHandler)
httpd.serve_forever()
