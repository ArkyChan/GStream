import struct,pygame
f = open("test.data",'rb')
pygame.init() 

window = pygame.display.set_mode((1280,720),pygame.HWSURFACE) 
for q in range(25*3):
	for h in range(720):
		for w in range(1280):
			try:
				data = struct.unpack("BBB",f.read(3));
				window.set_at((w,h),data)
			except:
				pass
		pygame.display.update()
f.close()
pygame.image.save(window, "test.data.jpg")
raw_input("Press enter to exit")
