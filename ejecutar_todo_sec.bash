#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_sec 
#-------------------------------------------------------------

./sec 200 1000  >> timesecT1.txt
./sec 200 1000  >> timesecT2.txt
./sec 200 1000 >> timesecT3.txt



gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"