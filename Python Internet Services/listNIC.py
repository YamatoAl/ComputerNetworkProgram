import netifaces
print (netifaces.interfaces())
print (netifaces.ifaddresses('lo'))
print (netifaces.AF_LINK)
addrs = netifaces.ifaddresses('ens160')
print(addrs[netifaces.AF_INET])
print(addrs[netifaces.AF_LINK])
