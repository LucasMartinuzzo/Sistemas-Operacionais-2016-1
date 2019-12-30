#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "burst.h"
#include "program.h"
#include "signal.h"
#include "tad.h"
#include "wait.h"

/*Define o número máximo de elementos lidos na linha de comando da NSH (um comando, cinco argumentos, um possível '@')*/
#define NUM_ARGS 7

/*Explicita as ações a serem tomadas pela NSH (os três comandos internos e a execução de um programa)*/
typedef enum {WAIT, BURST, EXIT, PROGRAM} type_command;

/*Lê a entrada dada por stdin e divide os seus termos, os quais são armazenados em um vetor*/
int read_and_split(char ***parameters){
	__fpurge(stdin);
	char entry[350];
	/*Os dois "ifs" a seguir identificam se a leitura da linha de comando detectou apenas um caractere de nova linha
	ou a constante EOF gerada por "Ctrl-...", pois, nesses casos, a NSH não deve tomar ação alguma. Caso a entrada
	seja válida, substitui-se o '\n' no final da mesma por '\0' (consequência do uso de fgets()).*/
    if(fgets(entry,350,stdin) == NULL)
    	return 0;
    if(strcmp(entry,"\n") == 0)
        return 0;
    else
        entry[strlen(entry) -1] = '\0';
	char **terms = (char **)malloc(NUM_ARGS*sizeof(char *));
	if (terms == NULL)
		return -1;
    char term[50], *eachTerm;
    int i,j,k = 0;
	/*O "for" abaixo é responsável por separar cada termo lido na linha de comando da NSH e armazená-los em um vetor
 	de termos, o qual é utilizado para modificar o argumento desta função. Tivemos alguns problemas com o uso de strtok(),
	portanto optamos por essa alternativa personalizada*/
    for(k=0,j=0,i=0;i <= strlen(entry);i++){
        if(entry[i]!= ' ' && entry[i] != '\0'){
            term[j] = entry[i];
            j++;
        }
        else {
            term[j] = '\0';
            eachTerm = (char *)malloc((strlen(term) + 1)*sizeof(char));
            strcpy(eachTerm,term);
            terms[k] = eachTerm;
            k++;
            j = 0;
        }
    }
	/*Abaixo encontram-se os resultados principais desta função. O argumento 'parameters' recebe o vetor de termos criado
	acima e a função retorna 'k', que corresponde ao tamanho desse vetor*/
    *parameters = terms;
    return k;
}

/*Libera a memória do vetor de termos (comando e argumentos) previamente alocado*/
void free_all(char **terms, int nTerms){
	int i;
	for(i = 0; i < nTerms; i++){
	    free(terms[i]);
	}
	free(terms);
	terms = NULL;
	return;
}

/*Checa se o primeiro termo entrado na linha de comando é um comando interno da NSH ou um programa e retorna um tipo
enumerado que será verificado na função seguinte para determinar a ação a ser tomada*/
type_command convert_command(char *command){
	if(strcmp(command,"wait") == 0)
		return WAIT;
	else if(strcmp(command,"burst") == 0)
		return BURST;
	else if(strcmp(command,"exit") == 0)
		return EXIT;
	else
		return PROGRAM;
}

/*Executa a Nice Shell (NSH), redirecionando-a de acordo com os termos entrados*/
void nsh (){
	char **terms;
	int nTerms;
	cPIDList cList = NULL;
	register_child(&cList,-1);
	define_nsh_bg_handler();
	while(1){
		printf("nsh> ");
		nTerms = read_and_split(&terms);
		if(nTerms == 0)
		    continue;
		switch(convert_command(*terms)){
			case WAIT:
				nsh_wait(&cList);
				break;
			case BURST:
				burst(&cList);
				break;
			case EXIT:
				free_all(terms,nTerms);
				if(nsh_exit(&cList) == 0){
					delete_pid_list(&cList);
					return;
				}
				break;
			case PROGRAM:
                execute(terms,nTerms,&cList);
				break;
		}
		free_all(terms,nTerms);
	}
	return;
}

/*Função principal, apenas invoca a função anterior*/
int main(int argc, char **argv){
    nsh();
    return 0;
}
