from socket import \
socket, AF_INET, SOCK_STREAM
s = socket(AF_INET, SOCK_STREAM)
s.connect(('127.0.0.1', 9999))
s.send('Hello, world')
data = s.recv(1024)
s.close()
print 'Received', 'data'