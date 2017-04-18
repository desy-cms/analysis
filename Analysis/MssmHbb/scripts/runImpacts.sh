#!/bin/sh
directory=../datacards/201702/01/binned/CHAYANIT_FIX_TURNON/
declare -a points=(300 350 400 500 600 700 900 1100 1300)
for i in "${points[@]}"; do
	./Impacts.sh $i ${directory} &
done
