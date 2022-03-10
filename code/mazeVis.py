import sys
from PIL import Image

rawFile = open(sys.argv[1], 'r')

fromFile = rawFile.read()
rawFile.close()

lines = fromFile.split('\n')[1:-1]
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
        px[i, j] = (0,0,0) if grid[i][j] == '#' else (255, 255, 255)
img.save("outs/maze.png","PNG")
