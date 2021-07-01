/*Practico de Máquina - Secuencial */
/*
Para compilar se usa 
	gcc -o o nombre_ejecutable -fopenmp nombre_archivo.c
Para ejecutar:
	./nombre_ejecutable tamaño_matriz cnt_dias_x_iteracion cnt_iteraciones
*/
#include <omp.h>
#include <stdio.h> 
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define PASOSXDIA 1
#define ITERACIONES 10

/*Blanco: Árbol podado,
o Azul: Enfermo con tratamiento antifúngico,
o Rojo: Enfermo con síntomas visibles,
o Naranja: Infectado con esporas (Enfermo sin síntomas visibles)
o Verde: Árbol sano.*/

typedef struct individuo
{
	int estado; /*Borde = -1; Blanco = 0;  Azul = 1;Rojo = 2; Naranja = 3;Verde=5;*/ 
	int edad; /*Edad joven=0 , adulto=1, viejo=2 */
	int heridas;
	int x; /*fila*/
	int y; /*columna*/
	int dia; /* El dia que contrajo la enfermedad*/

}individuo;



float probabilidad_contagio(int porcentaje_rojos, float suceptible){
	float probabilidad=0;
	//Prob. contagio= (%vecinos-con-síntomas + susceptibilidad (edad, heridas))* 0.60 + 0.05
	probabilidad= (((porcentaje_rojos+suceptible)*0.60)+0.05);
	return(probabilidad);
}

float  suceptibilidad(int edad, int heridas){
	float suceptible=0;
	
	switch (edad){
		/*joven*/
		case  0: 
			suceptible=suceptible+0.3;
			break;
		/*Adulto*/
		case  2:
			suceptible=suceptible+0.2;
			break;
		/*viejo*/
		case  3:
			suceptible=suceptible+0.5;
			break;	
	}
	if (heridas==1){
		suceptible=suceptible+0.15;
	}
	return(suceptible);
}

individuo  actualizar_individuo(individuo celda, int vecinos[], int dia_actual){
	int blanco=0, azul=0, rojo=0, naranja=0, verde=0,  borde=0;
	int i=0, proximo_estado=0;
	float suceptibilidad_ind=0, probabilidad_con=0, random, probabilidad_morir;
	individuo celda_aux;

	celda_aux.edad=celda.edad;
	celda_aux.heridas=celda.heridas;
	celda_aux.dia=celda.dia;
	/*Recorro los vecinos para saber cuantos tiene de cada tipo*/
	for (i = 0; i < 8; ++i)
	{
		switch (vecinos[i]){
			/*analisar si necesito todos los case*/
			case -1:
				borde++;
				break;
			case  0:
				blanco++;
				break;
			case  1:
				azul++;
				break;
			case  2:
				naranja++;
				break;
			case  3:
				rojo++;
				break;	
			case  4:
				verde++;
				break;
		}
	}

	/*Evaluo el valor actual del vecino y cual sera su proximo valor*/
	switch (celda.estado){
		case  1: 
		/*Árbol Sano*/
			suceptibilidad_ind= suceptibilidad(celda.edad, celda.heridas);			
			probabilidad_con= probabilidad_contagio(rojo/(8-borde), suceptibilidad_ind);
			random=rand() % (100);
			if (random <= probabilidad_con*100 && rojo>0){
				/*Cambio a Enfermo sin síntomas:*/
				proximo_estado= 2;
				/*Registro el dia en que se contagio*/
				celda_aux.dia=dia_actual;
			}else{
				/*Sigue como esta*/
				proximo_estado= 1;
			}
			break;
		case  2:
		/*Enfermo sin síntomas:*/
			if (dia_actual-celda.dia==6){
				proximo_estado=3;
				celda_aux.dia=dia_actual+1;
			}else{
				proximo_estado=2;
			}
			break;
		case  3:
		/*Enfermo con síntomas:*/

			if (dia_actual-celda.dia>=6 && dia_actual-celda.dia != 8){
				//No pasaron 8 semanas desde que está enfermo y analizo si se trata
				random=rand() % 100;
				if (random<=90){
					//se trata 
					proximo_estado=4;	
				}else{
					//sigue propagando enfermedad
					proximo_estado=3;
				}
			}else{
				if (dia_actual-celda.dia == 8){
					//Pasaron 8 semanas desde el contagio
					switch (celda.edad){
						case  0:
						/*joven -> podado*/
							probabilidad_resistencia=1;		
							break;
						case  1:
						/*Adulto -> podado*/
							probabilidad_resistencia=10;
							break;
						case  2:
						/*viejo -> reemplazado*/
							probabilidad_resistencia=45;
							break;
					}
					random = rand() % (100);
					if (random<=probabilidad_resistencia){
						/*recuperado*/
						proximo_estado=6;
					}else{
						/*podado*/
						proximo_estado=5;
					}

				}else{
					proximo_estado=3;
				}
			}
			break;	
		case  4:
		/*Enfermo con tratamiento antifúngico*/
			if (dia_actual-celda.dia == 8){

				switch (celda.edad){
					case  0:
					/*joven*/
						probabilidad_resistencia=1;		
						break;
					case  1:
					/*Adulto*/
						probabilidad_resistencia=10;
						break;
					case  2:
					/*viejo*/
						probabilidad_resistencia=45;
						break;
				}
				random = rand() % (100);
				if (random<=probabilidad_resistencia){
					if(celda.edad == 0 || celda.edad ==1)
					/*Muere*/
					proximo_estado=6;
				}else{
					/*Recuperado*/
					proximo_estado=5;
				}

			}else{
				proximo_estado=4;
			}
		
			break;
		case  5:
		/*Recuperado*/
			proximo_estado=5;
			break;
		case  6:
		/*podado*/
			proximo_estado=6;
			break;
		case  7:
		/*reemplazdo*/
			proximo_estado=7;
	}
	celda_aux.estado=proximo_estado;
	return(celda_aux);
}