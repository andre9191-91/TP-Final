#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_sec.bash
#-------------------------------------------------------------

./sec 200 2000  >> timeGlobalSec.txt
./sec 800 1000  >> timeGlobalSec.txt
./sec 1500 500 >> timeGlobalSec.txt
./sec 200 2000  >> timeGlobalSec.txt
./sec 800 1000  >> timeGlobalSec.txt
./sec 1500 500 >> timeGlobalSec.txt
./sec 200 2000  >> timeGlobalSec.txt
./sec 800 1000  >> timeGlobalSec.txt
./sec 1500 500 >> timeGlobalSec.txt
./sec 200 2000  >> timeGlobalSec.txt


gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"