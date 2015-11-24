#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "constante.h"

//Code de la machine 2

int main(int argc, char *argv[]) {
    printf(KGRN "Processus machine1 : " KWHT "Execution du code propre réussi.. \n" RESET);

    printf(KGRN "Processus machine1 : " KWHT "Récupération du file de message... \n" RESET);

	int msqid;
    key_t key;
    key = KEY;
	struct msgbuf rcvbuffer;
    size_t buflen;

    if ((msqid = msgget(key, 0666)) < 0)
      	printf(KRED "Processus machine1 : impossible d'ouvrir le file de message'... \n" RESET);

    printf(KGRN "Processus machine1 : " KWHT "File de message récupéré : %i... \n" RESET, msqid);

    int pid = getpid();

    printf(KGRN "Processus machine1 : " KWHT "Récupération du pid de la machine 2... \n" RESET);
	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
    	printf(KRED "Processus machine1 : impossible de récuperrer un message... \n" RESET);
	}

	printf(KGRN "Processus machine1 : " KWHT "PID reçu %s\n" RESET, rcvbuffer.mtext);

    int pidm2 = atoi(rcvbuffer.mtext);
    if(pidm2 <= 0)
		printf(KRED "Processus machine1 : PID Machine 2 incorrect : %i \n" RESET, pidm2);
	else
    	printf(KGRN "Processus machine1 : " KWHT "PID Machine 2 : %i... \n" RESET, pidm2);

    while(1){ //1 = Vrai
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
	    	printf(KRED "Processus machine1 : impossible de récuperrer un message... \n" RESET);
    	}

    	//Différence entre machine 1 et machine 2
    	if(strcmp(rcvbuffer.mtext,"A") == 0 ){
	    	printf(KGRN "Processus machine1 : " KWHT "%s\n", rcvbuffer.mtext);
    	}else{
			buflen = strlen(rcvbuffer.mtext) + 1 ;
			rcvbuffer.mtype = pidm2;

		    if (msgsnd(msqid, &rcvbuffer, buflen, 0 /*IPC_NOWAIT*/) < 0)
		    {
		        printf(KRED "Processus machine1 : impossible d'envoyer la pièce %s à la machine 2... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine1 : %s \n" RESET, getError());
		        printf(KRED "Processus machine1 : Valeures : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else{
		    	printf(KGRN "Processus machine1 : " KWHT "Envoie de la pièce %s à la machine 2 réussi \n" RESET, rcvbuffer.mtext);
		    }
    	}
	}
    return 0;
}