#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "wait.h"
#include "tad.h"

/*Percorre a lista dos PIDs dos processos filhos da NSH e libera todos os que estejam no estado zumbi*/
void nsh_wait(cPIDList *cList){
	update_pid_list(cList);
    cPIDList l;
    int flag = 1;
    for(l = *cList; l!=NULL && NXT(l) != NULL;l=NXT(l)){
        int status, error;
        /*A opção WNOHANG na chamada de waitpid() abaixo é utilizada para reportar se o filho foi encerrado,
		sem que o pai bloqueie caso o status não tenha sido reportado.*/
        while(waitpid(PID(NXT(l)), &status, WNOHANG) > 0){
        	/*A chamada WIFEXITED verifica se o filho terminou normalmente.*/
			/*A chamada WEXITSTATUS verifica o código de retorno do filho caso WIFEXITED retorne true.*/
            if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
			printf("Filho terminou de forma anormal.\n");
            }
        	flag = 0;
            printf("Processo filho zumbi de PID %d liberado.\n",PID(NXT(l)));
        }
		error = errno;
			if(error == EINTR)
			printf("Função foi interrompida por um sinal.\n");
    }
    if(flag)
    	printf("Nenhum processo em estado zumbi existente.\n");
    return;
}

/*Percorre a lista dos PIDs dos processos filhos da NSH e encerra todos os remanescentes. Em seguida, libera todos os
que estejam no estado zumbi. Retorna uma "flag" que é verificada pela NSH para que ela possa saber se pode ser encerrada
ou não.*/
int nsh_exit(cPIDList *cList){
	update_pid_list(cList);
    cPIDList l;
    int flag = 0;
    for(l = *cList; l != NULL && NXT(l) != NULL;l=NXT(l)){
        int status, error;
        /*O sinal SIGKILL é enviado para que o filho seja encerrado.*/
        kill(PID(NXT(l)),SIGKILL);
        /*A opção WNOHANG na chamada de waitpid() abaixo é utilizada para reportar se o filho foi encerrado,
		sem que o pai bloqueie caso o status não tenha sido reportado.*/
        while(waitpid(PID(NXT(l)), &status, WNOHANG) > 0){
        	/*A chamada WIFEXITED verifica se o filho terminou normalmente.*/
			/*A chamada WEXITSTATUS verifica o código de retorno do filho caso WIFEXITED retorne true.*/
            if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            	flag = 1;
				printf("Filho terminou de forma anormal.\n");
            }
        }
		error = errno;
		if(error == EINTR)
			printf("Função foi interrompida por um sinal.\n");
    }
    return flag;
}
