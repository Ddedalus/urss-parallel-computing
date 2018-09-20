#!/bin/bash

pbs_dir=../../../output/pbs
if [ ! -e $pbs_dir ]; then
    mkdir $pbs_dir
fi

whereAmI=$(dirname "$0")
for c in "$@"
do
   exec sed -e "s/\^/${c}/g" $whereAmI/basic_akka.pbs > $pbs_dir/${c}_akka.pbs
done