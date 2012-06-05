import struct,pygame,pygame.gfxdraw
f = open("test.data",'rb')
pygame.init() 

window = pygame.display.set_mode((1280,720),pygame.HWSURFACE) 
for h in range(720):
	for w in range(1280):
		try:
			data = struct.unpack("BBB",f.read(3))
			pygame.gfxdraw.pixel(window, w, h, data)
		except:
			pygame.gfxdraw.pixel(window, w, h, (255,0,0))
	pygame.display.update()
f.close()
pygame.image.save(window, "test.data.jpg")
raw_input("Press enter to exit")
