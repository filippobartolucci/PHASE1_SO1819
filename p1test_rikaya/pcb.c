#include <pcb.h>
#include <const.h>
#include <typer_rikaya.H>
#include <listx.h>

/* PCB free list handling functions */
void initPcbs(void);
void freePcb(pcb_t *p);
pcb_t *allocPcb(void);

/* PCB queue handling functions */
void mkEmptyProcQ(struct list_head *head);#include "types_rikaya.h"
#include "const.h"
#include "pcb.h"

HIDDEN pcb_t pcbFree_table[MAXPROC];

/* Lista con sentinella dei pcb liberi o inutilizzati */
LIST_HEAD(pcbFree_h);



/* PCB free list handling functions */

/* Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table. */
/* Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della struttura dati. */
void initPcbs(void){
	int i = 0;
	for(; i < MAXPROC; i++){
		pcb_t* = &pcbFree_table[MAXPROC];
		list_add_tail(&(pcb.p_next),&(pcbFree_h));
	}
}

/* Inserisce il pcb puntato da p nella lista dei PCB liberi */
void freePcb(pcb_t *p){
	    list_add_tail(&(p->p_next), &(pcbFree_h));
}


/* Restituisce NULL se la pcbFree +vuota.                                 */
/* Altrimenti rimuove un elemento dalla pcbFree,                          */
/* inizializza tutti i campi (NULL\0) e restituisce l'elemento rimosso    */
pcb_t *allocPcb(void){
	/* Controllo per verificare se pcbFree sia vuoto o no */
	if (list_empty(&(pcbFree_h))) return NULL;
	/* Elemento precedente alla sentinella è l'ultimo elemento della lista */
	/* */
	pcb_t *tmp=container_of(pcbFree_h.prev, pcb_t, p_next);
					       
	/* Rimuove tmp dalla lista pcbFree_H che lo contiene */
	list_del(tmp->p_next);

	/* Inizializzo tutti i campi di tmp a NULL */
	tmp->p_parent = NULL;
	tmp->p_semkey = NULL;
	tmp->priority = 0;
	tmp->p_s.entry_hi = 0;
	tmp->p_s.cause = 0;
	tmp->p_s.status = 0;
	tmp->p_s.pc_epc = 0;
	
	/* Per il campo p_s.gpr[] uso un for per settare a 0 tutti le 29 variabili dell' array */
	int j = 0;
	for(;j<29;j++){
		tmp->p_s.gpr[j];
	}	
	tmp->p_s.hi = 0;
	tmp->p_s.lo = 0;
							        
	INIT_LIST_HEAD(tmp->p_next);								    
	INIT_LIST_HEAD(tmp->p_child);								        
	INIT_LIST_HEAD(tmp->p_sib);
									    
	return tmp;
 }

/* PCB queue handling functions */
void mkEmptyProcQ(struct list_head *head){
	INIT_LIST_HEAD(head);
}

/* Controllo se la lista puntata da head è vuota e restituisco true in caso affermativo, false altrimenti */
int emptyProcQ(struct list_head *head){
	return (list_empty(&(head)));
}

/* Inserisco un processo in coda alla lista, controllando che la priorità sia decrescente */
void insertProcQ(struct list_head *head, pcb_t *p){
	/* Uso un puntatore a pcb_t temporaneo per scorrere la lista */
	pcb_t *tmp_pcb;
	/* Uso la macro list_for_each_entry per poter scorrere la lista partendo dalla sentinella head */
	list_for_each_entry(tmp_pcb, head, p_next){
		if(p->priority > tmp_pcb->priority){
			__list_add(&(p->next), &(list_prev(tmp_pcb->p_next)), &(tmp_pcb->p_next)); /* Inserisco l'elemento p->next se la sua priorità è maggiore della priorità dell'elemento puntato da tmp_pcb */
		}
	}

	/* Se la priorità di p non soddisfa la condizione dell'if, allora la sua priorità sarà minore di tutti gli altri elementi della lista e lo aggiungo in coda */
	list_add_tail(&(p->p_next), &(head));

}

pcb_t *headProcQ(struct list_head *head);

pcb_t *removeProcQ(struct list_head *head);
pcb_t *outProcQ(struct list_head *head, pcb_t *p);


/* Tree view functions */
int emptyChild(pcb_t *this);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t *removeChild(pcb_t *p);
pcb_t *outChild(pcb_t *p);

int emptyProcQ(struct list_head *head);
void insertProcQ(struct list_head *head, pcb_t *p);
pcb_t *headProcQ(struct list_head *head);

pcb_t *removeProcQ(struct list_head *head);
pcb_t *outProcQ(struct list_head *head, pcb_t *p);


/* Tree view functions */
int emptyChild(pcb_t *this);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t *removeChild(pcb_t *p);
pcb_t *outChild(pcb_t *p);
