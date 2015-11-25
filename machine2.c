#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "constante.h"

//Code de la machine 2

int main(int argc, char *argv[]) {
	int verbose = atoi(argv[1]);
	if(verbose){
   		printf(KGRN "Processus machine2 : " KWHT "Execution du code propre réussi.. \n" RESET);
   		printf(KGRN "Processus machine2 : " KWHT "Récupération du file de message... \n" RESET);
	}

	int msqid;
    key_t key;
    key = KEY;
	struct msgbuf rcvbuffer;
    size_t buflen;

    if ((msqid = msgget(key, 0666)) < 0)
      	printf(KRED "Processus machine2 : impossible d'ouvrir le file de messages'... \n" RESET);

    if(verbose){
    	printf(KGRN "Processus machine2 : " KWHT "File de messages récupérée : %i... \n" RESET, msqid);
    	printf(KGRN "Processus machine2 : " KWHT "Récupération du pid de la machine 1... \n" RESET);
    }

    int pid = getpid();

	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
    	printf(KRED "Processus machine2 : impossible de récuperrer un message... \n" RESET);
	}

	if(verbose)
		printf(KGRN "Processus machine1 : " KWHT "PID reçu %s\n" RESET, rcvbuffer.mtext);

    int pidm1 = atoi(rcvbuffer.mtext);
    if(pidm1 <= 0)
		printf(KRED "Processus machine2 : PID Machine 1 incorrect : %i \n" RESET, pidm1);
	else
		if(verbose)
    		printf(KGRN "Processus machine2 : " KWHT "PID Machine 1 : %i... \n" RESET, pidm1);

    while(1){ //1 = Vrai
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, pid, 0) < 0){
	    	printf(KRED "Processus machine2 : impossible de récuperrer un message... \n" RESET);
    	}

    	//Différence entre machine 1 et machine 2
    	if(strcmp(rcvbuffer.mtext,"B") == 0 || strcmp(rcvbuffer.mtext,"C") == 0){
	    	printf(KGRN "Processus machine2 : " KWHT "pièce %s traité, envoi à l'entrepot\n", rcvbuffer.mtext);
	    	rcvbuffer.mtype = 1;
	    	if (msgsnd(msqid, &rcvbuffer, buflen, 0 < 0))
		    {
		        printf(KRED "Processus machine2 : impossible d'envoyer la pièce %s à l'entrepot... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine2 : %s \n" RESET, getError());
		        printf(KRED "Processus machine2 : Valeurs : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else if(verbose){
		    	printf(KGRN "Processus machine2 : " KWHT "Envoi de la pièce %s à l'entrepot réussi \n" RESET, rcvbuffer.mtext);
		    }
    	}else{
			buflen = strlen(rcvbuffer.mtext) + 1 ;
			rcvbuffer.mtype = pidm1;
		    printf(KGRN "Processus machine2 : " KWHT "Envoi de la pièce %s à la machine 1 \n" RESET, rcvbuffer.mtext);
		    if (msgsnd(msqid, &rcvbuffer, buflen, 0) < 0)
		    {
		        printf(KRED "Processus machine2 : impossible d'envoyer la pièce %s à la machine 1... \n" RESET, rcvbuffer.mtext);
		        printf(KRED "Processus machine2 : %s \n" RESET, getError());
		        printf(KRED "Processus machine2 : Valeurs : %d, %ld, %s, %zd\n", msqid, rcvbuffer.mtype, rcvbuffer.mtext, buflen);
		    }else if (verbose){
		    	printf(KGRN "Processus machine2 : " KWHT "Envoi réussi \n" RESET);
		    }
    	}
	}
    return 0;
}