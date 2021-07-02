/*Practico de Máquina - Secuencial */
/*
Para compilar se usa 
	gcc -o o nombre_ejecutable -fopenmp nombre_archivo.c
Para ejecutar:
	./nombre_ejecutable tamaño_matriz cnt_dias_x_iteracion cnt_semanas
*/
#include <omp.h>
#include <stdio.h> 
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
Los colores de las plantas son:
	o Blanco: Árbol podado
	o Azul: Enfermo con tratamiento antifúngico
	o Rojo: Enfermo con síntomas visibles
	o Naranja: Infectado con esporas (Enfermo sin síntomas visibles)
	o Verde: Árbol sano
*/

typedef struct planta{
	int estado; 	/* Blanco = 0;  Azul = 1; Rojo = 2; Naranja = 3; Verde = 4;		*/ 
	int edad; 		/* Joven = 0 , Adulto = 1, Viejo = 2 							*/
	int heridas;	/* Si = 1, No = 0 												*/
	int x; 			/* fila 														*/
	int y; 			/* columna 														*/
	int semana; 	/* Semana en la que se enfermo									*/
	} planta;

planta **matriz, **aux;


// Calculamos la Suspetibilidad de la planta
float suceptibilidad( int edad, int heridas ){
	
	float suceptible = 0;
	
	switch( edad ){
		/* Joven */
		case 0: {
				suceptible = suceptible + 0.3;
				break;
				}
		/* Adulto */
		case 1: {
				suceptible = suceptible + 0.2;
				break;
				}
		/* Viejo */
		case 2: {
				suceptible = suceptible + 0.5;
				break;	
				}
		}
	if( heridas == 1 ){
		suceptible = suceptible + 0.15;
		}
		
	return suceptible;

	}
	
// Calculamos la probabilidad de contagio	
float probabilidad_contagio( int porcentaje_rojos, float suceptible ){
	
	float probabilidad = 0;

	probabilidad = ((( porcentaje_rojos + suceptible ) * 0.60 ) + 0.05 );
	
	return(probabilidad);
	
	}

// Imprimimos los contadores
void imprimir_datos( int plantas, int heridas, int sin_sintomas, int con_sintomas, int con_tratamiento, int podadas, int sanas ){
	
	printf( "Datos de la matriz\n");
	printf( "      - Cantidad de plantas: %d\n" , plantas );
	printf( "      - Cantidad de plantas heridas: %d\n", heridas );
	printf( "      - Cantidad de plantas sin contagio: %d\n", sin_sintomas );
	printf( "      - Cantidad de plantas con sintomas: %d\n", con_sintomas );
	printf( "      - Cantidad de plantas con tratamientos: %d\n", con_tratamiento );
	printf( "      - Cantidad de plantas podadas: %d\n", podadas );
	printf( "      - Cantidad de plantas sanas: %d\n", sanas );
	
}

