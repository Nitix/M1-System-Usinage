#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>

#include <time.h>
#include <stdlib.h>

#include "constante.h"

//Code du générateur de prièce brut

int main(int argc, char *argv[]) {
	int nb_piece = 20;
    printf(KGRN "Processus generateur : " KWHT "Execution du code propre réussi.. \n" RESET);
    printf(KGRN "Processus generateur : " KWHT "Création de la machine 1\n" RESET);
    printf(KGRN "Processus generateur : " KWHT "Tentative de fork... \n" RESET);
    int pidm1 = fork();
    switch(pidm1){
    	case 0:
    		pidm1 = getpid(); 
    		printf(KGRN "Processus machine1 : " KYEL "PID %i\n"  RESET, pidm1);
    		printf(KGRN "Processus machine1 : " KWHT "Tentative de d'éxecution du code propre à machine1... \n" RESET);
    		char *envp[] = { NULL };
			char *argv[] = { "./machine1", "hello", "there", NULL };
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus machine1 : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus generateur : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    printf(KGRN "Processus generateur : " KWHT "Fork crée et lancé, création de la deuxième machine... \n" RESET);
    printf(KGRN "Processus generateur : " KWHT "Tentative de fork... \n" RESET);
    int pidm2 = fork();
    switch(pidm2){
    	case 0:
    		pidm2 = getpid(); 
    		printf(KGRN "Processus machine2 : " KYEL "PID %i\n"  RESET, pidm2);
    		printf(KGRN "Processus machine2 : " KWHT "Tentative de d'éxecution du code propre à machine2... \n" RESET);
    		char *envp[] = { NULL };
			char *argv[] = { "./machine2", "hello", "there", NULL };
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus machine2 : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus generateur : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    printf(KGRN "Processus generateur : " KWHT "Fork crée et lancé... \n" RESET);

    printf(KGRN "Processus generateur : " KWHT "Récupération du file de message... \n" RESET);

	int msqid;
    key_t key;
    key = KEY;

    if ((msqid = msgget(key, 0666)) < 0)
      	printf(KRED "Processus generateur : impossible d'ouvrir le file de message'... \n" RESET);

    printf(KGRN "Processus generateur : " KWHT "File de message récupéré : %i... \n" RESET, msqid);

    struct msgbuf sbuf;
    size_t buflen;

    char pid_m[8];

    printf(KGRN "Processus generateur : " KWHT "Envoie du pid de la machine 2 à la machine 1 : %i... \n" RESET, msqid);
    sprintf(pid_m, "%d", pidm2);
	strcpy(sbuf.mtext, pid_m);
	buflen = strlen(sbuf.mtext) + 1 ;
	sbuf.mtype = pidm1;
    if (msgsnd(msqid, &sbuf, buflen, 0 /*IPC_NOWAIT*/) < 0)
    {
        printf(KRED "Processus generateur : impossible d'envoyer le message... \n" RESET);
        printf(KRED "Processus generateur : %s \n" RESET, getError());
        printf(KRED "Processus generateur : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
    }else{
    	printf(KGRN "Processus generateur : " KWHT "Envoie du pid réussi... \n" RESET);
	}



    printf(KGRN "Processus generateur : " KWHT "Envoie du pid de la machine 1 à la machine 2 : %i... \n" RESET, msqid);
    sprintf(pid_m, "%d", pidm1);
	strcpy(sbuf.mtext, pid_m);
	buflen = strlen(sbuf.mtext) + 1 ;
	sbuf.mtype = pidm2;
    if (msgsnd(msqid, &sbuf, buflen, 0 /*IPC_NOWAIT*/) < 0)
    {
        printf(KRED "Processus generateur : impossible d'envoyer le message... \n" RESET);
        printf(KRED "Processus generateur : %s \n" RESET, getError());
        printf(KRED "Processus generateur : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
    }else{
    	printf(KGRN "Processus generateur : " KWHT "Envoie du pid réussi... \n" RESET);
	}


    printf(KGRN "Processus generateur : " KWHT "Envoie des pièces via mesage... \n" RESET);
	srand(time(NULL));


    int i;
    for ( i = 0; i < nb_piece; ++i)
    {
		int machine = rand()%2;
		int type_piece = rand()%3;
		char piece[2];
		switch(type_piece){
			case 0:
				piece[0] = 'A';
				break;
			case 1:
				piece[0] = 'B';
				break;
			case 2:
				piece[0] = 'C';
				break;
		}
		piece[1] = '\0';

		if(machine == 0){
	    	sbuf.mtype = pidm1;
	    }else{
    		sbuf.mtype = pidm2;
	    }

        printf(KGRN "Processus generateur : " KWHT "Envoie de la piece %s à la machine %d... \n" RESET, piece, machine+1);

    	strcpy(sbuf.mtext, piece);
		buflen = strlen(sbuf.mtext) + 1 ;

	    if (msgsnd(msqid, &sbuf, buflen, 0 /*IPC_NOWAIT*/) < 0)
	    {
	        printf(KRED "Processus generateur : impossible d'envoyer le message... \n" RESET);
	        printf(KRED "Processus generateur : %s \n" RESET, getError());
	        printf(KRED "Processus generateur : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
	    }else{
	    	printf(KGRN "Processus generateur : " KWHT "Envoie du message réussi... \n" RESET);
		}
    }

    return 0;
}