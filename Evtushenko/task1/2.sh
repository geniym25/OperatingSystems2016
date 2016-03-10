#!/bin/bash

dirname="Files with first letter 'b'"
echo "Перемещаем все файлы, начинающиеся на 'b', в директорию $dirname"
mkdir -pv "$dirname"
mv -v b* "$dirname"
echo "Готово!"
