#!/bin/bash

touch help_list.txt
mkdir move_c_here

ls | grep "^c" | cut -f1 > help_list.txt

for i in `cat help_list.txt`
do
mv "$i" move_c_here/"new_$i"
done

rm help_list.txt
