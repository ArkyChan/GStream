import socket,struct,time
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
packets = 100
try:
	s.connect(("127.0.0.1",1337));
	start = time.clock()
	for i in range(11):
		s.send(struct.pack('B',i))
		s.recv(64)
			
	time = time.clock()-start
	print "Time taken: " + str(time) + "s (" + str(11/time) + "packets/s)"
except:
	print "Can't connect to host"
	
s.send(struct.pack('B',12))
print s.recv(64)
s.close()
raw_input("Press enter to exit")