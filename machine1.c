#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "constante.h"

//Code de la machine 1

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int verbose = atoi(argv[1]);
	int t1 = atoi(argv[2]);
	
	if(verbose){
   		printf(KGRN "Processus machine1 : " KWHT "Exéecution du code propre réussi.. \n" RESET);
   		printf(KGRN "Processus machine1 : " KWHT "Récupération de la file de message... \n" RESET);
	}

	int msqid;
    key_t key;
    key = KEY;
	struct msgbuf rcvbuffer;
    size_t buflen;

    if ((msqid = msgget(key, 0666)) < 0)
      	printf(KRED "Processus machine1 : impossible d'ouvrir la file de message'... \n" RESET);

    if(verbose){
    	printf(KGRN "Processus machine1 : " KWHT "File de message récupérée : %i... \n" RESET, msqid);
    	printf(KGRN "Processus machine1 : " KWHT "Récupération du pid de la machine 2... \n" RESET);
    }

    int pid = getpid();

	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
    	printf(KRED "Processus machine1 : impossible de récuperrer un message... \n" RESET);
	}

	if(verbose)
		printf(KGRN "Processus machine1 : " KWHT "PID reçu %s\n" RESET, rcvbuffer.mtext);

    int pidm1 = atoi(rcvbuffer.mtext);
    if(pidm1 <= 0)
		printf(KRED "Processus machine1 : PID machine 2 incorrect : %i \n" RESET, pidm1);
	else
		if(verbose)
    		printf(KGRN "Processus machine1 : " KWHT "PID machine 2 : %i... \n" RESET, pidm1);

    while(1){ //1 = Vrai
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
	    	printf(KRED "Processus machine1 : impossible de récuperrer un message... \n" RESET);
    	}

    	//Différence entre machine 1 et machine 2
    	if(strcmp(rcvbuffer.mtext,"A") == 0 || strcmp(rcvbuffer.mtext, "B") == 0){
    		if(t1 < 0){
	    		sleep(rand()%10);
    		}else{
	    		sleep(t1);
    		}
    		if(verbose)
	    		printf(KGRN "Processus machine1 : " KWHT "pièce %s traitée, envoi à l'entrepot\n", rcvbuffer.mtext);
			rcvbuffer.mtype = 1;
	    	buflen = strlen(rcvbuffer.mtext) + 1 ;
	    	if (msgsnd(msqid, &rcvbuffer, buflen, 0)< 0)
		    {
		        printf(KRED "Processus machine1 : impossible d'envoyer la pièce %s à l'entrepot... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine1 : %s \n" RESET, getError());
		        printf(KRED "Processus machine1 : Valeures : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else if(verbose){
		    	printf(KGRN "Processus machine1 : " KWHT "Envoie de la pièce %s à l'entrepot réussi \n" RESET, rcvbuffer.mtext);
		    }
    	}else{
			buflen = strlen(rcvbuffer.mtext) + 1 ;
			rcvbuffer.mtype = pidm1;
			if(verbose)
		    	printf(KGRN "Processus machine1 : " KWHT "Envoie de la pièce %s à la machine 2 \n" RESET, rcvbuffer.mtext);

		    if (msgsnd(msqid, &rcvbuffer, buflen, 0) < 0)
		    {
		        printf(KRED "Processus machine1 : impossible d'envoyer la pièce %s à la machine 2... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine1 : %s \n" RESET, getError());
		        printf(KRED "Processus machine1 : Valeures : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else if (verbose){
		    	 printf(KGRN "Processus machine1 : " KWHT "Envoie réussi \n" RESET);
		    }
    	}
	}
    return 0;
}
