#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

float* mat1;
float* mat2;
float* saida;
int N_THREADS,M_DIM;
int* indices;

//funcao executada pelas threads
void* tarefa(void* arg)
{
	int id = *((int*) arg);
	printf("t%d criada\n",id);
	for(int i = id; i < M_DIM; i+= N_THREADS)//linha
	{
		
		for(int j = 0; j < M_DIM; j++)
		{
			
			//linha+= mat1[(M_DIM * i) + j] * mat2[(M_DIM * j) + i];
			for(int n = 0; n < M_DIM; n++)
			{
				saida[(M_DIM * i) + j] += (mat1[(M_DIM*i) + n]) * (mat2[(M_DIM*n) + j]) ;
			}
		}
		
	}
	pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("comando invalido.\nformato aceito: %s <dim> <threads>\n",argv[0]);
		return 1;
	}
	M_DIM = (int) atoi(argv[1]);
	N_THREADS = (int) atoi(argv[2]);
	if(M_DIM == 0 || N_THREADS == 0)
	{
		printf("valores invalidos.\n valores aceitos: dim > 0, threads > 0, threads <= dim\n");
		return 2;
	}
	if (M_DIM < N_THREADS)
	{
		N_THREADS = M_DIM;
		printf("numero de threads acima do desejado, diminuindo para threads = dim.\n"); 
	}
	//inicializar
	mat1 = (float*) malloc(sizeof(float) * M_DIM * M_DIM);
	mat2 = (float*) malloc(sizeof(float) * M_DIM * M_DIM);
	saida = (float*) malloc(sizeof(float) * M_DIM * M_DIM);
	indices = (int*) malloc(sizeof(int) * N_THREADS);
	
	
	if(mat1 == NULL || mat2 == NULL || saida == NULL || indices == NULL)
	{
		printf("ERRO MALLOC\n");
		return 3;
	}
	for (int i = 0; i < M_DIM; i++)
	{
		for(int j = 0; j < M_DIM; j++)
		{
			mat1[i*M_DIM + j] = i + j;
			mat2[i*M_DIM + j] = i + j;
			saida[i*M_DIM + j] = 0;			
		}
	}
	//threads e time
	double inicio,fim,delta;
	pthread_t* tid;
	tid =(pthread_t*) malloc(sizeof(pthread_t) * N_THREADS);
	GET_TIME(inicio);
	for(int i = 0; i < N_THREADS; i++)
	{
		indices[i] = i;
		if(pthread_create((tid + i),NULL,tarefa,(void*) (indices + i)))
		{
			printf("ERRO PTHREAD_CREATE\n");
			return 4;
		}
	}
	//espera pelas threads
	for(int i = 0; i < N_THREADS; i++)
	{
		pthread_join(tid[i],NULL);
	}
	GET_TIME(fim);
	delta = fim - inicio;
	printf("%f\n", delta);
	/*for(int i = 0; i < M_DIM; i++)
	{
		printf("[ ");
		for(int j = 0; j < M_DIM; j++)
		{
			printf("%f ",saida[M_DIM * i + j]);
		}
		printf("]\n");
	}*/
	
	//encerra
	free(mat1);
	free(mat2);
	free(saida);
	free(indices);
	
	
	return 0;
}
