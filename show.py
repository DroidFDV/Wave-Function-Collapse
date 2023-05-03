from PIL import Image
import enum

class Color(enum.Enum):
    black = -1
    white = 5
    green = 3
    blue = 0
    yellow = 2
    grey = 4
    light_blue = 1
    
class TC(enum.Enum):
    black = (0, 0, 0)
    white = (255, 255, 255)
    green = (0, 255, 0)
    blue = (0, 0, 255)
    yellow = (255, 255, 0)
    
colors = {-1 : (0, 0, 0), 5 : (255, 255, 255), 3 : (0, 255, 0), 0 : (0, 0, 255), 2 : (255, 255, 0), 4 : (128, 128, 128), 1 : (128, 128, 255)}

map = open('map.txt')

w, l  = map.readline().split()
w = int(w)
l = int(l)


koef = 3

img = Image.new('RGBA', (w*koef, l*koef), 'white')

values = [int(i) for i in map.readline().split()]

def f(c, tc):
    for i in tc:
        if c.name == i.name:
            return i.value
        
for i in range(w):
    for j in range(l):
        s = colors[values[j + i*l]]
        
        for k in range(3):
            for m in range(3):
                img.putpixel((j*3+k, i*3+m), s)
        

img.show()