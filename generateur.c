#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include "constante.h"

//Code du générateur de prièce brut

int main(int argc, char *argv[]) {
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

    printf(KGRN "Processus generateur : " KWHT "Envoie du message... \n" RESET);

    struct msgbuf sbuf;
    size_t buflen;

    strcpy(sbuf.mtext, "Salut");
	buflen = strlen(sbuf.mtext) + 1 ;

    if (msgsnd(msqid, &sbuf, buflen, 0) < 0)
    {
        printf ("%d, %ld, %s, %zd\n", msqid, sbuf.mtype, sbuf.mtext, buflen);
        printf(KRED "Processus generateur : impossible d'envoyer le message... \n" RESET);
        int errsv = errno;
        switch(errsv){
        	case EACCES:
				printf(KRED "Processus generateur : The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability.\n" RESET);
				break;
			case EAGAIN:
				printf(KRED "Processus generateur : The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg.\n" RESET);
				break;
			case EFAULT:
				printf(KRED "Processus generateur : The address pointed to by msgp isn't accessible.\n" RESET);
				break;
			case EIDRM:
				printf(KRED "Processus generateur : The message queue was removed.\n" RESET);
				break;
			case EINTR:
				printf(KRED "Processus generateur : Sleeping on a full message queue condition, the process caught a signal.\n" RESET);
				break;
			case EINVAL:
				printf(KRED "Processus generateur : Invalid msqid value, or non-positive mtype value, or invalid msgsz value (less than 0 or greater than the system value MSGMAX).\n" RESET);
				break;
			case ENOMEM:
				printf(KRED "Processus generateur : The system does not have enough memory to make a copy of the message pointed to by msgp.\n" RESET);
				break;
			case ENOMSG:
				printf(KRED "Processus generateur : IPC_NOWAIT was specified in msgflg and no message of the requested type existed on the message queue.\n" RESET);
			break;
        }
    }
    printf(KGRN "Processus generateur : " KWHT "Envoie du message réussi... \n" RESET);
    
    return 0;
}