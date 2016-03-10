#!/bin/bash

for i in {1..100}
	do
		file=$(cat /dev/urandom | tr -dc 'a-z' | fold -w $((RANDOM%5+1)) | head -n 1)
		touch $file
	done
echo "Создано 100 файлов со случайными латинскими символами длиной от 1 до 5"
