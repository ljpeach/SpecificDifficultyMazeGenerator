import sys
import os
import subprocess
def neuralNetInput(difficulty, length, width):
    return (length, width, 0, 0.1, 0, 0.1, 0, 0.1, 1, 0, 1, 0)

def neuralNetFeedback(desired, actual, length, width):
    pass

def main(diff, length, width):
    args = neuralNetInput(diff, length, width)
    os.system("binaries/mazeGen "+" ".join([str(i) for i in args]))

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3])
