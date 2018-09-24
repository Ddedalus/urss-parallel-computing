#!/bin/bash
rootJava="/home/dcs/csscj/APSP/BSP/java"
pbs_dir=$rootJava/output/pbs
if [ ! -e $pbs_dir ]; then
    mkdir $pbs_dir
fi

whereAmI=$(dirname "$0")
for c in "$@"
do
   echo $c
   sed -e "s/\^/${c}/g" $whereAmI/basic_akka.pbs > $pbs_dir/${c}_akka.pbs
done
