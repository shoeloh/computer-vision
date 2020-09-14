#!/bin/bash

for a in 1 2 3
do
/home/shyloh/OpenCV/release/bin/opencv_createsamples -img fingers/pos/fin$a.png -bg fingers/neg.txt -info fingers/crop$a.txt -num 100 -w 30 -h 30 -maxxangle 0.1 -maxyangle 0.1 -maxzangle 0.3 
done
