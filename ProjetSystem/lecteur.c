#include "mfile.h"
#include <string.h>
#define LEN 5
     
           /*ce proramme est un lecteur qui nous servira à tester le programme try_write_test.c, 
           et de voir au meme temps le comportement de la fonction mfifo_read 
           .ce programme executé seul (sans executer un write avant) se met en atente jusqu'a ce que 
            il y'aura des octets ecrit dans memory
           .executé plusieurs fois il lit tant qu'il y'a des octets dans memory,sinon il se met en attente
           .s'il demande de lire un nombre de caractere > la capacite de memory il s'interompe
           */

mfifo *mymfifo;

int main(void) {
  
  printf("%s\n","***********************************************" );
  int i=O_CREAT;
  mymfifo = mfifo_connect("/object3",i,S_IRUSR | S_IWUSR ,30);

  if(mymfifo == NULL) {
    perror("erreur de connexion");
    return 0;
  }

  printf("%s\n","--------avant la lecture---------------------" );
  printf("debut = %zu\n",mymfifo->objet->debut);
  printf("fin = %zu\n",mymfifo->objet->fin);


 char* buf = malloc(sizeof(char)*LEN);
 
  if(mfifo_read(mymfifo, buf, 3)==-1)
    printf("pas assez de caractere\n");
  else
    {
      printf("%s\n","--------apres la lecture----------------" );
      printf("debut = %zu\n",mymfifo->objet->debut);
      printf("message reçu = %s\n", buf);
    }
  sleep(1);
  printf("%s\n","***********************************************" );


  return 0;
}