#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_mpi
#-------------------------------------------------------------

mpirun -np 4 -f ./hostfile ./mpi 200 2000  >> timempiT1.txt
mpirun -np 4 -f ./hostfile ./mpi 800 1000  >> timempiT2.txt
mpirun -np 4 -f ./hostfile ./mpi 1500 500 >> timempiT3.txt

# gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"