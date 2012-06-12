import socket,struct
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1",1337));
for i in range(11):
	s.send(struct.pack('b',i))
	print s.recv(64)
	
s.send(struct.pack('b',11))
print s.recv(64)

s.close()
raw_input("Press enter to exit")