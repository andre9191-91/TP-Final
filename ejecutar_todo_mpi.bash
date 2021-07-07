#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_mpi.bash
#-------------------------------------------------------------

mpirun -np 4 -f ./hostfile ./mpi 200 2000  >> timeGobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 800 1000  >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 1500 500 >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 200 2000  >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 800 1000  >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 1500 500 >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 200 2000  >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 800 1000  >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 1500 500 >> timeGlobalmpi.txt
mpirun -np 4 -f ./hostfile ./mpi 200 2000  >> timeGlobalmpi.txt


# gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"