#!/bin/sh

#-------------------------------------------------------------
#-------- ejecuta secuencial para los diferentes tamaños------
# antes de ejecutar este script.. darle permisos:-------------
#   chmod 777 ejecutar_todo_hibrido.bash
#-------------------------------------------------------------

mpirun -np 4 -f ./hostfile ./hibrido 200 2000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 800 1000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 1500 500 >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 200 2000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 800 1000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 1500 500 >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 200 2000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 800 1000  >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 1500 500 >> timeGobalhibrido.txt
mpirun -np 4 -f ./hostfile ./hibrido 200 2000  >> timeGobalhibrido.txt


# gdialog --msgbox "Listo! se ejecutaron los procesos, la salidas se encuentran en los archivos que correspondan"