#include <stdio.h>      
#include <stdlib.h> 
#include <time.h>    
#include<mpi.h>

int m=2500;
int n= 1251, nproc,myid; // n = m/2 +1

typedef struct {
  char opinionPolitica;
  float conviccion;
  float creencRojo;
  float creencAzul;
  float creencVerde;
  float creencBlanco;
} celda;

celda **matriz, **aux, *buffer, *resultadoAux,**matrizResultado;
 
void initMatriz();
void imprimirMatriz();
void cambiaPolitica(int f, int c);
void getOpinionVeci(float opinionVec[4],int f,int c,int numV);
void completarMatriz();
void armarResultado();

int main (int argc,char **argv){

int col,fila,tiempo,j;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &nproc);
MPI_Comm_rank(MPI_COMM_WORLD, &myid);

matriz=(celda**) malloc (n*(sizeof(celda*)));
aux=(celda**) malloc (n*(sizeof(celda*)));
for(col=0;col<n;col++){
  matriz[col]=(celda*) malloc (n*(sizeof(celda)));
  aux[col]=(celda*) malloc (n*(sizeof(celda)));  
}

j= (n-1)*(n-1);
buffer=(celda*) malloc (j*(sizeof(celda)));

j=m*m;
resultadoAux=(celda*) malloc (j*(sizeof(celda)));

matrizResultado=(celda**) malloc (m*(sizeof(celda*)));
for(j=0;j<m;j++)
  matrizResultado[j]=(celda*) malloc (m*(sizeof(celda)));

initMatriz();

double Tinicial, Tfinal, time;
if(myid==0)
  Tinicial=MPI_Wtime();

for(tiempo=0;tiempo<1000;tiempo++){
 armarResultado();
 completarMatriz();
 for(fila=0;fila<n;fila++){
  for(col=0;col<n;col++){   
    cambiaPolitica(fila,col);
}   
  }
  matriz=aux;
}

if(myid==0){
  Tfinal=MPI_Wtime();
  time=Tfinal-Tinicial;
  printf("el tiempo es %lf segundos",time);
}
MPI_Finalize();
return EXIT_SUCCESS;
}

void initMatriz(){

int fila,col;
int random, interv,rest, semilla= time(NULL)+myid,i;
float r[4], sum=0.0;
srand48(semilla);
srand(semilla);
 
for(fila=0;fila<n; fila++){
 for(col=0;col<n;col++){
   random=rand() % 4;
   switch(random){
    case 0: matriz[fila][col].opinionPolitica='A';break;
    case 1: matriz[fila][col].opinionPolitica='R';break;
    case 2: matriz[fila][col].opinionPolitica='V';break;
    case 3: matriz[fila][col].opinionPolitica='B';
   }
   matriz[fila][col].conviccion= (1+rand()%100)/100.0;
  
  sum=0.0;
  for(i=0;i<4;i++)
    r[i]=drand48()*95.0;   
  for(i=0;i<4;i++)
    sum+= r[i];
  for(i=0;i<4;i++)
    r[i]=((r[i]*100.0)/sum)/100;

   matriz[fila][col].creencRojo=r[0];
   matriz[fila][col].creencAzul=r[1];
   matriz[fila][col].creencVerde=r[2];
   matriz[fila][col].creencBlanco=r[3];
 }
}  

}

void armarResultado(){
int i,j,f,c, fila, col;

 
MPI_Status s;

i=0;

for(f=0;f<n-1;f++){
  for(c=0;c<n-1;c++){
    buffer[i]=matriz[f][c];
    i++;
    }
  }

MPI_Gather(buffer,((n-1)*(n-1))*(sizeof(celda)),MPI_BYTE,resultadoAux,((n-1)*(n-1))*(sizeof(celda)),MPI_BYTE,0,MPI_COMM_WORLD);

if(myid==0){
 
  i=0;
  for(f=0; f<n-1; f++){
     for(c=0;c<n-1;c++){
       matrizResultado[f][c]=resultadoAux[i];
       i++;
       }
     }

  for(f=0; f<n-1; f++){ 
    for(c=n-1;c<m;c++){
      matrizResultado[f][c]=resultadoAux[i];
      i++;
      }
    }

  for(f=n-1; f<m; f++){ 
    for(c=0;c<n-1;c++){
      matrizResultado[f][c]=resultadoAux[i];
      i++;
      }
    }

  for(f=n-1; f<m; f++){ 
    for(c=n-1;c<m;c++){
      matrizResultado[f][c]=resultadoAux[i];
      i++;
      }
    }
 
}


}