// Actualizamos la celda
planta actualizar_parcela( planta celda, int vecinos[], int semana_actual ){
	
	int blanco = 0, azul = 0, rojo = 0, naranja = 0, verde = 0;
	
	float suceptibilidad_parcela = 0, probabilidad_de_contagio = 0;
	
	int i=0, proximo_estado = 0;
	
	float random;
	
	planta celda_aux;

	// Guardo los datos de la celda a ser analizada
	celda_aux.estado = celda.estado;
	celda_aux.edad   = celda.edad;
	celda_aux.heridas = celda.heridas;
	celda_aux.semana = celda.semana;
	
	// Recorro los vecinos para saber cuantos tiene de cada tipo
	for( i = 0; i < 8; ++i ){
		
		switch( vecinos[i] ){
			case 0:{
					blanco++;
					break;
					}
			case 1:{
					azul++;
					break;
					}
			case 2:{
					rojo++;
					break;	
					}
			case 3:{
					naranja++;
					break;
					}
			case 4:{
					verde++;
					break;
					}
			}
		}

	// Evaluo el estado actual de la celda y cual sera su proximo estado
	switch (celda.estado){
		
		// Arbol Podado - Color Blanco
		case 0:{
				if( semana_actual - celda.semana == 7 ){
					proximo_estado = 4; // Arbol Sano - Color Verde
					}
				else{
					proximo_estado = 0; // Podado - Color Blanco
					}
				break;
				}
		
		// Enfermo con tratamiento - Color Azul		
		case 1:{
				if( semana_actual - celda.semana == 8 ){
				
					random = rand() % (100);
				
					switch (celda.edad){
						// Joven
						case 0:{							
								if( random <= 1 ){
									proximo_estado = 0; // Podado - Color Blanco
									}
								else{
									proximo_estado = 4; // Recuperado - Color Verde
									}
								break;
								}
						// Adulto
						case 1:{
								if( random <= 10 ){
									proximo_estado = 0; // Podado - Color Blanco
									}
								else{
									proximo_estado = 4; // Recuperado - Color Verde
									}
								break;
								}
						// Viejo
						case 2:{
								if( random <= 10 ){
									proximo_estado = 4; // Reemplazado - Color Verde
									celda.edad = 0;
									}
								else{
									proximo_estado = 4; // Recuperado - Color Verde
									}
								break;
								}
						}
					}
		
		// Enfermo con sintomas - Color Rojo
		case 2:{
				random = rand() % 100;
				if( random <= 90 ){
					proximo_estado = 1;	// Pasa a tratamiento - Color Azul
					}
				else{
					proximo_estado = 2; // Sigue con sintomas - Color Naranja
					}
				break;
				}
		 
		 // Enfermo sin sintomas - Color Naranja
		case 3:{
				if( semana_actual - celda.semana == 6 ){
					proximo_estado = 2; // Pasa a tener sintomas - Color Rojo
					celda_aux.semana = semana_actual + 1;
					}
				else{
					proximo_estado = 3; // Sigue sin sintomas - Color Naranja
					}
				break;
				}
		// Sano	- Color Verde	
		case 4:{ 
				suceptibilidad_parcela = suceptibilidad( celda.edad, celda.heridas );			
				probabilidad_de_contagio = probabilidad_contagio( rojo / 8 , suceptibilidad_parcela ) + 0.60 + 0.05;
			
				random = rand() % (100);
				if(( random <= (probabilidad_de_contagio * 100 ))&& (rojo > 0)){
					proximo_estado = 3; // Pasa ha estar enfermo sin sintomas - Color Naranja
					celda_aux.semana = semana_actual;
					}
				else{
					proximo_estado = 4; // Sigue sano - Color Verde
					}
				break;
				}
		}
	
	celda_aux.estado = proximo_estado;
	
	return( celda_aux );

}
}

