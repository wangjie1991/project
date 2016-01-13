#!/bin/bash


fin=$1
while read LINE
do
#./WordSplit lexicon.ws gbk/$LINE seg/$LINE.seg
#ngram -order 3 -lm ../lm/lm.arpa.gbk.picc -ppl seg/$LINE.seg -debug 2 > ppl/$LINE.ppl
ngram -order 3 -lm ../../lm/lm.arpa.gbk.picc -ppl gbk/$LINE -debug 2 > ppl/$LINE.ppl
done < $fin

