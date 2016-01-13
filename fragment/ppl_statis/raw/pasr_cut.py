#-*- coding: utf_8 -*-

import os
import re

path = './gbk'
if not os.path.exists(path):
    os.makedirs(path)

rf = open('picc.txt', 'r')
fnset = set()

for line in rf:
    ll = line.decode('gbk').split('\t')
    
    fn = ll[0]
    fn = fn[fn.rfind('/')+1:fn.rfind('.')]
    fn = '%s/%s' % (path, fn)

    text = ll[1]
    text = text[text.find(':')+1:]
    text = re.sub(r'\(.*?\)', ' ', text)
    text = text + '\n'
    text = text.encode('gbk')

    if fn in fnset:
        with open(fn, 'a') as af:
            af.write(text)
    else:
        fnset.add(fn)
        with open(fn, 'w') as wf:
            wf.write(text)

rf.close()
