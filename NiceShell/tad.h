#ifndef CPIDLIST_H
#define CPIDLIST_H

#define PID(L) ((L)->cPID)
#define NXT(L) ((L)->next)

/*Define uma lista encadeada dos PIDs dos processos filhos das NSH, a qual será utilizada para a implemetação de
seus comandos internos "burst", "wait" e "exit".*/
typedef struct cPIDElement cPIDElement, *cPIDList;
struct cPIDElement {
	pid_t cPID;
	cPIDList next;
};

void register_child(cPIDList *, int);
void delete_next_child(cPIDList *);
void delete_pid_list(cPIDList *);
void update_pid_list(cPIDList *);

#endif
