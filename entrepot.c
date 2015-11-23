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
    printf(KGRN "Processus entrepot : " KWHT "Fork crée et lancé, éxécution du code entrepot... \n" RESET);
    return 0;
}