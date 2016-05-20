#!/bin/bash
cd vi*
i=0;
for f in b*;
do
    mv $f xyz$i.victim;
    let i=i+1;
done
