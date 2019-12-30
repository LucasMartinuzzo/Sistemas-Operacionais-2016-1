#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h"

/*Trata os sinais "Ctrl-..." enviados para a NSH quando ela não possui filho algum ou apenas filhos em background.*/
void msg_handler(int signal){
	char letter;
	switch(signal){
		case SIGINT:
			letter = 'c';
			break;
		case SIGTERM:
			letter = 'u';
			break;
		case SIGQUIT:
			letter = '\\';
			break;
		case SIGTSTP:
			letter = 'z';
			break;
	}
	printf("\nNão adianta me enviar o sinal por Ctrl-%c. Não vou fazer nada!!\n", letter);
	return;
}

/*Trata de sinais "Ctrl-..." enviados para a NSH quando ela possui um filho em foreground (apenas pula a linha para
que o prompt ">nsh " possa aparecer corretamente na linha seguinte a "^C", "^U", "^\" ou "^Z").*/
void newline_handler(int signal){
	printf("\n");
	return;
}

/*Especifica o tratamento da NSH em relação aos sinais "Ctrl-..." quando ela possui um filho em foreground.*/
void define_nsh_fg_handler(void){
	int error;
	struct sigaction act;
	act.sa_handler = newline_handler;
	act.sa_flags = 0;
	/*O "if" a seguir bloqueia o recebimento de quaisquer sinais (exceto SIGKILL e SISTOP) durante o tratamento. Esse 
	mesmo trecho de código é executado nas funções abaixo, portanto os comentários seguintes serão omitidos.*/
	if(sigfillset(&act.sa_mask) == -1){
		printf("Não foi possível bloquear o recebimento de sinais durante o tratamento dos sinais \"Ctrl-...\" .\n");
		return;
	}
	if ((sigaction(SIGINT, &act, NULL) == -1) || (sigaction(SIGTERM, &act, NULL) == -1) ||
		(sigaction(SIGQUIT, &act, NULL) == -1) || (sigaction(SIGTSTP, &act, NULL) == -1)){
		error = errno;
		switch(error){
			case EFAULT:
				printf("Não foi possível especificar o tratamento dos sinais: acesso indevido à memória.\n");
				break;
			case EINVAL:
				printf("Não foi possível especificar o tratamento dos sinais: sinal inválido.\n");
				break;
		}
	}
	return;
}

/*Especifica o tratamento da NSH em relação aos sinais "Ctrl-..." quando ela não possui filho algum ou apenas filhos
em background.*/
void define_nsh_bg_handler(void){
	int error;
	struct sigaction act;
	act.sa_handler = msg_handler;
	act.sa_flags = 0;
	if(sigfillset(&act.sa_mask) == -1){
		printf("Não foi possível bloquear o recebimento de sinais durante o tratamento dos sinais \"Ctrl-...\" .\n");
		return;
	}
	if ((sigaction(SIGINT, &act, NULL) == -1) || (sigaction(SIGTERM, &act, NULL) == -1) ||
		(sigaction(SIGQUIT, &act, NULL) == -1) || (sigaction(SIGTSTP, &act, NULL) == -1)){
		error = errno;
		switch(error){
			case EFAULT:
				printf("Não foi possível especificar o tratamento dos sinais: acesso indevido à memória.\n");
				break;
			case EINVAL:
				printf("Não foi possível especificar o tratamento dos sinais: sinal inválido.\n");
				break;
		}
	}	
	return;
}

/*Especifica o tratamento de um processo filho da NSH rodando em foreground (faz com que ele ignore somente SIGTERM e 
trate os demais sinais da forma padrão, conforme a especificação do trabalho).*/
void define_children_fg_handler(void){
	int error;
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	if(sigfillset(&act.sa_mask) == -1){
		printf("Não foi possível bloquear o recebimento de sinais durante o tratamento de SIGTERM.\n");
		return;
	}
	if (sigaction(SIGTERM, &act, NULL) == -1){
		error = errno;
		switch(error){
			case EFAULT:
				printf("Não foi possível especificar o tratamento de SIGTERM: acesso indevido à memória.\n");
				break;
			case EINVAL:
				printf("Não foi possível especificar o tratamento de SIGTERM: sinal inválido.\n");
				break;
		}
	}
	return;
}

/*Especifica o tratamento de um processo filho da NSH rodando em background (faz com que ele ignore todos os sinais do
tipo "Ctrl-...").*/
void define_children_bg_handler(void){
	int error;
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	if(sigfillset(&act.sa_mask) == -1){
		printf("Não foi possível bloquear o recebimento de sinais durante o tratamento dos sinais \"Ctrl-...\" .\n");
		return;
	}
	if ((sigaction(SIGINT, &act, NULL) == -1) || (sigaction(SIGTERM, &act, NULL) == -1) ||
		(sigaction(SIGQUIT, &act, NULL) == -1) || (sigaction(SIGTSTP, &act, NULL) == -1)){
		error = errno;
		switch(error){
			case EFAULT:
				printf("Não foi possível especificar o tratamento dos sinais: acesso indevido à memória.\n");
				break;
			case EINVAL:
				printf("Não foi possível especificar o tratamento dos sinais: sinal inválido.\n");
				break;
		}
	}
	return;
}
