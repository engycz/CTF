table = ['SQUIRELL*JUDGE*NEWS*LESSON',
         'WORRY*UPDATE*SEAFOOD*CROSS',
         'CHAPTER*SPEEDBUMP*CHECKERS',
         'PHONE*HOPE*NOTEBOOK*ORANGE',
         'CARTOONS*CLEAN*TODAY*ENTER',
         'ZEBRA*PATH*VALUABLE*MARINE',
         'VOLUME*REDUCE*LETTUCE*GOAL',
         'BUFFALOS*THE*CATCH*SUPREME',
         'LONG*OCTOPUS*SEASON*SCHEME',
         'CARAVAN*TOBACCO*WORM*XENON',
         'PUPPYLIKE*WHATEVER*POPULAR',
         'SALAD*UNKNOWN*SQUATS*AUDIT',
         'HOUR*NEWBORN*TURN*WORKSHOP',
         'USEFUL*OFFSHORE*TOAST*BOOK',
         'COMPANY*FREQUENCY*NINETEEN',
         'AMOUNT*CREATE*HOUSE*FOREST',
         'BATTERY*GOLDEN*ROOT*WHEELS',
         'SHEEP*HOLIDAY*APPLE*LAWYER',
         'SUMMER*HORSE*WATER*SULPHUR']

passLen = 18
rows = 19
cols = 26

def AddToPass(pwd, row, col):
    if row >= 0 and row < rows:
        if col >= 0 and col < cols:
            return pwd + table[row][col]

def GenPass(rowDir, colDir):
    pwd = ''
    for i in range(passLen):
        pwd = AddToPass(pwd, row + i * rowDir, col + i * colDir)
        if pwd == None:
            return

    print(pwd)

for row in range(rows):
    for col in range(cols):
        GenPass( 0, -1) # Left
        GenPass( 0,  1) # Right
        GenPass(-1,  0) # Up
        GenPass( 1,  0) # Down

        GenPass(-1, -1) # Left + Up
        GenPass(-1,  1) # Rigth + Up
        GenPass( 1,  1) # Right + Down
        GenPass( 1, -1) # Left + Down
