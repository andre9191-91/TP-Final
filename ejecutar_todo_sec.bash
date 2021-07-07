#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_sec 
#-------------------------------------------------------------

./sec 200 2000  >> timesecT1.txt
./sec 800 1000  >> timesecT2.txt
./sec 1500 500 >> timesecT3.txt
./sec 200 2000  >> timesecT4.txt
./sec 800 1000  >> timesecT5.txt
./sec 1500 500 >> timesecT6.txt
./sec 200 2000  >> timesecT7.txt
./sec 800 1000  >> timesecT8.txt
./sec 1500 500 >> timesecT9.txt
./sec 200 2000  >> timesecT10.txt


gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"