#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_openMp.bash  
#-------------------------------------------------------------

./openmp 200 2000  >> timeGlobalOpenMp.txt
./openmp 800 1000  >> timeGlobalOpenMp.txt
./openmp 1500 500 >> timeGlobalOpenMp.txt
./openmp 200 2000  >> timeGlobalOpenMp.txt
./openmp 800 1000  >> timeGlobalOpenMp.txt
./openmp 1500 500 >> timeGlobalOpenMp.txt
./openmp 200 2000  >> timeGlobalOpenMp.txt
./openmp 800 1000  >> timeGlobalOpenMp.txt
./openmp 1500 500 >> timeGlobalOpenMp.txt
./openmp 200 2000  >> timeGlobalOpenMp.txt


# gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"