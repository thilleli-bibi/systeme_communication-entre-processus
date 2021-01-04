#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include "mfile.h"

mfifo *mymfifo1;
mfifo *mymfifo2;
mfifo *mymfifo3;
mfifo *mymfifo4;
mfifo *mymfifo5;


     /*ce programme teste la fonction mfifo_connect avec tous les cas possibles*/


int main()
{    printf("%s\n\n","------------------------mfifo_connect_test---------------------------------" );

	 /*cas 1 : mfifo nomme avec O_CREAT*/
       mymfifo1 = mfifo_connect("/objet1",O_CREAT,S_IRUSR | S_IWUSR ,10);

       if (mymfifo1 != NULL)
    		printf("%s\n","mymfifo1 (O_CREAT) reussi" );
       else
    		printf("%s\n","mymfifo1 (O_CREAT) echoué" );
       
       /*si l'objet existe déja mfifo_connect avec option 
       O_CREAT ne re crée pas l'objet mais l'ouvre seulement*/

       //modifier le champ debut,ouvrire l'objet et verfier que debut vaut 5 et non 0
       mymfifo1->objet->debut = 5;
       mymfifo1 = mfifo_connect("/objet1",O_CREAT,S_IRUSR | S_IWUSR ,10);

       printf("mymfifo1 : debut = %zu\n",mymfifo1->objet->debut );
       printf("%s\n","***************************************************************" );
   /*-------------------------------------------------------------------------------------------------------------------*/     
      /*cas2 : mfifo nomme avec O_CREAT | O_EXCL */
        //cas reussite :
        mymfifo2 = mfifo_connect("/objet6",O_CREAT | O_EXCL,S_IRUSR | S_IWUSR ,5);

        if (mymfifo2 != NULL)
    	   	printf("%s\n","mymfifo2 (O_CREAT | O_EXCL) : cas objet non existant :: reussi" );
      	else
    		  printf("%s\n","mymfifo2 (O_CREAT | O_EXCL) : cas objet non existant :: echoué" );

        //cas echou : objet2 existe deja
         mymfifo2 = mfifo_connect("/objet6",O_CREAT | O_EXCL,S_IRUSR | S_IWUSR ,5);

        if (mymfifo2 != NULL)
          printf("%s\n","mfifo2 (O_CREAT | O_EXCL) : cas objet existe déja :: reussi" );
        else
          printf("%s\n","mfifo2 (O_CREAT | O_EXCL) : cas objet existe déja :: echoué" );
        printf("%s\n","***************************************************************" );
   /*-------------------------------------------------------------------------------------*/
     /*cas3 : ouverture de mfifo déja crée*/
       //cas reussite :
       mymfifo3 =mfifo_connect("/objet1",0, S_IRUSR | S_IWUSR );

       if (mymfifo3 != NULL)
    		printf("%s\n","mymfifo3 (option 0 : ouverture ) : cas objet déja crée :: reussi" );
    	else
    		printf("%s\n","mymfifo3 (option 0 : ouverture ) : cas objet déja crée :: echoué" );

      //cas echoue : tenter d'ouvrir un objet qui n'existe pas

        mymfifo3 =mfifo_connect("/objetX",0, S_IRUSR | S_IWUSR );

       if (mymfifo3 != NULL)
         printf("%s\n","mymfifo3 (option 0 : ouverture ) : cas objet n'existe pas :: reussi" );
       else
        printf("%s\n","mymfifo3 (option 0 : ouverture ) : cas objet n'existe pas :: echoué" );
       printf("%s\n","***************************************************************" );
   /*-----------------------------------------------------------------------------------t*/   
     /*cas4 : mfifo anonym*/
      mymfifo4 = mfifo_connect(NULL,O_CREAT,S_IRUSR | S_IWUSR ,10);

      if (mymfifo4 != NULL)
    		printf("%s\n\n","mfifo4 (cas anonym ) :: reussi" );
      else
    		printf("%s\n\n","mfifo4 (cas anonym ) :: echoué" );
   /*------------------------------------------------------------------------------------*/
     
     printf("%s\n","----------------------------FIN---------------------------------------------" );

return 0;	
}