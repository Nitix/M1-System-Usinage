#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


#include "constante.h"

int main(int argc, char *argv[]) {
	int n_arg;
	int nb_piece = 0;
	int verbose = 0;
	int t1 = -1;
	int t2 = -1;

	for(n_arg = 1; n_arg < argc; n_arg++){
		if (strcmp(argv[n_arg], "-n") == 0) 
		{
			++n_arg;
			if(argc == n_arg){
				printf(KRED "Nombre de pièce manquant après -n\n" RESET);
				return 1;
			}
			nb_piece = atoi(argv[n_arg]);
		} 
		else if (strcmp(argv[n_arg], "-v") == 0)
		{
			verbose = 1;
		}
		else if (strcmp(argv[n_arg], "-t1") == 0)
		{
			++n_arg;
			if(argc == n_arg){
				printf(KRED "Temps manquant après -t1\n" RESET);
				return 1;
			}
			t1 = atoi(argv[n_arg]);
		}
		else if (strcmp(argv[n_arg], "-t2") == 0)
		{
			++n_arg;
			if(argc == n_arg){
				printf(KRED "Temps manquant après -t2\n" RESET);
				return 1;
			}
			t2 = atoi(argv[n_arg]);
		}
		else if (strcmp(argv[n_arg], "-h") == 0 || strcmp(argv[n_arg], "--help") == 0)
		{
			printf("%s -n nombre [-t1 temps] [-t2 temps] [-v]\n", argv[0]);
			printf("\t-n nombre de pièces à fabriquer\n");
			printf("\t-t1 Temps de fabrication de la machine 1\n");
			printf("\t-t2 Temps de fabrication de la machine 2\n");
			printf("\t-v mode verbose\n");

			printf("\n");
			printf("Si le temps n'est pas spécifié, cela sera une valeur aléatoire entre 0 et 10 secondes pour chaque machine et pièce\n");
			return 0;
		}
		else
		{
			printf(KRED "Paramètre %s non reconnu\n" RESET, argv[n_arg]);
		}
	}
	if(nb_piece == 0){
		printf(KRED "Nombre de pièce incorrect ou paramètre -n manquant \n" RESET);
		return 1;
	}

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;

    key = KEY;

	char s_nb_piece[8];
	sprintf(s_nb_piece, "%d" ,nb_piece);
	char s_verbose[2];
	sprintf(s_verbose, "%d" ,verbose);
	char s_t1[8];
	sprintf(s_t1, "%d" ,t1);
	char s_t2[8];
	sprintf(s_t2, "%d" ,t2);

	printf(KGRN "Processus entrepot : " KWHT "Lancement... \n" RESET);
	if(!verbose)
		printf("\e[1;1H\e[2J");
    int pid = getpid(); 

    if(verbose){
    	printf(KGRN "Processus entrepot : " KYEL "PID %i\n"  RESET, pid);
    	printf(KGRN "Processus entrepot : " KWHT "Création du file de message... \n" RESET);
    }

    if ((msqid = msgget(key, msgflg )) < 0)  { //Get the message queue ID for the given key
      	printf(KRED "Processus entrepot : impossible de créer la queue de message... \n" RESET);
    }
    if(verbose){
	    printf(KGRN "Processus entrepot : " KWHT "File de message créé : %i... \n" RESET, msqid);
	    printf(KGRN "Processus entrepot : " KWHT "Création de la machine 1\n" RESET);
    	printf(KGRN "Processus entrepot : " KWHT "Tentative de fork... \n" RESET);
    }
    struct msgbuf sbuf;
    size_t buflen;

    int pidm1 = fork();
    switch(pidm1){
    	case 0:
    		pidm1 = getpid(); 
    		if(verbose) {
                printf(KGRN "Processus machine1 : " KYEL "PID %i\n"  RESET, pidm1);
    			printf(KGRN "Processus machine1 : " KWHT "Tentative de d'éxecution du code propre à machine1... \n" RESET);
            }
    		char *envp[] = { NULL };
			char *argv[] = { "./machine1", s_verbose, s_t1, NULL};
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus machine1 : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus entrepot : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    if(verbose){
    	printf(KGRN "Processus entrepot : " KWHT "Fork crée et lancé \n" RESET);
        printf(KGRN "Processus entrepot : " KWHT "Création de la machine 2 \n" RESET);
    	printf(KGRN "Processus entrepot : " KWHT "Tentative de fork... \n" RESET);
    }
    int pidm2 = fork();
    switch(pidm2){
    	case 0:
    		pidm2 = getpid(); 
            if(verbose){
    		    printf(KGRN "Processus machine2 : " KYEL "PID %i\n"  RESET, pidm2);
    			printf(KGRN "Processus machine2 : " KWHT "Tentative de d'éxecution du code propre à machine2... \n" RESET);
            }
    		char *envp[] = { NULL };
			char *argv[] = { "./machine2", s_verbose, s_t2, NULL};
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus machine2 : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus entrepot : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    if(verbose){
    	printf(KGRN "Processus entrepot : " KWHT "Fork crée et lancé... \n" RESET);
    	printf(KGRN "Processus entrepot : " KWHT "Envoie du pid de la machine 2 à la machine 1... \n" RESET);
	}
	char pid_m[8];
    sprintf(pid_m, "%d", pidm2);
	strcpy(sbuf.mtext, pid_m);
	buflen = strlen(sbuf.mtext) + 1 ;
	sbuf.mtype = pidm1;

    if (msgsnd(msqid, &sbuf, buflen, 0) < 0)
    {
        printf(KRED "Processus entrepot : impossible d'envoyer le message... \n" RESET);
        printf(KRED "Processus entrepot : %s \n" RESET, getError());
        printf(KRED "Processus entrepot : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
    }else if(verbose){
    	printf(KGRN "Processus entrepot : " KWHT "Envoie du pid réussi... \n" RESET);
	}


    if(verbose)
    	printf(KGRN "Processus entrepot : " KWHT "Envoie du pid de la machine 1 à la machine 2... \n" RESET);
    sprintf(pid_m, "%d", pidm1);
	strcpy(sbuf.mtext, pid_m);
	buflen = strlen(sbuf.mtext) + 1 ;
	sbuf.mtype = pidm2;
    if (msgsnd(msqid, &sbuf, buflen, 0) < 0)
    {
        printf(KRED "Processus entrepot : impossible d'envoyer le message... \n" RESET);
        printf(KRED "Processus entrepot : %s \n" RESET, getError());
        printf(KRED "Processus entrepot : Valeures : %d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
    }else if(verbose){
    	printf(KGRN "Processus entrepot : " KWHT "Envoie du pid réussi... \n" RESET);
    	printf(KGRN "Processus entrepot : " KWHT "Création du générateur... \n" RESET);
  		printf(KGRN "Processus entrepot : " KWHT "Tentative de fork... \n" RESET);
    }

    char s_pidm1[8];
	sprintf(s_pidm1, "%d" ,pidm1);
	char s_pidm2[8];
	sprintf(s_pidm2, "%d" ,pidm2);

    pid = fork();

    switch(pid){
    	case 0:
    		pid = getpid(); 
    		if(verbose){
    			printf(KGRN "Processus generateur : " KYEL "PID %i\n"  RESET, pid);
    			printf(KGRN "Processus generateur : " KWHT "Tentative de d'éxecution du code propre à générateur... \n" RESET);
    		}
    		char *envp[] = { NULL };
			char *argv[] = { "./generateur", s_nb_piece, s_verbose, s_pidm1, s_pidm2,NULL};
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus generateur : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus entrepot : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    if(verbose){
    	printf(KGRN "Processus entrepot : " KWHT "Fork crée et lancé... \n" RESET);
    	printf(KGRN "Processus entrepot : " KWHT "Prêt à recevoir les pièces... \n" RESET);
    }



    struct msgbuf rcvbuffer;
     //Receive an answer of message type 1.
    int i;

    int nb_a = 0;
    int nb_b = 0;
    int nb_c = 0;
    int j;
    for ( i = 1; i <= nb_piece*2; i++){
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
	    	printf(KRED "Processus entrepot : impossible de récuperrer un message... \n" RESET);
	    if(verbose)
	   		printf(KGRN "Processus entrepot : " KWHT "Reception de la piece %s\n" RESET, rcvbuffer.mtext);
	   	if(strcmp(rcvbuffer.mtext,"A") == 0){
			nb_a++;
		}else if(strcmp(rcvbuffer.mtext,"B") == 0){
			nb_b++;
		}else if(strcmp(rcvbuffer.mtext,"C") == 0){
			nb_c++;
		}
		if(verbose){
	    		printf(KGRN "Processus entrepot : " KWHT "Entrepot remplit à %d / %d \n" RESET, i, nb_piece*2);
		}else{
			printf("\e[1;1H\e[2J");
			//printf("--------------------------------------------------------------------------------------------\n");
			printf("A ");
			for (j = 0; j < nb_a; ++j)
			{
				printf("| ");
			}
			printf("(%d)\n", nb_a);
			printf("B ");
			for (j = 0; j < nb_b; ++j)
			{
				printf("| ");
			}
			printf("(%d)\n", nb_b);
			printf("C ");
			for (j = 0; j < nb_c; ++j)
			{
				printf("| ");
			}
			printf("(%d)\n", nb_c);
			printf("Total : %d / %d\n", nb_a+nb_b+nb_c, nb_piece * 2);
			fflush(stdout); // On ejecte l'affichage
		}
	}

	kill(pidm1, SIGINT);
	kill(pidm2, SIGINT);

	if(msgctl(msqid, IPC_RMID, NULL) < 0){
	    	printf(KRED "Processus entrepot : impossible de fermer la queue de message... \n" RESET);
	} 
    return 0;
}
