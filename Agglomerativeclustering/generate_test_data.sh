#!/bin/bash

SIZE=100

for i in `seq 0 1 $SIZE`
do	
	
	echo "$i $(((RANDOM %1000) + 1))  $(((RANDOM %1000) + 1))  "
	
done

