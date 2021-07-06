#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

/*
Trabajo Práctico Final - OpenMP 
Maldonado Andrea 
Moran Marcos 

Se compila: gcc -fopenmp -o openmp OpenMp.c
Se ejecuta: ./openmp tamaño_matriz semanas

Los colores de las plantas son:
	o Blanco: Árbol podado
	o Azul: Enfermo con tratamiento antifúngico
	o Rojo: Enfermo con síntomas visibles
	o Naranja: Infectado con esporas (Enfermo sin síntomas visibles)
	o Verde: Árbol sano
*/

// Defino el tipo de celda
typedef struct planta{
	int estado; 	/* Blanco = 0;  Azul = 1; Rojo = 2; Naranja = 3; Verde = 4;		*/ 
	int edad; 		/* Joven = 0 , Adulto = 1, Viejo = 2 							*/
	int herida;		/* Si = 1, No = 0 												*/
	int semana; 	/* Semana en la que se enfermo									*/
	} planta;

// Defino la matriz y una auxiliar para calcular las actualizaciones	
planta **matriz, **aux;

// Defino el tamaño de la matriz y la cantidad de semanas
int tam , semanas ;

void inicializar_matriz( ){
	
	int s, fila, columna, random;
	
		
	#pragma omp parallel for collapse(2) num_threads(4) private(columna)	
	for( fila = 0; fila < tam; fila++ ) {
			
		for( columna = 0; columna < tam; columna++ ) {
		    
            matriz[fila][columna].semana = 0;
			matriz[fila][columna].herida = 0;
             
            // Calculamos la densidad poblacional a un %50 de arboles sanos
			random = rand() % 100;
			if( random <= 50 ){
				matriz[fila][columna].estado = 4; // Arbol Sano - Color Verde
				}
			if( random > 50 && random <= 55 ){   
			    matriz[fila][columna].estado = 3; // Arbol sin sintomas - Color Naranja
				} 
			
            if( random > 55 && random <= 57 ){
                matriz[fila][columna].estado = 2; // Arbol con sintomas - Color Rojo
                }
			if( random > 57 ){
                // Distibuyo sobre los restantes
                random = rand() % 100;
                if( random <= 50 ){
                    matriz[fila][columna].estado = 0; // Arbol Podado - Color Blanco
				    }
                else{
                    matriz[fila][columna].estado = 1; // Arbol con Tratamiento - Color Azul
                    }
                } 
                
			// Calculamos la Edad de la planta
			random = rand() % 100;
			
			if( random <= 30 ){ 
				matriz[fila][columna].edad = 0; // Joven
				}
			if( random > 30 && random <= 84 ){
				matriz[fila][columna].edad = 1; // Adulto
	            }
			if( random > 84 ){
                matriz[fila][columna].edad = 2; // Viejo
				}
			
			// Calculamos las heridas
			if( matriz[fila][columna].edad = 0 ){
                random = rand() % 100;
			
			    if( random <= 15 ){ 
				    matriz[fila][columna].herida = 1; // Joven
				    }
                }
            if( matriz[fila][columna].edad = 1 ){
                random = rand() % 100;
			
			    if( random <= 5 ){ 
				    matriz[fila][columna].herida = 1; // Joven
				    }
                }
			if( matriz[fila][columna].edad = 2 ){
                random = rand() % 100;
			
			    if( random <= 30 ){ 
				    matriz[fila][columna].herida = 1; // Joven
				    }
                }
			
			}
		}
		
	}

// Imprimimos los contadores
void imprimir_matriz( ){
	
	int fila, columna;
	
	for( fila = 0; fila < tam; fila++ ){
		for( columna = 0; columna < tam; columna++ ){
			printf( " %d ",matriz[fila][columna].estado);
			}
		printf( "\n");
		}
	
	}	

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
	
	return( probabilidad );
	
	}	

int resistencia_hongo( int edad, int fila, int columna ){
    
    int random;
    
    int hongo = 1; // 0 significa que desaparecio, 1 aun esta
    
    random = rand() % 100;
    
    switch( matriz[fila][columna].edad ){
        // Joven
        case 0:{
                if( random > 1 ){
                    hongo = 0;
                    }
                }
        // Adulto
        case 1:{
                if( random > 10 ){
                    hongo = 0;
                    }
                }
        // Anciano
        case 2:{
                if( random > 45 ){
                    hongo = 0;
                    }
                }
    
        }
    
    return hongo;
    
    }

