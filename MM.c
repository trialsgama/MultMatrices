#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>


int main(int argc, char * argv[]) {
    int numeroProc,id;
    int **A,// Matriz A 
         **B,// Matriz B
         **C,// Matriz C que sera la matriz resultado
         resultado;
    
    double tInicio, // Tiempo que comienza la ejecucion
           tfin; // Tiempo que termina el procesamiento


    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    

    A = (int **)malloc(numeroProc * sizeof(int*)); //Creando la reservacion de filas en base a procesos
    B = (int **)malloc(numeroProc * sizeof(int*)); //Creando la reservacion de filas en base a procesos
    C = (int **)malloc(numeroProc * sizeof(int*)); //Creando la reservacion de filas en base a procesos

    // Solo el proceso 0 ejecuta el siguiente bloque
    if (id == 0) {
        
        for (unsigned int i = 1; i < numeroProc; i++) {
            A[i]=(int *)malloc(numeroProc * sizeof(int));
		    C[i]=(int *)malloc(numeroProc * sizeof(int));
		    B[i]=(int *)malloc(numeroProc * sizeof(int));
        }

        srand(time(0));
        for (unsigned int i = 0; i < numeroProc; i++) {
            for (unsigned int j = 0; j < numeroProc; j++) {
  
                A[i][j] = rand() % 100;
                B[i][j] = rand() % 100;
            }
        }
        printf("[+] La matriz sera una matriz cuadrada de %i filas y columnas", numeroProc);
    } // Termina el trozo de codigo que ejecuta solo 0

    

    
    MPI_Bcast(A,//Dato a compartir
    numeroProc*numeroProc, //Numero de elementos que se van a enviar y recibir
    MPI_INT, // Tipo de dato que se va a compartir
    0, // Proceso raiz que envia los datos
    MPI_COMM_WORLD);


    //Vamos a compartir la matriz B entre todos los procesos
    MPI_Bcast(B,//Dato a compartir
    numeroProc*numeroProc, //Numero de elementos que se van a enviar y recibir
    MPI_INT, // Tipo de dato que se va a compartir
    0, // Proceso raiz que envia los datos
    MPI_COMM_WORLD);

    //Hacemos que todos los procesos comiencen la ejecucion a la vez con la funcion Barrier
    MPI_Barrier(MPI_COMM_WORLD);
    //Inicio de medicion de tiempo
    tInicio = MPI_Wtime();

    resultado = 0;
    for (unsigned int i=0; i < numeroProc; i++){
        for (unsigned int j=0 ; j < numeroProc; j++){
            for (unsigned int k=0 ; k < numeroProc; k++){
                 resultado +=A [i][k] * B[k][j];
            }
            
        }
    }

    //Agregamos otro barrier 
    MPI_Barrier(MPI_COMM_WORLD);
    // fin de medicion de tiempo
    tfin = MPI_Wtime();

    MPI_Gather(&resultado, // Dato que envia cada proceso
            1, // Numero de elementos que se envian
            MPI_INT, // Tipo del dato que se envia
            C, // Matriz en el que se recolectan los datos
            1, // Numero de datos que se esperan recibir por cada proceso
            MPI_INT, // Tipo del dato que se recibira
            0, // proceso que va a recibir los datos
            MPI_COMM_WORLD); // Canal de comunicacion (Comunicador Global)
    
    // Terminamos la ejecucion de los procesos, despues de esto solo existira
    // el proceso 0
    // Ojo! Esto no significa que los demas procesos no ejecuten el resto
    // de codigo despues de "Finalize", es conveniente asegurarnos con una
    // condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".

     for (int i=0;i<numeroProc;i++){
        for(int j=0;j<numeroProc;j++){
            printf("%d\t",C[i][j]);
        }   
        printf("\n");
    }
    
    MPI_Finalize();
 
   
   
    

}
