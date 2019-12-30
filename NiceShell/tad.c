#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "tad.h"

/*Insere um PID no início da lista dos PIDs dos processos filhos da NSH.*/
void register_child(cPIDList *list, int pid){
	cPIDList child = (cPIDList)malloc(sizeof(cPIDElement));
	if(child == NULL){
		printf("Não foi possível alocar memória.\n");
		return;
	}
	PID(child) = pid;
	/*O "if" a seguir trata o caso da primeira inserção na lista, a qual, por convenção, é inicializada com um elemento
	contendo um PID igual a -1.*/
	if(pid == -1){
		NXT(child) = NULL;
		*list = child;
		return;
	}	
	NXT(child) = NXT(*list);
	NXT(*list) = child;
	return;
}

/*Remove o PID seguinte ao PID passado como parâmetro da lista dos PIDs dos processos filhos da NSH. Funciona como uma
função auxiliar para as duas funções seguintes.*/
void delete_next_child(cPIDList *element){
	cPIDList removed;
	removed = NXT(*element);
	NXT(*element) = NXT(removed);
	free(removed);
	return;
}

/*Deleta toda a lista dos PIDs dos processos filhos da NSH.*/
void delete_pid_list(cPIDList *list){
	while(NXT(*list) != NULL)
		delete_next_child(list);
	free(*list);
	*list = NULL;
	return;
}

/*Atualiza a lista dos PIDs dos processos filhos da NSH, removendo os PIDs de filhos que já foram encerrados e não
existem mais no sistema.*/ 
void update_pid_list(cPIDList *list){
	cPIDList l;
	for(l = *list; l!=NULL && NXT(l) != NULL;l=NXT(l))
		/*A chamada kill() abaixo retorna um valor diferente de 0 caso o processo não exista mais*/
		if(kill(PID(NXT(l)),0) != 0)
			delete_next_child(&l);
	return;
}
