#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "burst.h"
#include "program.h"
#include "signal.h"
#include "tad.h"

/*Verifica se um '@' foi passado junto aos termos da linha de comando da NSH. Caso positivo, retira o '@' e indica que
o processo deve ser iniciado em background. Também insere o NULL necessário no final do vetor de termos para que ele
possa ser passado como parâmetro da função execvp()*/
type_stat verify_background(char ***terms, int *nTerms){
	if(strcmp((*terms)[*nTerms-1],"@") == 0){
		free((*terms)[*nTerms-1]);
		(*terms)[*nTerms-1] = NULL;
		*nTerms = (*nTerms) -1;
		return BG;
	}
	else{
		(*terms)[*nTerms] = NULL;
		*nTerms = *nTerms - 1;
		return FG;
	}
}

/*Executa o programa passado pelo primeiro termo lido na linha de comando da NSH, considerando os seus parâmetros*/
void execute(char **terms, int nTerms, cPIDList *cList){
	pid_t pid;
	int status;
	type_stat stat = verify_background(&terms,&nTerms);
	if((pid = fork()) < 0){
		printf("Não foi possível criar um processo filho.\n");
		exit(1);
	}
	/*Código executado pelos filhos da NSH após o fork()*/
	else if(pid == 0){
		set_min_priority();
		if(stat == BG){
			setsid();
			define_children_bg_handler();
		}
		else
			define_children_fg_handler();
		if(execvp(*terms,terms) < 0){
			status = errno;
			printf("%s: programa não encontrado.\n",*terms);
			/*O "if" a seguir garante o aparecimento do prompt "nsh> " da NSH quando o execvp() tenta executar um
			programa inválido em background.*/
			if(stat == BG)
				printf("nsh> ");
			exit(1);
		}
	}
	/*Código executado pela NSH após o fork()*/
	else{
		int error;
		register_child(cList,pid);
		if(stat != BG){
			define_nsh_fg_handler();
			/*A opção WUNTRACED na chamada de waitpid() abaixo é utilizada reportar se o filho da NSH em foreground foi
			parado através de "Ctrl-z". Nesse caso, a NSH deve ser desbloqueada e voltar a exibir o prompt "nsh> ".*/
			while(waitpid(pid,&status,WUNTRACED) != pid);
			error = errno;
			if(error == ECHILD)
				printf("Processo de PID %d não existe ou não é filho do processo que chamou.\n", pid);
			define_nsh_bg_handler();
		}
	}
	return;
}
