#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "constante.h"

int main(int argc, char *argv[]) {
	printf(KGRN "Processus entrepot : " KWHT "Lancement... \n" RESET);
    int pid = getpid(); 
    printf(KGRN "Processus entrepot : " KYEL "PID %i\n"  RESET, pid);
    printf(KGRN "Processus entrepot : " KWHT "Création du générateur... \n" RESET);
    printf(KGRN "Processus entrepot : " KWHT "Tentative de fork... \n" RESET);
    pid = fork();

    switch(pid){
    	case 0:
    		pid = getpid(); 
    		printf(KGRN "Processus generateur : " KYEL "PID %i\n"  RESET, pid);
    		printf(KGRN "Processus generateur : " KWHT "Tentative de d'éxecution du code propre à générateur... \n" RESET);
    		char *envp[] = { NULL };
			char *argv[] = { "./generateur", "hello", "there", NULL };
    		execve(argv[0], argv, envp);
    		printf(KRED "Processus generateur : Execution du code impossible... \n" RESET);
    		return 1;
		case -1:
			printf(KRED "Processus entrepot : impossible de lancer le fork... \n" RESET);
			return 1;
		default:
			break;
    }
    printf(KGRN "Processus entrepot : " KWHT "Fork crée et lancé... \n" RESET);

    //http://simplestcodings.blogspot.fr/2010/08/ipc-message-queue-implementation-in-c.html

    printf(KGRN "Processus entrepot : " KWHT "Création du file de message... \n" RESET);

    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;

    key = KEY;


    if ((msqid = msgget(key, msgflg )) < 0)  { //Get the message queue ID for the given key
      	printf(KRED "Processus entrepot : impossible de créer le pipe de communication... \n" RESET);
    }
    printf(KGRN "Processus entrepot : " KWHT "File de message créé : %i... \n" RESET, msqid);

    printf(KGRN "Processus entrepot : " KWHT "Attente d'un message... \n" RESET);
    struct msgbuf rcvbuffer;
     //Receive an answer of message type 1.
    int i;
    for ( i = 0; i < 21; ++i){
    	if (msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0)
	    	printf(KRED "Processus entrepot : impossible de récuperrer un message... \n" RESET);
	    printf(KGRN "Processus entrepot : " KWHT "%s\n", rcvbuffer.mtext);
	}

    return 0;
}