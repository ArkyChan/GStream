import socket,struct,time,pygame,time
from OpenGL.GL import *
from OpenGL.GLU import *

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
packets = 100
try:
	s.connect(("127.0.0.1",1337))
	
	pygame.init() 
	pygame.display.set_mode((1280,720),pygame.DOUBLEBUF | pygame.OPENGL) 
	
	s.send('\x09')
	recvSize = struct.unpack('I',s.recv(4))[0]
	print "RecvSize: " + str(recvSize)
	data = s.recv(recvSize)
	frameCount = 0
	
	while len(data)>0:
		print "Frame got!"
		data = s.recv(recvSize)
		
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos2i(-1, 1); 
		glPixelZoom(1,-1); 
		glDrawPixels(1280,720,GL_RGB, GL_UNSIGNED_BYTE , data);
		glEnd()
		
		frameCount += 1
		pygame.display.flip()
		
		
	print frameCount
except Exception as inst:
	print inst
	
s.close()
raw_input("Press enter to exit")