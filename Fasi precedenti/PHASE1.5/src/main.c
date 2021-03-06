/*						*
 *		 PHASE1 RIKAYA	   		*
 *						*
 * 	 Sviluppato dal gruppo lso19az22 	*
 *						*
 * 	 Componenti del gruppo:	   		*
 *	   - Filippo Bartolucci	   		*
 *	   - Francesco Cerio		     	*/
 
#include "pcb.h"
#include "const.h"
#include "handler.h"
#include "listx.h"
#include "scheduler.h"
#include "types_rikaya.h"
#include "initArea.h"
#include "utils.h"

/* Funzioni di test per PHASE1.5 */
extern void test1();
extern void test2();
extern void test3();

/* Lista dei processi ready */
LIST_HEAD(ready_queue);
/* Puntatore al processo attivo */
pcb_t* current_process = NULL;
/* Contatore processi */
u32 process_count = 0;

/* Puntatori alle NEW AREA della ROM */
state_t *sysbk_newarea = (state_t *)SYSBK_NEWAREA;
state_t *program_trap_newarea = (state_t *)PGMTRAP_NEWAREA;
state_t *interrupt_newarea = (state_t *)INT_NEWAREA;
state_t *tblmgt_newarea = (state_t *)TLB_NEWAREA;

/* Puntatori alle OLD AREA della ROM */
state_t *sysbk_oldarea = (state_t *)SYSBK_OLDAREA;
state_t *program_trap_oldarea = (state_t *)PGMTRAP_OLDAREA;
state_t *interrupt_oldarea = (state_t *)INT_OLDAREA;
state_t *tblmgt_oldarea = (state_t *)TLB_OLDAREA;

/* Funzione che si occupa di caricare i processi nello scheduler 
 * settando in maniera oppurtuna i vari campi:
 *    - Interrupt ON;
 *    - Virtual Memory OFF;
 *    - Processor Local Timer ON;
 *    - $SP a RAMSIZE-FRAMESIZE*n;
 *    - Priorità uguale ad n
 *    - pc_epc = (memaddr) testn;
*/
void setProcess(memaddr proc, int n){
    /* Prendo un PCB dalla lista dei PCB liberi */
	pcb_t *tmp = allocPcb();
    /* Imposto il PROGRAM COUNTER del processo */
	tmp->p_s.pc_epc = proc;
	tmp->p_s.reg_t9 = proc;
    /* Imposto la priorità */
	tmp->priority = n;
	tmp->original_priority = n;
    /* Imposto lo STACK POINTER */
	tmp->p_s.reg_sp = RAMTOP - FRAME_SIZE * n;
    /* Imposto lo STATUS del process */
	tmp->p_s.status = STATUS_P ;
	
    /* Aumento il contatore dei processi */
	process_count++;
    /* Inserisco il PCB nella lista dei processi in stato ready */
	insertProcQ(&ready_queue, tmp);
}

int main(void){
    
    termprint("\nPKAYA_OS\n",0);

    /* Inizializzazione del sistema */
    termprint("- Inizializzazione newArea\n", 0);
    initAREA();
    termprint("- Inizializzazione dei PCB()\n", 0);
    initPcbs(); 

    termprint("- Caricamento processi di test\n", 0);
    
    setProcess((memaddr)test1, 1);
    setProcess((memaddr)test2, 2);
    setProcess((memaddr)test3, 3);

    termprint("- Avvio dello scheduler\n\n",0);
	
    /* Passo il controllo allo scheduler */
    scheduler();
    
    /* L'esecuzione non può mai arrivare qua */
    PANIC();

    return 0;
}

