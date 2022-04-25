import sys
from PIL import Image

wall = (0,0,0) #black
space = (255, 255, 255) #white
special = (255,0,0)

rawFile = open(sys.argv[1], 'r')

fromFile = rawFile.read()
rawFile.close()

lines = fromFile.split('\n')[0:-1]
#print('\n'.join(lines))
length = len(lines)
width = len(lines[0])
grid = [0]*length
for i in range(length):
    grid[i] = [0]*width
    for j in range(width):
        grid[i][j] = lines[i][j]
img = Image.new(mode = "RGB", size = (length, width))
px = img.load()
for i in range(length):
    for j in range(width):
        if grid[i][j] == '#':
            px[j, i] = wall
        elif grid[i][j] == ' ':
            px[j, i] = space
        else:
            px[j, i] = special
img.save(sys.argv[2],"PNG")
