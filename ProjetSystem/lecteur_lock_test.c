#include "mfile.h"
#include <string.h>
#define LEN 5
/*ce programme teste la fonction read,avec des rédacteur (write ou trywrite) :
        1.à la premiére execution de ce programme read réussit a lire un message si y en des messages ecrit par les rédacteur.
          -le lecteur exécute mfifo_lock pour vérrouiller l'objet mfifo.
          -fait une lecture.
          -le lecteur déverrouille l'objet en exécutant la fonction mfifo_unlock.*/
        
      

mfifo *mymfifo;

int main(void) {
  
  printf("%s\n","***********************************************" );
  int i=O_CREAT;
  mymfifo = mfifo_connect("/object2",i,S_IRUSR | S_IWUSR ,30);

  if(mymfifo == NULL) {
    perror("erreur de connexion");
    return 0;
  }

  printf("%s\n","--------avant la lecture---------------------" );
  printf("debut = %zu\n",mymfifo->objet->debut);
  printf("fin = %zu\n",mymfifo->objet->fin);


 char* buf = malloc(sizeof(char)*LEN);
 mfifo_lock(mymfifo);
  if(mfifo_read(mymfifo, buf, 3)==-1)
    printf("pas assez de caractere\n");
  else
    {
      printf("%s\n","--------apres la lecture----------------" );
      printf("debut = %zu\n",mymfifo->objet->debut);
      printf("message reçu = %s\n", buf);
    }
    mfifo_unlock(mymfifo);
  sleep(1);
  printf("%s\n","***********************************************" );


  return 0;
}