void imprimirMatriz(){

int fila,col;

for(fila=0;fila<n; fila++){
 for(col=0;col<n;col++){ 
   printf(" %c %f \n",matriz[fila][col].opinionPolitica, matriz[fila][col].conviccion);
   printf(" %f %f %f %f\n",matriz[fila][col].creencRojo, matriz[fila][col].creencAzul, matriz[fila][col].creencVerde, matriz[fila][col].creencBlanco);
 }
 printf("\n");
 }
}

void cambiaPolitica(int f, int c){
    
float opinionVec[]={0.0,0.0,0.0,0.0}; // r a v b

if(f!=0 && f!=n-1 && c!=0 && c!=n-1) // 8 vecinos.
  getOpinionVeci(opinionVec, f, c, 8);

else{
  if( (f==0 && c==0) || (f==0 && c==n-1) || (f==n-1 && c==0) || (f==n-1 && c==n-1) ) // esquina, 3 vecinos.
    getOpinionVeci(opinionVec, f, c, 3);
  else // 5 vecinos.
    getOpinionVeci(opinionVec, f, c, 5);
 }

float difRojo, difAzul, difVerde, difBlanco, porCambio, ajusRojo, ajusAzul, ajusVerde, ajusBlanco;

difRojo= opinionVec[0] - matriz[f][c].creencRojo;
difAzul= opinionVec[1] - matriz[f][c].creencAzul;
difVerde= opinionVec[2] - matriz[f][c].creencVerde;
difBlanco= opinionVec[3] - matriz[f][c].creencBlanco;

porCambio= 1- matriz[f][c].conviccion;

ajusRojo= difRojo*porCambio;
ajusAzul= difAzul*porCambio;
ajusVerde= difVerde*porCambio;
ajusBlanco= difBlanco*porCambio;

aux[f][c].creencRojo= matriz[f][c].creencRojo + ajusRojo;
aux[f][c].creencAzul= matriz[f][c].creencAzul + ajusAzul;
aux[f][c].creencVerde= matriz[f][c].creencVerde + ajusVerde;
aux[f][c].creencBlanco= matriz[f][c].creencBlanco + ajusBlanco;

aux[f][c].conviccion= matriz[f][c].conviccion;

float mayor=aux[f][c].creencRojo;
char m='R';

if (mayor < aux[f][c].creencAzul){
  mayor=aux[f][c].creencAzul;
  m='A';
}
if(mayor < aux[f][c].creencVerde){
  mayor=aux[f][c].creencVerde;
  m='V';
}
if (mayor < aux[f][c].creencBlanco)
  m='B';

aux[f][c].opinionPolitica=m;

}

