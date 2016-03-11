#!/bin/bash

read pattern

ls | grep $pattern > file_names.txt

echo "Filenames were added to file_names.txt" 
