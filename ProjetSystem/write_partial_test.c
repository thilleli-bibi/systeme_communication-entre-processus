#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"
 
       /*ce programme teste la fonction write_partial avec le lecteur lecteur2.c
         1.l'execution ce programme ecrit 4 octets du message (fran)
         et se met en attente pour ecrire les deux octets restants; cela parceque 
         il peut pas ecrire tous le message dans memory (capacity 13 octets), il a beoin de 
         15 ocets = 6+8(sizeof(sizet) pour inscrire la taille) +1 (fin doit etre tjr vide)
 
         2.on execute juste apres (dans un autre terminal) le programme lecteur2.c (qui lit les 4 octets dans memory),
           le programme (write_partial_test) ecrit les deux octets restants (ce).
         3.on peut executer ce programme le nombre de fois qu'on veut il aura toujours le meme comportement
       */


mfifo *fifo;
int main()
{
	printf("%s\n","********************************************" );
 
    int i=O_CREAT;
    fifo = mfifo_connect("/myobject8",i,S_IRUSR | S_IWUSR ,13);
  
  if(fifo == NULL) 
    perror("erreur de connexion");
    
  printf("%s\n","-------avant write_partial--------------" );
  printf("debut = %zu\n", fifo->objet->debut);
  printf("fin = %zu\n", fifo->objet->fin);

  
   mfifo_write_partial(fifo, "france", 6);
   
   sleep(1);

   printf("------------apres write_partial--------------\n");

   printf("debut = %zu\n",fifo->objet->debut);
   printf("fin = %zu\n",fifo->objet->fin);

   printf("%s\n","********************************************" );
  


return 0;
}