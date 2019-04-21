#include "listx.h"
#include "scheduler.h"

void scheduler(void) {
	termprint("inizio scheduler\n", 0);
	/*gestione dei deadlock poi dei processi*/
	if (current_process == NULL){
		if(process_count == 0)
			HALT();
		current_process = removeProcQ(&ready_queue);
		restorePriority(current_process);
		priorityAging();
		log_process_order(current_process->original_priority);
		setTIMER(TIMESLICE);
		LDST(&current_process->p_s);
	}	
}

/* Funzione che si occupa del meccanismo di aging delle priorità dei PCB nella ready queue */
HIDDEN inline void priorityAging(void) {
	struct list_head *iter;
	list_for_each(iter, &ready_queue){
		pcb_t *tmp = container_of(iter, pcb_t, p_next);
		if (tmp->priority<20) tmp->priority++;
		}
}

/* Funzione che si occupa di ripristinare la priorità originale di un PCB */
HIDDEN inline void restorePriority(pcb_t *pcb){
	pcb->priority = pcb->original_priority; 	
}
