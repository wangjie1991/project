#-*- coding: utf_8 -*-

import re


lf = open('file.list', 'r')
wf = open('sel.list', 'w')
fs = ''
fl100 = []
fl200 = []
fl300 = []
fl400 = []
fl500 = []
flo = []
count = 0

for line in lf:
    count = count + 1

    text = ''
    line = line[:-1]
    fn = 'ppl/' + line + '.ppl'
    with open(fn, 'r') as rf:
        text = rf.read()

    ppls = re.findall('ppl= ([0-9\.]*)', text)
    ppl = float(ppls[-1])

    #if 80 < ppl and ppl < 200:
    if ppl <= 100:
        fl100.append(line)
    elif ppl <= 200:
        fl200.append(line)
    elif ppl <= 200:
        fl300.append(line)
    elif ppl <= 200:
        fl400.append(line)
    elif ppl <= 200:
        fl500.append(line)
    else:
        flo.append(line)

fs = fs + 'ppl <= 100:\ncount=' + str(len(fl100)) + ' rate=' + str(len(fl100)/float(count)) + '\n'
for f in fl100:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'

fs = fs + 'ppl <= 200:\ncount=' + str(len(fl200)) + ' rate=' + str(len(fl200)/float(count)) + '\n'
for f in fl200:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'

fs = fs + 'ppl <= 300:\ncount=' + str(len(fl300)) + ' rate=' + str(len(fl300)/float(count)) + '\n'
for f in fl300:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'

fs = fs + 'ppl <= 400:\ncount=' + str(len(fl400)) + ' rate=' + str(len(fl400)/float(count)) + '\n'
for f in fl400:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'

fs = fs + 'ppl <= 500:\ncount=' + str(len(fl500)) + ' rate=' + str(len(fl500)/float(count)) + '\n'
for f in fl500:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'

fs = fs + 'ppl > 500:\ncount=' + str(len(flo)) + ' rate=' + str(len(flo)/float(count)) + '\n'
for f in flo:
    fs = fs + '\t' + f + '\n'
fs = fs + '\n'


wf.write(fs)
wf.close()
rf.close()

