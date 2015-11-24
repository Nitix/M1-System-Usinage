#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "constante.h"

//Code de la machine 2

int main(int argc, char *argv[]) {
    printf(KGRN "Processus machine2 : " KWHT "Execution du code propre réussi.. \n" RESET);

    printf(KGRN "Processus machine2 : " KWHT "Récupération du file de message... \n" RESET);

	int msqid;
    key_t key;
    key = KEY;
	struct msgbuf rcvbuffer;
    size_t buflen;

    if ((msqid = msgget(key, 0666)) < 0)
      	printf(KRED "Processus machine2 : impossible d'ouvrir le file de message'... \n" RESET);

    printf(KGRN "Processus machine2 : " KWHT "File de message récupéré : %i... \n" RESET, msqid);

    int pid = getpid();

    printf(KGRN "Processus machine2 : " KWHT "Récupération du pid de la machine 1... \n" RESET);
	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
    	printf(KRED "Processus machine2 : impossible de récuperrer un message... \n" RESET);
	}

	printf(KGRN "Processus machine1 : " KWHT "PID reçu %s\n" RESET, rcvbuffer.mtext);

    int pidm1 = atoi(rcvbuffer.mtext);
    if(pidm1 <= 0)
		printf(KRED "Processus machine2 : PID Machine 1 incorrect : %i \n" RESET, pidm1);
	else
    	printf(KGRN "Processus machine2 : " KWHT "PID Machine 1 : %i... \n" RESET, pidm1);

    while(1){ //1 = Vrai
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
	    	printf(KRED "Processus machine2 : impossible de récuperrer un message... \n" RESET);
    	}

    	//Différence entre machine 1 et machine 2
    	if(strcmp(rcvbuffer.mtext,"B") == 0 || strcmp(rcvbuffer.mtext,"C") == 0){
	    	printf(KGRN "Processus machine2 : " KWHT "%s\n", rcvbuffer.mtext);
    	}else{
			buflen = strlen(rcvbuffer.mtext) + 1 ;
			rcvbuffer.mtype = pidm1;

		    if (msgsnd(msqid, &rcvbuffer, buflen, 0 /*IPC_NOWAIT*/) < 0)
		    {
		        printf(KRED "Processus machine2 : impossible d'envoyer la pièce %s à la machine 1... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine2 : %s \n" RESET, getError());
		        printf(KRED "Processus machine2 : Valeures : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else{
		    	printf(KGRN "Processus machine2 : " KWHT "Envoie de la pièce %s à la machine 1 réussi \n" RESET, rcvbuffer.mtext);
		    }
    	}
	}
    return 0;
}