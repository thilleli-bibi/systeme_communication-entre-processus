#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"
 
         /*
            ce programmr sert a tester la fonction unlock.c 
            .seul il affiche une erreur car l'objet n'est pas encore vérouillé
            .on l'execute comme indiqué dans le programme lock_unlock_trylock_test.c 
         */


mfifo *fifo;
int main()
{   
   printf("%s\n","********************************************" );
 
    int i=O_CREAT;
    fifo = mfifo_connect("/mfif",i,S_IRUSR | S_IWUSR ,13);

    if(fifo == NULL) 
    	perror("erreur de connexion");

    //appeler mfifo_unlock et voir la valeur du verrou
     printf("%s\n","-------mfifo_unlock-----------------" );

     if(mfifo_unlock(fifo)!=-1)
      printf("%s\n","mfifo_unlock reussie" );
     else
      printf("%s\n","mfifo_unlock echouée" );
    //afficher la valeur du verrou
    printf("aprés unlock verrou = %d\n\n",fifo->objet->verrou );
 


 return 0;
}
 