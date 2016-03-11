#!/bin/bash

echo "Введите строку для поиска:"
read string
echo "Файлы с заданной строкой:"
echo `ls | grep -E "$string"`
