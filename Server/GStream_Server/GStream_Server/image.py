import struct,pygame,pygame.gfxdraw
f = open("test.data",'rb')
pygame.init() 

window = pygame.display.set_mode((1280,720)) 
for h in range(1080):
	for w in range(1920):
		try:
			pygame.gfxdraw.pixel(window, w, h, struct.unpack("BBB",f.read(3)))
		except:
			pygame.gfxdraw.pixel(window, w, h, (255,0,0))
pygame.display.update()
raw_input("Press enter to exit")