// Actualizamos una parcela de la matriz
void actualizar_matriz( int fila , int columna, int semana_actual ){
     
     // Usamos un vector para contar la canitdad de vecinos
     int colores[]={0,0,0,0,0}; // Blanco , Azul, Rojo , Naranja, Verde
     
     int cantidad_vecinos;
     
     int limite_inferior_fila, limite_inferior_columna;
     int limite_superior_fila, limite_superior_columna;
     int limite_auxiliar;
     
     float suceptibilidad_parcela = 0.0, probabilidad_de_contagio = 0.0;
     
     int proximo_estado = matriz[fila][columna].estado;
     
     int random;
     
     // Calculamos la cantidad de vecinos
     
     // Tenemos 8 Vecinos, por lo tanto no estoy ni en los esquineros ni en los bordes
     if( fila !=0 && fila != tam-1 && columna != 0 && columna != tam-1 ){ 
     	cantidad_vecinos = 8;
         }
     else{
        // Tenemos 3 Vecinos, por lo tanto estoy en algun esquinero
        if( ( fila == 0 && columna == 0 ) || ( fila == 0 && columna == tam-1 ) || ( fila == tam-1 && columna == 0 ) || ( fila == tam-1 && columna == tam-1 ) ){ 
            cantidad_vecinos = 3;
            }
        // Tenemos 5 Vecinos, por lo tanto no estoy ni en los esquineros ni en los bordes
        else{ 
            cantidad_vecinos = 5;
            }
        }
     
     // Calculamos los limites para recorrer los vecinos
     switch( cantidad_vecinos ){
                  
        case 3:{
                if( fila == 0 && columna ==0 ){
                    limite_superior_fila    = fila + 2; 
                    limite_superior_columna = columna + 2;
                    limite_inferior_fila    = fila; 
                    limite_inferior_columna = columna;
                    }
                if( fila ==0 && columna == tam-1 ){
                    limite_superior_fila    = fila + 2; 
                    limite_superior_columna = columna + 1;
                    limite_inferior_fila    = fila; 
                    limite_inferior_columna = columna - 1; 
                    }
                if( fila == tam-1 && columna == 0 ){
                    limite_superior_fila     = fila + 1;
                    limite_superior_columna  = columna + 2;
                    limite_inferior_fila     = fila - 1;
                    limite_inferior_columna  = columna;
                    }
                if( fila == tam-1 && columna == tam-1 ){
                    limite_superior_fila      = fila + 1;
                    limite_superior_columna   = columna + 1;
                    limite_inferior_fila      = fila - 1;
                    limite_inferior_columna   = columna - 1;
                    }
                limite_auxiliar = limite_inferior_columna;
                break;
                }
        case 5:{ 
                 if( columna == 0 ){
                     limite_superior_fila     = fila + 2; 
                     limite_superior_columna  = columna + 2;
                     limite_inferior_fila     = fila - 1; 
                     limite_inferior_columna  = columna; 
                     }
                 if( fila == 0 ){
                     limite_superior_fila     = fila + 2; 
                     limite_superior_columna  = columna + 2;
                     limite_inferior_fila     = fila; 
                     limite_inferior_columna  = columna - 1; 
                     }
                 if( columna == tam-1 ){
                     limite_superior_fila     = fila + 2; 
                     limite_superior_columna  = columna + 1;
                     limite_inferior_fila     = fila - 1; 
                     limite_inferior_columna  = columna - 1; 
                     }
                 if( fila == tam-1 ){
                     limite_superior_fila     = fila + 1;
                     limite_superior_columna  = columna + 2;
                     limite_inferior_fila     = fila - 1; 
                     limite_inferior_columna  = columna - 1; 
                     }
               limite_auxiliar = limite_inferior_columna;      
               break;  
               }
        case 8:{ 
                limite_superior_fila    = fila + 2;
                limite_superior_columna = columna + 2; 
                limite_inferior_fila    = fila - 1;
                limite_inferior_columna = columna - 1;
                limite_auxiliar         = limite_inferior_columna;
                break;
                } 
        }
     
     // Recorro los vecinos para contar los colores
	 for( ; limite_inferior_fila < limite_superior_fila; limite_inferior_fila++ ){
          for( limite_inferior_columna = limite_auxiliar; limite_inferior_columna < limite_superior_columna; limite_inferior_columna++ ){
               if( ! ( limite_inferior_fila == fila && limite_inferior_columna == columna ) ){
                   switch( matriz[limite_inferior_fila][limite_inferior_columna].estado ){
                          case 0:{
                                  colores[0] += 1; 
                                  break;
                                  }
                          case 1:{
                                  colores[1] += 1; 
                                  break;
                                  }
                          case 2:{
                                  colores[2] += 1; 
                                  break;
                                  }
                          case 3:{
                                  colores[3] += 1;
                                  break;
                                  }
                          case 4:{
                                  colores[4] += 1;
                                  break;
                                  }  
                          }
                   }
               }
          }

     
     switch( matriz[fila][columna].estado ){
		
		// Arbol Podado - Color Blanco
		case 0:{
				if( semana_actual - matriz[fila][columna].semana == 7 ){
					proximo_estado = 4; // Arbol Sano - Color Verde
					}
				else{
					proximo_estado = 0; // Podado - Color Blanco
					}
				break;
				}
		
		// Enfermo con tratamiento - Color Azul		
		case 1:{
                
                int existe_hongo = resistencia_hongo( matriz[fila][columna].edad , fila , columna );
                
                if( semana_actual - matriz[fila][columna].semana == 8 ){
                    
                    if( existe_hongo == 0 ){
                        proximo_estado = 4;
                        }
                    else{
                         if( matriz[fila][columna].edad == 0 || matriz[fila][columna].edad == 1 ){
                             proximo_estado = 0; // Pasa a Podado
                             }
                         else{
                              proximo_estado = 4; // Es reemplazado por un arbol sano y joven
                              matriz[fila][columna].edad = 0;
                              }
                         }
                    
                    }
                break;
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
				if( semana_actual - matriz[fila][columna].semana == 6 ){
                    
                    proximo_estado = 2; // Pasa a tener sintomas - Color Rojo
					matriz[fila][columna].semana = semana_actual;
					}
				else{
					proximo_estado = 3; // Sigue sin sintomas - Color Naranja
					}
				break;
				}
		// Sano	- Color Verde	
		case 4:{ 
				suceptibilidad_parcela = suceptibilidad( matriz[fila][columna].edad, matriz[fila][columna].herida );
                			
				probabilidad_de_contagio = probabilidad_contagio( colores[2] / cantidad_vecinos , suceptibilidad_parcela );
			    
				random = rand() % 100;
				
				if( ( random <= probabilidad_de_contagio * 100 ) && ( colores[2] > 0 || colores[3] > 0 ) ){
					proximo_estado = 3; // Pasa ha estar enfermo sin sintomas - Color Naranja
					matriz[fila][columna].semana = semana_actual + 1;
					}
				else{
					proximo_estado = 4; // Sigue sano - Color Verde
					}
				break;
				}
		}
	 
     matriz[fila][columna].estado = proximo_estado;
     	
     }
     
	
