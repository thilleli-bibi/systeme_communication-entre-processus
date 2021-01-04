#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"
		/*ce programme teste la fonction write,avec un lecteur "lecteur.c" :
        1.à la premiére execution de ce programme write reussit à ecrire le message :
        	on a suffisament d'esapce dans memory(20 octets) pour ecrire la taille du message (un size_t (8octets))
        	ainsi que le message lui meme de taille 7 octets,au total 15 octets seront ecrit dans memory.
        	on visualise ça en regardant les valeurs de debut et fin (changent uniquement si y'a eu une ecriture 
        	on une lecture )
        
        2.à la deuxieme execution le message sera pas ecrit 
        (il nous reste que 5 octets dans memmory < 15 octets dont on a besoin comme expliqué dans1) 
        et la valeur de debut et fin seront les memes q'avant l'execution de write
		
		3.on exécute maintenant le lecteur ( lecteur.c) qui lit le message écrit dans 1,
		 et on exécute write_test qui écrit cette fois (vuqu'on a libérer assez d'espace dans memory)*/

mfifo *mymfifo;

int main(void) {

  
    mymfifo = mfifo_connect("/mfifo",O_CREAT,S_IRUSR | S_IWUSR ,20);
  
   if(mymfifo == NULL) {
    	perror("erreur de connexion");
    	return 0;
   }

   printf("debut = %zu\n",mymfifo->objet->debut);
   printf("fin = %zu\n",mymfifo->objet->fin);

  
   mfifo_write(mymfifo, "Bonjour", 7);

   printf("------apres l'ecriture du message------------\n");

   printf("debut = %zu\n",mymfifo->objet->debut);
   printf("fin = %zu\n",mymfifo->objet->fin);

  return 0;


}