int main( int argc, char *argv[] ){	
	
	int semanas = 0;
	
	int s, i, j, fila, columna, size, vecinos[8];
	
	double tiempoInicial = 0, tiempoFinal = 0, speedUp = 0;
	
	int plantas = 0, heridas = 0,  sin_sintomas = 0, con_sintomas = 0, con_tratamiento = 0, sanas = 0, podadas = 0;
	
	// Obtenemos el tamaño de la matriz que viene por parametro
	size = atoi(argv[1]);
	
	// Obtenemos la cantidad de semanas que vienen por parametro
	semanas = atoi(argv[2]);
	
	//¨Pedimos memoria para toda una fila de la matriz y para la matriz que usaremos para calcular el proximo estado
	matriz = (planta **) malloc((size)*sizeof(planta*));
	aux = (planta **) malloc((size)*sizeof(planta*));
		
	// Terminamos de pedir el resto de la memoria de manera que obtengo la matriz completa
	for( i = 0; i < size; i++ ){
		matriz[i] = (planta *) malloc(size*sizeof(planta));
		aux[i] = (planta *) malloc(size*sizeof(planta));
		}	
	
	for( s = 0; s < semanas; s++ ){
		
		// Recorremos la matriz para determinar con probabilidades las caracteristicas de cada parcela
		for( i = 1; i < size; i++ ) {
			
			for( j = 1; j < size; j++ ) {
			
				int random;
				
				// Calculamos la Edad de la planta
				random = rand() % 100;
				// Jovenes
				if( random < 30 ){ 
					matriz[i][j].edad = 0;
					// Calculamos si tiene una herida
					if( rand() % 100 < 15 && matriz[i][j].heridas == 0 ){ 
						matriz[i][j].heridas = 1;
						heridas++;
						}
					if( rand() % 100 > 15 && matriz[i][j].heridas == 1 ){
						matriz[i][j].heridas = 0;
						heridas--;
						}
					}
				else{ 
					// Adultos
					if( random < 54 ){
						matriz[i][j].edad = 1;
						// Calculamos si tiene una herida
						if( rand() % 100 < 5 && matriz[i][j].heridas == 0 ){ 
							matriz[i][j].heridas = 1;
							heridas++;
							}
						if( rand() % 100 > 5 && matriz[i][j].heridas == 1 ){ 
							matriz[i][j].heridas = 0;
							heridas++;
							}
						}
					// Viejos	
					else{ 
						matriz[i][j].edad = 2;
						// Calculamos si tiene una herida
						if( rand() % 100 < 30 && matriz[i][j].heridas == 0 ){ 
							matriz[i][j].heridas = 1;
							heridas++;
							}
						if( rand() % 100 > 30 && matriz[i][j].heridas == 1 ){ 
							matriz[i][j].heridas = 0;
							heridas--;
							}
						}
					}
				
				// Calculamos la densidad poblacional a un %50
				random = rand() % 100;
				if( random < 50 ){ 
					
					// El %0.02 de las plantas estan infectadas
					random = rand() % 1000;
					if( random < 2 ){
						matriz[i][j].semana = s;
						matriz[i][j].estado = 2; // Con sintomas - Color Rojo
						con_sintomas++;
						}
					// Arboles enfermos sin sintomas
					random = rand() % 1000;
					if( random < 5 ){   
						matriz[i][j].estado = 2; 
						} 
					} 
				// El % 50 son plantas sanas
				else{
					matriz[i][j].estado = 4;
					}
				}
			}
		
		printf("Matriz Inicial semana %d\n",s);
		
		imprimir_datos( plantas, heridas, sin_sintomas, con_sintomas, con_tratamiento, podadas, sanas );
		
		//tiempoInicial = omp_get_wtime();

		
		// Inicializamos los contadores
		plantas = 0; 
		heridas = 0;  
		sin_sintomas = 0; 
		con_sintomas = 0; 
		con_tratamiento = 0; 
		sanas = 0; 
		podadas = 0;

		// Recorremos la matriz para ir procesando los cambios de estados
		for( fila = 0; fila < size; fila++ ){
			
			for( columna = 0; columna < size; columna++ ){
				
				// Colocamos todos los vecinos de la celda a ser analizada 
				vecinos[0] = matriz[fila-1][columna-1].estado;
				vecinos[1] = matriz[fila-1][columna].estado;
				vecinos[2] = matriz[fila-1][columna+1].estado;
				vecinos[3] = matriz[fila][columna-1].estado;
				vecinos[4] = matriz[fila][columna+1].estado;
				vecinos[5] = matriz[fila+1][columna-1].estado;
				vecinos[6] = matriz[fila+1][columna].estado;
				vecinos[7] = matriz[fila+1][columna+1].estado;
				
				// Llamamos al actulizar y dejamos la matriz actualizada en un matriz de respaldo
				aux[fila][columna] = actualizar_parcela( matriz[fila][columna] , vecinos , s );
				
				heridas = 0,  sin_sintomas = 0, con_sintomas = 0, con_tratamiento = 0, sanas = 0, podadas = 0;
				
				switch( aux[fila][columna].estado ){
					case 0:{ 
							podadas++;
							break;
							}
					case 1:{
							con_tratamiento++;
							break;
							}
					case 2:{
							con_sintomas++;
							break;	
							}
					case 3:{
							sin_sintomas++;
							break;
							}
					case 4:{
							sanas++;
							break;
							}
					}
				}
				
			}
			
		
		// Actualizamos las matrices
		matriz = aux;
		
		//tiempoFinal = tiempoFinal + ( omp_get_wtime() - tiempoInicial );
		
		printf( "Final de la semana numero: %d\n" , s );
		
		imprimir_datos( plantas, heridas, sin_sintomas, con_sintomas, con_tratamiento, podadas, sanas );
		
		// Liberamos el espacio de memoria solicitado para las matrices
		
	}

	
	free( matriz );
	free( aux );
	
	printf( "El Tiempo Promedio de %d ejecuciones con tamanio de %d es de: %f \n", semanas , size , tiempoFinal / semanas );
	
	return 0;
}

