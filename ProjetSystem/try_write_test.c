#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"

       /*ce programme teste la fonction try_write,avec un lecteur "lecteur.c" :
        1.à la premiére execution de ce programme try_write reussit à ecrire le message :
        on a suffisament d'esapce dans memory(20 octets) pour ecrire la taille du message (un size_t (8octets))
        ainsi que le message lui meme de taille 3 octets,au total 11 octets seront ecrit dans memory.
        on visualise ça en regardant les valeurs de debut et fin (changent uniquement si y'a eu une ecriture 
        on une lecture )
        
        2.à la deuxieme execution le message sera pas ecrit 
        (il nous reste que 9 octets dans memmory < 11 octets dont on a besoin comme expliqué dans1) 
        et la valeur de debut et fin seront les memes q'avant l'execution de trywrite

        3.on execute maintenat le lecteur (lecteur.c) qui lit le message ecrit dans 1, et 
        on reexecute try_write_test qui ecrit cette fois (vu qu'on a liberer assez d'espace dans memory)*/

mfifo *mymfifo;
int main(void) {

    printf("%s\n","********************************************" );
    int i=O_CREAT;
    mymfifo = mfifo_connect("/object3",i,S_IRUSR | S_IWUSR ,20);
  
  if(mymfifo == NULL) {
    perror("erreur de connexion");
    return 0;
  }
  
  printf("%s\n","-------avant trywrite--------------" );
  printf("debut = %zu\n", mymfifo->objet->debut);
  printf("fin = %zu\n", mymfifo->objet->fin);

  
   mfifo_trywrite(mymfifo, "moi", 3);
   
   sleep(1);

   printf("------------apres trywrite--------------\n");

   printf("debut = %zu\n",mymfifo->objet->debut);
   printf("fin = %zu\n",mymfifo->objet->fin);

   printf("%s\n","********************************************" );



  return 0;


}