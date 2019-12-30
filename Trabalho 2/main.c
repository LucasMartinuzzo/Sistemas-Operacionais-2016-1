#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//Estrutura utilizada por cada uma das threads C para armazenar o seu ID (1~5),
//um ponteiro para o número da última mensagem enviada até determinado instante
//(0~999), um ponteiro para o descrito do FIFO utilizado para a comunicação e um
//ponteiro para o semáforo utilizado para a sincronização.
typedef struct{
	int id;
	int *current_msg;
	int *fifo;
	pthread_mutex_t *mutex;
}thread_arg;

//Função responsável por criar e abrir um FIFO no modo de escrita, retornando o
//descritor que o identifica. A criação do FIFO foi feita via Shell, pois
//tivemos problemas relacionados à permissão de acesso quando utilizamos a SVC
//mkfifo().
int open_FIFO(void){
	int fifo;
	system("mkfifo fifo");
	fifo = open("fifo",O_WRONLY);
	if (fifo == -1){
		printf("Erro na abertura do FIFO: %d\n", errno);
	}
	return fifo;
}

//Função executada por cada uma das threads C, a qual consiste em gerar
//mensagens conforme o formato especificado e escrevê-las no FIFO.
void *put_message(void *arg){
	char msg[10];
	thread_arg *targ = (thread_arg *) arg;
	while(1){
		//Início da região crítica.
		pthread_mutex_lock(targ->mutex);
		int current = *(targ->current_msg);
		*(targ->current_msg) = *(targ->current_msg) + 1;
		pthread_mutex_unlock(targ->mutex);
		//Fim da região crítica. A condição abaixo define o término do envio das
		//1000 mensagens, o que faz com que as threads encerrem.
		if (current > 999){
			pthread_exit(NULL);
			return;
		}
		sprintf(msg, "%d-MSG-%03d", targ->id, current);
		printf("%s\n",msg);
		write(*(targ->fifo), msg, 9);
		sleep(1);
	}
}

//Função responsável por criar as threads C e inicializar as suas respectivas
//estruturas conforme o tipo definido no início deste arquivo. Também chamada a
//função de abertura do FIFO.
void create_threads(void){
	int i, msg = 0, fifo;
	pthread_t threads[5];
	thread_arg arguments[5];
	pthread_mutex_t mutex;
	fifo = open_FIFO();
	pthread_mutex_init(&mutex, NULL);
	for (i = 1; i <= 5; i++){
		arguments[i - 1].id = i;
		arguments[i - 1].current_msg = &msg;
		arguments[i - 1].fifo = &fifo;
		arguments[i - 1].mutex = &mutex;
		if (pthread_create(&(threads[i - 1]), NULL, put_message, &(arguments[i - 1])) != 0)
			printf("Erro na criação da thread: %d\n", errno);
	}
	//Graças à função pthread_join(), a thread atual esperará pelo término das
	//threads criadas neste código, garantindo que elas executarão put_message().
	for (i = 1; i <= 5; i++)
		pthread_join(threads[i - 1], NULL);
}

//Função principal do programa, a qual consiste apenas na chamada da função
//definida anteriormente.
int main(int argc, int **argv){
	create_threads();
	return 0;
}
