#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_sec 
#-------------------------------------------------------------

./openmp 200 2000  >> timeOpT1.txt
./openmp 800 1000  >> timeOpT2.txt
./openmp 1500 500 >> timeOpT3.txt
./openmp 200 2000  >> timeOpT4.txt
./openmp 800 1000  >> timeOpT5.txt
./openmp 1500 500 >> timeOpT6.txt
./openmp 200 2000  >> timeOpT7.txt
./openmp 800 1000  >> timeOpT8.txt
./openmp 1500 500 >> timeOpT9.txt
./openmp 200 2000  >> timeOpT10.txt


# gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"