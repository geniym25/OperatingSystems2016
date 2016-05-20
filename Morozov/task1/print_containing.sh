#!/bin/bash
cd vi*
read str
echo `ls -a | grep -E $str` >> ../search_result.txt
