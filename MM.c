#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

//Predefino las matrices
const int n = 10;

int A[n][n];
int B[n][n];
int C[n][n];
int a[n],c[n];
 

int main(int argc, char * argv[]) {
    int size,rank,sum; //size=numero de procesos, rank= process id, sum=auxiliar que usaremos para la suma 
   

    
    double tInicio, // Tiempo que comienza la ejecucion
           tfin; // Tiempo que termina el procesamiento


    MPI_Init(&argc,&argv); //Inicio MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); //seteo size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //seteo rank
    
         

        srand(time(0));
	//Relleno las matrices
        for (unsigned int i = 0; i < n; i ++) {
            for (unsigned int j = 0; j < n; j ++) {
  		//Valores aleatorios 
                A[i][j] = rand() % 10; 
                B[i][j] = rand() % 10;
            }
        }

   

   //Compartimos la matriz A entre los nodos	
   MPI_Scatter(A, // Matriz que vamos a dividir
	 n*n/size, // Determinamos el numero de datos dividiendo la matriz entre el numero de procesos
	 MPI_INT, // Tipo de dato
	 a, // Matriz donde almacenaremos localmente los datos
	 n*n/size, // Datos a recibir
	 MPI_INT, // tipo de datos a recibir
	0, // Proceso que envia los datos
	MPI_COMM_WORLD); // comunicador


    //Vamos a compartir la matriz B entre todos los procesos
    MPI_Bcast(B,//Dato a compartir
    n*n, //Numero de elementos que se van a enviar y recibir
    MPI_INT, // Tipo de dato que se va a compartir
    0, // Proceso raiz que envia los datos
    MPI_COMM_WORLD);

    //Hacemos que todos los procesos comiencen la ejecucion a la vez con la funcion Barrier
    MPI_Barrier(MPI_COMM_WORLD);
    //Inicio de medicion de tiempo
    tInicio = MPI_Wtime();


    for (unsigned int i=0; i < n; i ++){
        for (unsigned int j=0 ; j < n; j ++){
                 sum = sum + a[j] * B[j][i]; //Recorro la matriz B y la multiplico por los valores de la variable local y sumo a sum
	        }
	c[i]=sum; // asigno el valor de sum en esa pasada al vector auxiliar c
	sum=0; // Dejo sum en 0 para la proxima pasada
    }

     MPI_Gather(c, // Dato que envia cada proceso
            n*n/size, // Numero de elementos que se envian
            MPI_INT, // Tipo del dato que se envia
            C, // Matriz en el que se recolectan los datos
            n*n/size, // Numero de datos que se esperan recibir por cada proceso
            MPI_INT, // Tipo del dato que se recibira
            0, // proceso que va a recibir los datos
            MPI_COMM_WORLD); // Canal de comunicacion (Comunicador Global)
  

    //Agregamos otro barrier 
    MPI_Barrier(MPI_COMM_WORLD);
    // fin de medicion de tiempo
    tfin = MPI_Wtime();

  
    // Terminamos la ejecucion de los procesos, despues de esto solo existira
    // el proceso 0
    
    MPI_Finalize();
    //Ejecutamos esto cuando es el proceso 0	
    if (rank==0) {
    //Mostramos la matriz A
    printf("La matriz A es:\n");
		
    	 for (int i=0;i<n;i ++){
       		for(int j=0;j<n;j ++){
        	   printf("%d\t",A[i][j]);
		       }   
      		printf("\n");
    		}
   //Mostramos Matriz B
   printf("La matriz B es:\n");
		
    	 for (int i=0;i<n;i ++){
       		for(int j=0;j<n;j ++){
        	   printf("%d\t",C[i][j]);
		       }   
      		printf("\n");
    		}  
   Mostramos matriz C	
    printf("La matriz C es:\n");
		
    	 for (int i=0;i<n;i ++){
       		for(int j=0;j<n;j ++){
        	   printf("%d\t",C[i][j]);
		       }   
      		printf("\n");
    		}
    //Mostramos el tiempo tardado	
    printf("El tiempo tardado es %f segundos\n", tfin - tInicio);

    }
    
    return 0;
 
   
   
    

}
