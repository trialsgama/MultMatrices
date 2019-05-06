#include <stdio.h>
#include <conio.h>
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

int main(int argc, char * argv[]) {
    int numeroProc,id;
    long **A,// Matriz A 
         **B,// Matriz B
         **C,// Matriz C que sera la matriz resultado
         *filaloc; // fila local donde se almacena los resultados
    double tInicio, // Tiempo que comienza la ejecucion
           tfin; // Tiempo que termina el procesamiento


    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    printf("[+] La matriz sera de %i",&numeroProc);

    A = new long *[numeroProc]; //Creando la reservacion de filas en base a procesos
    B = new long *[numeroProc]; //Creando la reservacion de filas en base a procesos
    C = new long *[numeroProc]; //Creando la reservacion de filas en base a procesos

    // Solo el proceso 0 ejecuta el siguiente bloque
    if (idProceso == 0) {
        A[0] = new long [numeroProc * numeroProc];
        B[0] = new long [numeroProc * numeroProc];
        C[0] = new long [numeroProc * numeroProc];
        for (unsigned int i = 1; i < numeroProc; i++) {
            A[i] = A[i - 1] + numeroProc;
            B[i] = B[i - 1] + numeroProc;
        }

        srand(time(0));
        for (unsigned int i = 0; i < numeroProc; i++) {
            for (unsigned int j = 0; j < numeroProc; j++) {
  
                A[i][j] = rand() % 1000;
                B[i][j] = rand() % 1000;
            }
        }
    } // Termina el trozo de codigo que ejecuta solo 0

    filaloc = new long [numeroProc]; // Reservando espacio para la fila local que guardara cada nodo

    //La funcion Scatter me permite compartir la matriz entre los procesos, separandola por filas
    MPI_Scatter(A[0],//Matriz que se compartira entre los nodos
    numeroProc, // Numero de columnas a compartir
    MPI_LONG, // Tipo de dato
    filaloc, // Vector que almacena los datos
    numeroProc, // Numero de columnas a compartir
    MPI_LONG, // Dato a recibir
    0, // Proceso raiz que envia los datos
    MPI_COMM_WORLD); // Comunicador utilizado


    //Vamos a compartir la matriz B entre todos los procesos
    MPI_Bcast(B,//Dato a compartir
    numeroProc, //Numero de elementos que se van a enviar y recibir
    MPI_LONG, // Tipo de dato que se va a compartir
    0, // Proceso raiz que envia los datos
    MPI_COMM_WORLD);

    //Hacemos que todos los procesos comiencen la ejecucion a la vez con la funcion Barrier
    MPI_Barrier(MPI_COMM_WORLD);
    //Inicio de medicion de tiempo
    tInicio = MPI_Wtime();

    long subfinal = 0;
    for (unsigned int i=0; i < numeroProc; i++){
        for (unsigned int j=0 ; j < numeroProc; j++){
            subfinal += filaloc[i] * B[i][j];
        }
    }

    //Agregamos otro barrier 
    MPI_Barrier(MPI_COMM_WORLD);
    // fin de medicion de tiempo
    tfin = MPI_Wtime();

    // Recogemos los datos de la multiplicacion, por cada proceso sera un escalar
    // y se recoge en un vector, Gather se asegura de que la recoleccion se haga
    // en el mismo orden en el que se hace el Scatter, con lo que cada escalar
    // acaba en su posicion correspondiente del vector.
    MPI_Gather(&subFinal, // Dato que envia cada proceso
            1, // Numero de elementos que se envian
            MPI_LONG, // Tipo del dato que se envia
            C[0], // Matriz en el que se recolectan los datos
            1, // Numero de datos que se esperan recibir por cada proceso
            MPI_LONG, // Tipo del dato que se recibira
            0, // proceso que va a recibir los datos
            MPI_COMM_WORLD); // Canal de comunicacion (Comunicador Global)
 
    // Terminamos la ejecucion de los procesos, despues de esto solo existira
    // el proceso 0
    // Ojo! Esto no significa que los demas procesos no ejecuten el resto
    // de codigo despues de "Finalize", es conveniente asegurarnos con una
    // condicion si vamos a ejecutar mas codigo (Por ejemplo, con "if(rank==0)".
    MPI_Finalize();
 
     
    delete [] B;
    delete [] A;
    delete [] filaloc;

}
