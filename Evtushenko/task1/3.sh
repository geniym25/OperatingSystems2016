#!/bin/bash

dirname="Files which have first letter 'c'"
echo "Копируем все файлы, начинающиеся на 'c', с новым именем в директорию $dirname..."
mkdir -pv "$dirname"
for i in c* 
	do
		cp -v $i "$dirname/$(cat /dev/urandom | tr -dc 'a-z' | fold -w $((RANDOM%5+1)) | head -n 1)"
	done
echo "Готово!"

