#!/bin/bash
cd vi*
i=0;
for f in c*;
do
    cp $f copy_of_c$i.victim;
    let i=i+1;
done
