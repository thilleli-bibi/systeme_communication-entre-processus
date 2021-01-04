#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"
mfifo *mymfifo;
int main(){
int i=O_CREAT;
    mymfifo = mfifo_connect("/obj",i,S_IRUSR | S_IWUSR ,33);
  if (mfifo_disconnect(mymfifo)== -1)
    	perror("mfifo_disconnect echoué ");
    else 
    	printf("mfifo_disconnect reussi\n");

return 0;
}