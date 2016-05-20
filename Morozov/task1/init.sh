#!/bin/bash
mkdir "victim files"
rm vi*/*
for ((i=1; i <= 20; i++))
do
    touch "victim files/a$i.victim"
    touch "victim files/b$i.victim"
    touch "victim files/c$i.victim"
    touch "victim files/d$i.victim"
    touch "victim files/e$i.victim"
    touch "victim files/f$i.victim"
done

