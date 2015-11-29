#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>

#include "constante.h"

//Code du générateur de prièce brut

int main(int argc, char *argv[]) {
    int nb_piece = atoi(argv[1]);
    char s_verbose[2];
    strcpy(s_verbose, argv[2]);
    int verbose = atoi(argv[2]);
    int pidm1 = atoi(argv[3]);
    int pidm2 = atoi(argv[4]); 

    if(verbose){
    	printf(KGRN "Processus generateur : " KWHT "Execution du code propre réussi.. \n" RESET);
        printf(KGRN "Processus generateur : " KWHT "Récupération du file de message... \n" RESET);
    }

	int msqid;
    key_t key;
    key = KEY;

    if ((msqid = msgget(key, 0666)) < 0){
      	printf(KRED "Processus generateur : impossible d'ouvrir le file de message'... \n" RESET);
    }

	if(verbose){
    	printf(KGRN "Processus generateur : " KWHT "File de message récupéré : %i... \n" RESET, msqid);
	}
    struct msgbuf sbuf;
    size_t buflen;

    if(verbose){
        printf(KGRN "Processus generateur : " KWHT "Envoie de %d pièces via mesage... \n" RESET, nb_piece);
    }
	srand(time(NULL));


    int i;
    for ( i = 0; i < nb_piece*2; ++i)
    {
		int machine = i%2;
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

        if(verbose)
            printf(KGRN "Processus generateur : " KWHT "Envoie de la piece %s à la machine %d... \n" RESET, piece, machine+1);

    	strcpy(sbuf.mtext, piece);
		buflen = strlen(sbuf.mtext) + 1 ;

	    if (msgsnd(msqid, &sbuf, buflen, 0) < 0)
	    {
	        printf(KRED "Processus generateur : impossible d'envoyer le message... \n" RESET);
	        printf(KRED "Processus generateur : %s \n" RESET, getError());
	        printf(KRED "Processus generateur : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
	    }else if(verbose){
	    	printf(KGRN "Processus generateur : " KWHT "Envoie du message réussi... \n" RESET);
		}
    }
    return 0;
}