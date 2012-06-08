import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1",1337));
s.send("Hey :)")
print s.recv(64)
s.close()
raw_input("Press enter to exit")