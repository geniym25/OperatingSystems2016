#!/bin/bash

i=1

while [ $i -lt 35 ]
do
touch abc$i bca$i cab$i
let "i = $i + 1"
done