int main( int argc, char *argv[] ){	
	
	int s, fila, columna, proximo_estado;
	clock_t tiempo_inicial , tiempo_final;
	long int duracion; 

	// Obtenemos el tamaño de la matriz que viene por parametro
	tam = atoi(argv[1]);
	
	// Obtenemos la cantidad de semanas que vienen por parametro
	semanas = atoi(argv[2]);
	
	printf( "Comienza el programa en OpenMp\n");
	
	
	// Pedimos memoria para toda una fila de la matriz y para la matriz que usaremos para calcular el proximo estado
	matriz = (planta**) malloc (tam*(sizeof(planta*)));
	//aux = (planta**) malloc (tam*(sizeof(planta*)));
	
	// Terminamos de pedir el resto de la memoria ( para las columnas ) de manera que obtengo la matriz completa
	for( columna = 0; columna < tam; columna++ ){
		matriz[columna] = (planta*) malloc (tam*(sizeof(planta)));
		//aux[columna] = (planta*) malloc (tam*(sizeof(planta)));  
		}	
	
	// Inicializamos la matriz
	inicializar_matriz( );
	tiempo_inicial= clock();

	// Imprimimos la matriz
	//imprimir_matriz( );
	
	
	for( s = 1; s <= semanas; s++ ){
         
         // Imprimimos la matriz
         //printf("Semana %d \n", s);
	     //imprimir_matriz( );
         
		 #pragma omp parallel for collapse(2) num_threads(4) private(columna)
         for( fila = 0; fila < tam; fila++ ){
              
              for( columna = 0; columna < tam; columna++ ){
                   
                   actualizar_matriz( fila , columna , s );           
                       
                   }
              }
         
         }
    
	tiempo_final= clock();

	free( matriz );
	
	duracion= tiempo_final - tiempo_inicial; 
	duracion/= CLOCKS_PER_SEC; 
	printf("El tiempo %d es de: %ld segundos \n", tam, duracion);
	
	
	
	system("pause");
	return 0;
}
