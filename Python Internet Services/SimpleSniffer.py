#!/usr/bin/python
import socket
#create an INET, raw socket
s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
# receive a packet
while True:
   # print output on terminal
   print s.recvfrom(65565)