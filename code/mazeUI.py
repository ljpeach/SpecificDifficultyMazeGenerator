import sys
import os
import subprocess
import time

def neuralNetInput(difficulty, length, width):
    return (length, width, 0, 0.1, 0, 0.1, 0, 0.1, 1, 0, 1, 0)

def neuralNetFeedback(desired, actual, length, width):
    pass

def choicesToDict(choiceList):
    choiceDict = {}
    option = 0
    for choice in choiceList:
        choiceDict[option] = choice
        option+=1
    return choiceDict

def printUI(choiceDict, out = None):
    ui = "Enter the number of the option you wish to choose:\n"
    if out:
        for key in choiceDict.keys():
            ui+=str(key)+":"+choiceDict[key][0]+("" if out[key] == None else " : current val: "+str(out[key]))+"\n"
        return ui
    for key in choiceDict.keys():
        ui+=str(key)+":"+choiceDict[key][0]+"\n"
    return ui

def fillerChoice():
    pass

def getInInt():
    while True:
        try:
            return int(input("Enter Value:\n"))
        except:
            input("Bad input. Enter to retry.")
            continue

def getInFloat():
    while True:
        try:
            return float(input("Enter Value:\n"))
        except:
            input("Bad input. Enter to retry.")
            continue

def listOuts():
    print(getOuts())
    input()

def getOuts():
    return subprocess.run(["ls", "outs"], stdout=subprocess.PIPE).stdout.decode()

def strToChoiceList(string, filter):
    ans = {}
    key = 0
    for i in string.split("\n")[:-1]:
        if filter in i:
            continue
        ans[key] = (i, fillerChoice)
        key+=1
    return ans

def collectMazeParam(outPlaceHold):
    choices = [ ("Specify length", getInInt),
                ("Specify width", getInInt),
                ("Specify old node insertion low point (0<=old node low<=old node high<=1)", getInFloat),
                ("Specify old node insertion high point (0<=old node low<=old node high<=1)", getInFloat),
                ("Specify new node insertion low point (0<=new node low<=new node high<=1)", getInFloat),
                ("Specify new node insertion high point (0<=new node low<=new node high<=1)", getInFloat),
                ("Specify node pop low poin (0<=pop low<=pop high<=1)", getInFloat),
                ("Specify node pop high point (0<=pop low<=pop high<=1)", getInFloat),
                ("Specify left turn bias (0<=left bias<=1)", getInFloat),
                ("Specify straight turn bias (0<=straight bias<=1)", getInFloat),
                ("Specify right turn bias (0<=right bias<=1)", getInFloat),
                ("Specify solution percentile (0<=solution percentile<=1, 0 is longest solution.)", getInFloat),
                ("Confirm", "return"),
                ("Cancel", fillerChoice)
    ]
    return menuHandler(choices, outPlaceHold)

def makeMaze():
    out = None
    while True:
        out = collectMazeParam(out)
        if out==None:
            return
        length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7], out[8], out[9], out[10], out[11]
        if None in out[:-2]:
            ans = input("Missing important values. type 'def' to use default values, enter to try again.\n")
            if ans == "def":
                length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = 10, 10, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0
                break
            continue
        break

    args = ["binaries/mazeGen", str(length), str(width), str(oInLo), str(oInHi), str(nInLo), str(nInHi), str(outLo), str(outHi), str(lbias), str(sbias), str(rbias), str(solRank)]
    print("Building Maze")
    maze = subprocess.run(args, stdout = subprocess.PIPE)
    os.system('clear')
    textMaze = maze.stdout.decode()
    print(textMaze)
    ans = input("Would you like to save your maze? Y if yes.\n")
    if ans!="Y":
        return
    fileName = input("Existing maze text files: \n"+getOuts()+"\nWhat would you like to name your maze? (txt extention is automatically added. 'cancel' to cancel save.)\n")
    if fileName=='cancel':
        return
    fileName = fileName.strip()
    if fileName[-4:] != ".txt":
        fileName += ".txt"
    inFile = open("outs/"+fileName, 'w')
    inFile.write(textMaze)
    inFile.close()
    input("Save complete!\n")

def mazeToImage():
    txtDict = strToChoiceList(getOuts(), ".png")
    fileSelection = printUI(txtDict)
    while True:
        try:
            ans = int(input("Which .txt file do you wish to convert?\n"+fileSelection+str(len(txtDict))+":Cancel\n"))
            if ans > len(txtDict):
                raise ValueError()
            break
        except:
            if input("Invalid number. Enter to try again, or 'cancel' to quit.\n") == 'cancel':
                return
            continue
    if ans==len(txtDict):
        return
    srcFile = "outs/"+txtDict[ans][0]
    while True:
        try:
            ans = input("What should the output file be named?\n")
            ans = ans.strip()
            if ans[-4:] != ".png":
                ans += ".png"
            break
        except:
            if input("Invalid name. Enter to try again, or 'cancel' to quit.\n") == 'cancel':
                return
            continue
    destFile = "outs/"+ans
    subprocess.run(["python3", "code/mazeVis.py", srcFile, destFile])
    listOuts()

