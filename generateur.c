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
    
    return 0;
}