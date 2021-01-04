#include "mfile.h"
#include <string.h>
#define LEN 5

    /*ce programme est un lecteur qui noos serivira à tester le programme write_partial_test.c*/

mfifo *mymfifo;

int main(void) {
  
  printf("%s\n","***********************************************" );
  int i=O_CREAT;
  mymfifo = mfifo_connect("/myobject8",i,S_IRUSR | S_IWUSR ,13);

  if(mymfifo == NULL) {
    perror("erreur de connexion");
    return 0;
  }

  printf("%s\n","--------avant la lecture---------------------" );
  printf("debut = %zu\n",mymfifo->objet->debut);
  printf("fin = %zu\n",mymfifo->objet->fin);


 char* buf = malloc(sizeof(char)*LEN);
 
  if(mfifo_read(mymfifo, buf, 4)==-1)
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