def difficulty():
    out = None
    while True:
        out = collectMazeParam(out)
        if out==None:
            return
        length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7], out[8], out[9], out[10], out[11]
        if None in out[:-2]:
            ans = input("Missing important values. type 'def' to use default values, enter to try again.\n")
            if ans == "def":
                length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = 10, 10, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0
                break
            continue
        break
    args = ["binaries/mazeDifficulty", str(length), str(width), str(oInLo), str(oInHi), str(nInLo), str(nInHi), str(outLo), str(outHi), str(lbias), str(sbias), str(rbias), str(solRank)]
    print("Building Maze")
    maze = subprocess.run(args, stdout = subprocess.PIPE)
    os.system('clear')
    textMaze = maze.stdout.decode()
    print(textMaze)
    ans = input("Would you like to save your maze? Y if yes.\n")
    if ans!="Y":
        return
    fileName = input("Existing maze text files: \n"+getOuts()+"\nWhat would you like to name your maze? (txt extention is automatically added. 'cancel' to cancel save.)\n")
    if fileName=='cancel':
        return
    fileName = fileName.strip()
    if fileName[-4:] != ".txt":
        fileName += ".txt"
    inFile = open("outs/"+fileName, 'w')
    inFile.write(textMaze)
    inFile.close()
    input("Save complete!\n")

def agents():
    out = None
    while True:
        out = collectMazeParam(out)
        if out==None:
            return
        length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7], out[8], out[9], out[10], out[11]
        if None in out[:-2]:
            ans = input("Missing important values. type 'def' to use default values, enter to try again.\n")
            if ans == "def":
                length, width, oInLo, oInHi, nInLo, nInHi, outLo, outHi, lbias, sbias, rbias, solRank = 10, 10, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0
                break
            continue
        break
    args = ["binaries/mazeAgents", str(length), str(width), str(oInLo), str(oInHi), str(nInLo), str(nInHi), str(outLo), str(outHi), str(lbias), str(sbias), str(rbias), str(solRank)]
    print("Building Maze")
    maze = subprocess.run(args, stdout = subprocess.PIPE)
    os.system('clear')
    textMaze = maze.stdout.decode()
    print(textMaze)
    ans = input("Would you like to save your maze? Y if yes.\n")
    if ans!="Y":
        return
    fileName = input("Existing maze text files: \n"+getOuts()+"\nWhat would you like to name your maze? (txt extention is automatically added. 'cancel' to cancel save.)\n")
    if fileName=='cancel':
        return
    fileName = fileName.strip()
    if fileName[-4:] != ".txt":
        fileName += ".txt"
    inFile = open("outs/"+fileName, 'w')
    inFile.write(textMaze)
    inFile.close()
    input("Save complete!\n")

def shutDown():
    os.system('clear')
    exit()

def runTests():
    choices = [ ("Enter the size of the mazes to test (square)", getInInt),
                ("Confirm", "return"),
                ("Cancel", fillerChoice)
    ]
    while True:
        ans = menuHandler(choices)
        if ans==None:
            return
        if None in ans[:-2]:
            input("Missing important values. Enter to try again.\n")
            continue
        break;
    os.system('clear')
    subprocess.run(["binaries/testSuite", str(ans[0])])
    input("Enter when ready to return to menu.\n")

def menuHandler(choices, out=None):
    choiceDict = choicesToDict(choices)
    if out==None:
        out = [None]*len(choices)
    while True:
        responseStr = printUI(choiceDict, out)
        os.system('clear')
        try:
            response = int(input(responseStr))
        except Exception as e:
            os.system('clear')
            input("Bad input. Press enter to return to menu.\n")
            continue
        if response not in choiceDict:
            os.system('clear')
            input("Bad input. Press enter to return to menu.\n")
            continue
        if choiceDict[response][1] == "return":
            return out
        out[response] = choiceDict[response][1]()
        if choiceDict[response] == choices[-1]:
            return None

def about():
    os.system("clear")
    about = (   "This program is designed to make many of the tasks the maze "
                "generator is in charge of easier to execute. \nMost of the menu "
                "options are self explanatory as well. While this may offer a "
                "more convenient experience, \nI would still urge you to try "
                "running each file individually as specified within the readme. "
                "Once you're \ncomfortable with what each of the inputs means, "
                "that process becomes much faster than individually entering \n"
                "each element. Have fun, and thank you for taking a look at my "
                "code!\n"
            )
    input(about)

def main():
    if not os.path.isdir("binaries"):
        os.system("make")
    choices = [ ("Create a maze", makeMaze),
                ("Convert a text maze to an image", mazeToImage),
                ("Create a maze and analyze its difficulty", difficulty),
                ("Create a maze and analyze it using agents", agents),
                ("Run test suite", runTests),
                ("View Outs", listOuts),
                ("About", about),
                ("Quit", shutDown)
            ]

    menuHandler(choices)

if __name__ == '__main__':
    main()
