from pytz import timezone
from datetime import datetime

lines = open('sonar.log').read().split('\n')
data = []
for line in lines:
    if 'Object detected in depth' in line:
        line = line.split(' ')
        time = datetime.strptime(line[0] + ' ' + line[1], '%Y-%m-%d %H:%M:%S')
        time = timezone(line[2]).localize(time)
        data.append([time, chr(int(line[8]))])

data.sort()
flag = ''
for line in data:
    flag = flag + line[1]

print(flag)