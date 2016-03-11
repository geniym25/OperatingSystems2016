#!/bin/bash

echo "Удаляем все файлы и папки, кроме скриптов..."
dir2="Files with first letter 'b'"
dir3="Files which have first letter 'c'"
rmdirs ()
{
	if [ -d "$1" ]; then
		cd "$1"
		rm *
		cd ..
		rmdir "$1"	
	fi	
}
rmdirs "$dir2"
rmdirs "$dir3"
find . -type f -not -name '*.sh' -not -name 'README.md' | xargs rm
echo "Готово!"