void getOpinionVeci(float opinionVec[4],int f,int c,int numV){

int fila, col, limF, limC, auxcol,i;
switch(numV){
  case 8: limF=f+2; limC=c+2; 
          fila=f-1; col=c-1; auxcol=col;
          break;
  case 3: if(f==0 && c==0){
           limF=f+2; limC=c+2;
           fila=f; col=c; auxcol=col;
          }
          else{
           if(f==0 && c==n-1){
             limF=f+2; limC=c+1;
             fila=f; col=c-1; auxcol=col;
           }
           else{
            if(f==n-1 && c==0){
              limF=f+1; limC=c+2;
              fila=f-1; col=c; auxcol=col;
            }
            else{ // f==n-1 && c==n-1
             limF=f+1; limC=c+1;
             fila=f-1; col=c-1; auxcol=col;
            }
           }
          }
          break;
 case 5: if(c==0){
          limF=f+2; limC=c+2;
          fila=f-1; col=c; auxcol=col;
          }
         else{
          if(f==0){
            limF=f+2; limC=c+2;
            fila=f; col=c-1; auxcol=col;
           }
          else{
           if (c==n-1){
            limF=f+2; limC=c+1;
            fila=f-1; col=c-1; auxcol=col;
           }
          else{ //f==n-1
             limF=f+1; limC=c+2;
             fila=f-1; col=c-1; auxcol=col;
           }
          } }
}

for( ; fila< limF; fila++){
 for( col=auxcol; col<limC; col++){
  if (!( fila==f && col==c)){
   switch(matriz[fila][col].opinionPolitica){
    case 'R': opinionVec[0]+=1; break;
    case 'A': opinionVec[1]+=1; break;
    case 'V': opinionVec[2]+=1; break;
    case 'B': opinionVec[3]+=1; 
  }}}}

for(i=0; i<4; i++)
 opinionVec[i]/=numV;

}

void completarMatriz(){

  int i;
  celda columna[m/2], fila[m/2], posicion;
  MPI_Status s;

  switch(myid){
    case 0: posicion= matriz[n-2][n-2];
            for(i=0; i< n-1; i++)
               columna[i]= matriz[i][n-2];
            for(i=0; i< n-1; i++)
               fila[i]= matriz[n-2][i];           
            MPI_Send(&posicion, (sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD);
            MPI_Send(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD);
            MPI_Send(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD);
            MPI_Recv(&posicion, (sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD,&s);
            matriz[n-1][n-1]=posicion;
            for(i=0; i< n-1; i++)
               matriz[i][n-1]=columna[i];
            for(i=0; i< n-1; i++)
               matriz[n-1][i]=fila[i];
            break;
    case 1: posicion= matriz[n-2][1];
            for(i=0; i< n-1; i++)
               columna[i]= matriz[i][1];
            for(i=1; i< n; i++)
               fila[i]= matriz[n-2][i];           
            MPI_Send(&posicion, (sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD);
            MPI_Send(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD);
            MPI_Send(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD);
            MPI_Recv(&posicion, (sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD,&s);
            matriz[n-1][0]=posicion;
            for(i=0; i< n-1; i++)
               matriz[i][0]=columna[i];
            for(i=1; i< n; i++)
               matriz[n-1][i]=fila[i];
            break;
    case 2: posicion= matriz[1][n-2];
            for(i=1; i< n; i++)
               columna[i]= matriz[i][n-2];
            for(i=0; i< n-1; i++)
               fila[i]= matriz[1][i];           
            MPI_Send(&posicion, (sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD);
            MPI_Send(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD);
            MPI_Send(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD);
            MPI_Recv(&posicion, (sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 3, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD,&s);
            matriz[0][n-1]=posicion;
            for(i=0; i< n-1; i++)
               matriz[i][n-1]=columna[i];
            for(i=0; i< n-1; i++)
               matriz[0][i]=fila[i];
            break;
    case 3: posicion= matriz[1][1];
            for(i=1; i< n; i++)
               columna[i]= matriz[i][1];
            for(i=1; i< n; i++)
               fila[i]= matriz[1][i];           
            MPI_Send(&posicion, (sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD);
            MPI_Send(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD);
            MPI_Send(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD);
            MPI_Recv(&posicion, (sizeof(celda*)), MPI_BYTE, 0, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(columna, (n-1)*(sizeof(celda*)), MPI_BYTE, 2, 0,MPI_COMM_WORLD,&s);
            MPI_Recv(fila, (n-1)*(sizeof(celda*)), MPI_BYTE, 1, 0,MPI_COMM_WORLD,&s);
            matriz[0][0]=posicion;
            for(i=1; i< n; i++)
               matriz[i][0]=columna[i];
            for(i=1; i< n; i++)
               matriz[0][i]=fila[i];
            break;
  }
  
}
