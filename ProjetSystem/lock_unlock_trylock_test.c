#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"
 
         /*ce programme sert à visualiser les comportement des fonctions mfifo_lock,mfifo_unlock et mfifo_trylock
          -pour precision le champs verrou de l'objet mfifo est à 0 lorsque il y'a aucun verrou posé sur l'objet
           et à 1 si y'a un processus qui posé un verrou sur l'objet
           et on a egalement le programme "lecteur_lock_test.c" dont on utilise ces fonction avec un read et des writes.

         */


mfifo *fifo;
int main()
{   
   printf("%s\n","********************************************" );
 
    int i=O_CREAT;
    fifo = mfifo_connect("/mfif",i,S_IRUSR | S_IWUSR ,13);

    if(fifo == NULL) 
    	perror("erreur de connexion");
 
    //un premier lock qui resussira
    printf("%s\n","----------avant lock-------------" );
    printf("verrou = %d\n",fifo->objet->verrou );
     if (mfifo_lock(fifo)!=-1)
      	 printf("%s\n","mfifo_lock reussie!" );
      else
      	printf("%s\n","mfifo_lock echoué" );
   
    printf("%s\n","----------apres lock----------------" );
    printf("verrou = %d\n\n",fifo->objet->verrou );

       
    //un mfifo_trylock qui echoue emidiateemnt
     if (mfifo_trylock(fifo)!=-1)
      	 printf("%s\n\n","mfifo_trylock reussie!" );
      else
      	printf("%s\n\n","mfifo_try_lock echoué" );



    //essayons de verouiller le meme objet fifo qui est toujours verouillé
    printf("%s\n","******2eme lock sur le méme objet vérouillé*******" );
      if (mfifo_lock(fifo)!=-1)
      	 printf("%s\n\n","mfifo_lock reussie!" );
      else
      	printf("%s\n\n","mfifo_lock echoué" );
    
    
     //maintenat executer le proramme "unlock.c"
     //le lock va sortir de son attente et verouille l'objet    


return 0;
}