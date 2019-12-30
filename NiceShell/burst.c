#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "burst.h"
#include "tad.h"

/*Atribui prioridade mínima a um processo. Utilizada na criação dos filhos da NSH*/
void set_min_priority(void){	
    int error;
    if(setpriority(PRIO_PROCESS,0,19) == -1){
    	error = errno;
		switch(error){
			case ESRCH:
				printf("Não foi possível atribuir prioridade mínima: nenhum processo com o PID %d foi localizado.\n",(int) getpid());
			case EPERM:
				printf("Não foi possível atribuir prioridade mínima: permissão negada.\n");
		}
	}
    return;
}

/*Atribui prioridade máxima a um processo através da execução de um comando da shell. Para fazê-lo, é necessário possuir
a permissão adequada, portanto será solicitada a inserção de uma senha autorizando o comando*/
int set_max_priority(pid_t cPID){
	char cmd[100] = "sudo renice -20 ";
	char pid[10];
	sprintf(pid,"%d",(int) cPID);
	strcat(cmd,pid);
	strcat(cmd," -u");
	return system(cmd);
}	

/*Percorre a lista dos PIDs dos processos filhos da NSH e atribui prioridade máxima a todos eles*/
void burst(cPIDList *list){
	update_pid_list(list);
	cPIDList l;
	for(l = *list;l!=NULL && NXT(l) != NULL;l=NXT(l)){
		if (set_max_priority(PID(NXT(l))) == -1)
			printf("Não foi possível executar o burst no processo de PID %d.\n",(int) PID(NXT(l)));
	}
	return;
}
