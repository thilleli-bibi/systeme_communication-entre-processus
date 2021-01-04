#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"

int main(){

  if (mfifo_unlink("/obj")== -1)
    	perror("%s\n","mfifo_unlink echoué ");
    else 
    	printf("%s\n","mfifo_unlink reussi");

return 0;
}