#include "mfile.h"
/*-----------la capacite total de mfifo----------------------------------------------------------------------------*/
size_t mfifo_capacity(mfifo *fifo) {
  return fifo->objet->capacity;
}
int fileVide(mfifo *fifo) {
  return (fifo->objet->count == 0);
}
/*--------------------------------------------------------------------------------------------------------------------------*/
/*int filePleine(mfifo *fifo) {
  return (fifo->debut == fifo->fin) ;
}*/
/*------afficher les messages d'erreurs de l'initialisation des mutex conditions------------------------------------------*/
/*les fonctions thread_error,initialiser_mutex et initialiser_cond
  sont prises d'un exemple vu en cour
*/
void thread_error(const char *file, int line, int code, char *txt){
  if( txt != NULL )
    fprintf( stderr,  "[%s] in file %s in line %d :  %s\n",
       txt, file , line, strerror( code ) );
  else
    fprintf( stderr,  "in file %s in line %d :  %s\n",
       file , line, strerror( code ) );
  exit(1);
}
/*-----initialisation des mutex condition-----------------------------------------------------------------------------------------------------------*/
int initialiser_mutex(pthread_mutex_t *pmutex){
    pthread_mutexattr_t mutexattr;
  int code;
  if( ( code = pthread_mutexattr_init(&mutexattr) ) != 0)     
    return code;

  if( ( code = pthread_mutexattr_setpshared(&mutexattr,
              PTHREAD_PROCESS_SHARED) ) != 0)     
    return code;
  code = pthread_mutex_init(pmutex, &mutexattr)  ;
  return code;
}

int initialiser_cond(pthread_cond_t *pcond){
  pthread_condattr_t condattr;
  int code;
  pthread_condattr_init(&condattr);
  pthread_condattr_setpshared(&condattr,PTHREAD_PROCESS_SHARED);
  if( ( code = pthread_cond_init(pcond, &condattr) ) != 0)
    return code;
  code = pthread_cond_init(pcond, &condattr)  ;
  return code;
} 
/*---------------------------------------------------------------------------------------------------------------------*/
mfifo *connect_mfifo_anonym(int options,mode_t permission,size_t capacite )
{
  size_t len = sizeof(struct mfifo) + capacite ;
     fifo1 *mfifo_object = (fifo1 *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  
    if(mfifo_object == MAP_FAILED) {
        perror("erreur de mmap");
    return  NULL;
    }


    //initialisation de mfifo 

          //mfifo_object->nom = NULL;
          mfifo_object->permission = permission;
          mfifo_object->capacity = capacite;
          mfifo_object->debut = 0;
          mfifo_object->fin = 0;
          mfifo_object->count = 0;
          mfifo_object->pid = -1;
          mfifo_object->verrou = 0;
         
           int err;
          if( ( err = initialiser_mutex( &mfifo_object->mutex ) ) < 0 )
            thread_error(__FILE__,__LINE__, err, "initialiser mutex");
          if( (err = initialiser_cond( &mfifo_object->rcond )) < 0 )
              thread_error(__FILE__,__LINE__, err,"initialiser rcond");
          if( (err = initialiser_cond( &mfifo_object->wcond )) < 0 )
            thread_error(__FILE__,__LINE__, err,"initialiser wcond");

           if( ( err = initialiser_mutex( &mfifo_object->read_mutex ) ) < 0 )
          thread_error(__FILE__,__LINE__, err, "initialiser mutex");

    
   // if(msync(mfifo_object,len,MS_SYNC)==-1)
        // perror("errreur msync");
           mfifo* reponse=malloc(sizeof(mfifo));
          reponse->option = options;
          reponse->objet = mfifo_object;

    return reponse;  
}
/*******************************************************************************************************/
  mfifo *open_mfifo( const char *nom, int options)
  {
     int fd = shm_open(nom,O_RDWR, S_IRUSR | S_IWUSR );

     if(fd<0) {
      perror("shm_open");
      return NULL;
    }

    struct stat bufStat;
    fstat(fd, &bufStat);
    size_t len = bufStat.st_size;

    fifo1 *mfifo_object = (fifo1 *)mmap(0 , len , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//sizeof(mfifo) a la place de len

    if(mfifo_object == MAP_FAILED) {
      perror("erreur de mmap");
      return NULL;
    }

  if(msync(mfifo_object,len,MS_SYNC)==-1)
       perror("errreur msync");

  mfifo* reponse=malloc(sizeof(mfifo));
          reponse->option = options;
          reponse->objet = mfifo_object;

    return reponse;
  }
/************************************************************************************************************/
 mfifo *connect_mfifo_nomme( const char *nom, int options, mode_t permission,size_t capacite )
{
      if (shm_open(nom,O_RDWR, S_IRUSR | S_IWUSR ) < 0 )
        {  
          int fd = shm_open(nom, options | O_RDWR , S_IRUSR | S_IWUSR);
          if(fd<0)
          {
            perror("shm_open");
            return NULL;
          }
            size_t len = sizeof(fifo1) + capacite;
            ftruncate(fd,len ); 
          
            struct stat bufStat;
            fstat(fd,&bufStat);   

 
             fifo1 *mfifo_object = (fifo1 *)mmap(0, len , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
          
          if(mfifo_object == MAP_FAILED) {
             perror("erreur de mmap");
             return  NULL;
          }
          close(fd);
          

          //initialisation de mfifo 
              
            //mfifo_object->nom = nom;      
            mfifo_object->permission = permission;
          mfifo_object->capacity = capacite;
          mfifo_object->debut = 0;
          mfifo_object->fin = 0;
          mfifo_object->count = 0;
          mfifo_object->pid = -1;
          mfifo_object->verrou = 0;

          
          mfifo* reponse=malloc(sizeof(mfifo));
          reponse->option = options;
          reponse->objet = mfifo_object;

             int err;
        if( ( err = initialiser_mutex( &mfifo_object->mutex ) ) < 0 )
          thread_error(__FILE__,__LINE__, err, "initialiser mutex");
        if( (err = initialiser_cond( &mfifo_object->rcond )) < 0 )
            thread_error(__FILE__,__LINE__, err,"initialiser rcond");
        if( (err = initialiser_cond( &mfifo_object->wcond )) < 0 )
          thread_error(__FILE__,__LINE__, err,"initialiser wcond");
        
         if( ( err = initialiser_mutex( &mfifo_object->read_mutex ) ) < 0 )
          thread_error(__FILE__,__LINE__, err, "initialiser mutex");


            
           /* if(msync(mfifo_object,len,MS_SYNC)==-1){
              perror("errreur msync");
            }*/
        printf("creation de file CAPACITE %ld\n", mfifo_capacity(reponse));
     return reponse;
  }
 
     return open_mfifo(nom,options);
}

/*************************************************************************************************************************************/
 mfifo *connect_mfifo_nomme_O_EXCL( const char *nom, int options, mode_t permission,size_t capacite )
{
  if (shm_open(nom,O_RDWR, S_IRUSR | S_IWUSR ) < 0 )
        {  
          int fd = shm_open(nom, options | O_RDWR , S_IRUSR | S_IWUSR);
          if(fd<0)
          {
            perror("shm_open");
            return NULL;
          }
            size_t len = sizeof(fifo1) + capacite;
            ftruncate(fd,len ); 
          
            struct stat bufStat;
            fstat(fd,&bufStat);   

 
             fifo1 *mfifo_object = (fifo1 *)mmap(0, len , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
          
          if(mfifo_object == MAP_FAILED) {
             perror("erreur de mmap");
             return  NULL;
          }
          close(fd);
          

          //initialisation de mfifo 
              
            //mfifo_object->nom = nom;      
            mfifo_object->permission = permission;
          mfifo_object->capacity = capacite;
          mfifo_object->debut = 0;
          mfifo_object->fin = 0;
          mfifo_object->count = 0;
          mfifo_object->pid = -1;
          mfifo_object->verrou = 0;

          
          mfifo* reponse=malloc(sizeof(mfifo));
          reponse->option = options;
          reponse->objet = mfifo_object;

             int err;
        if( ( err = initialiser_mutex( &mfifo_object->mutex ) ) < 0 )
          thread_error(__FILE__,__LINE__, err, "initialiser mutex");
        if( (err = initialiser_cond( &mfifo_object->rcond )) < 0 )
            thread_error(__FILE__,__LINE__, err,"initialiser rcond");
        if( (err = initialiser_cond( &mfifo_object->wcond )) < 0 )
          thread_error(__FILE__,__LINE__, err,"initialiser wcond");
        
         if( ( err = initialiser_mutex( &mfifo_object->read_mutex ) ) < 0 )
          thread_error(__FILE__,__LINE__, err, "initialiser mutex");


            
           /* if(msync(mfifo_object,len,MS_SYNC)==-1){
              perror("errreur msync");
            }*/
     return reponse;
  }
     return NULL;
}
/***********************************************************************************************************/
/*fait appel au fonctions : open_mfifo,connect_mfifo_nomme_O_EXCL,
connect_mfifo_nomme,connect_mfifo_anonym selon est ce que l'objet et nommé ou pas, 
et selon option si l'objet est nommé*/
mfifo *mfifo_connect( const char *nom, int options, ... )
{ 
  //mfifo nommes
  va_list va;
  va_start (va, options);
  if (nom != NULL)  
  {
      if (options == 0)   //connexion direct (ouverture) 
          return open_mfifo(nom,options);

      if ((!(options & O_EXCL ) == 0))  //O_CREAT|O_EXCL 
         {
          mode_t permission = va_arg (va, mode_t);
          size_t capacite = va_arg (va, size_t);
          return connect_mfifo_nomme_O_EXCL(nom,options,permission,capacite );
         }
      if (!(options & O_CREAT)==0)    //  O_CREAT
      {   
      	  mode_t permission = va_arg (va, mode_t);
          size_t capacite = va_arg (va, size_t);
          return connect_mfifo_nomme(nom,options,permission,capacite );
      }
  }
    //anonym
    mode_t permission = va_arg (va, mode_t);
    size_t capacite = va_arg (va, size_t);
    return connect_mfifo_anonym(options,permission,capacite);
  
    va_end (va);
                
}
/*----------------------------------------------------------------------------------------------------------------------*/
/*int mfifo_disconnect(mfifo *fifo)
{
  size_t len =sizeof(mfifo) + sizeof(fifo->objet->memory);

  return munmap(fifo,len);  
}*/
/*---------------------------------------------------------------------------------------------------------------------*/
int mfifo_unlink(const char *nom)
{
  return shm_unlink(nom);
}
/*-------------------------------------------------------------------------------------------------------------------*/
size_t mfifo_free_memory(mfifo *fifo){
 
  int espace = fifo->objet->debut - fifo->objet->fin;
  
  if (espace < 0) {
    return mfifo_capacity(fifo) + espace;

  }
  else if(espace > 0){
		return espace;
  }
  else  {

  	if(espace==0){
        if (fileVide(fifo)) {
			return mfifo_capacity(fifo);;
    }
    }

      return 0;
   
  }
}
/*---------------------------------------------------------------------------------------------------------------------*/
//la fonction mis a jour le champs fin de mfifo
//incremente count++ (nombre de message ecrit dans mfifo)
//retourne la valeur de fin a partir de laquelle on peut ecrire le message 

int majEcriture(mfifo *fifo, size_t len) {

  int old = fifo->objet->fin;
  fifo->objet->count++;
  fifo->objet->fin = (fifo->objet->fin + len + sizeof(size_t)) % mfifo_capacity(fifo);

  return old;
}
/*--------------------------------------------------------------------------------------------------------------------*/
//la fonction ecrit la taille du message ainsi que le message lui meme dans memory
int ecrire(fifo1 * objet, const void *buf, size_t len, int indexEcrire) {
  
  //ecrire la taille du message dans mfifo
  memcpy((objet->memory+indexEcrire), &len, sizeof(size_t));

  //ecrire le message dans mfifo
  memcpy((objet->memory+indexEcrire+sizeof(size_t)), buf, len);


  return 0;
}
/*--------------------------------------------------mfifo_write------------------------------------------------------------------*/
int mfifo_write(mfifo *fifo, const void *buf, size_t len)
{
  
    if (len+1 > fifo->objet->capacity) 
     { 
      perror("lonngeur du message > capacity de mfifo !");
      errno = EMSGSIZE ; 
      return -1;
     }  
    
       pthread_mutex_lock( & fifo->objet->mutex );
        
       /* if((mfifo_capacity(fifo) - fifo->objet->fin) < sizeof(size_t)+len) {   //capacite restante < taille de message + taille d'un size_t (un entier represeantant la longeur du message) 
          fifo->objet->last = fifo->objet->fin;
          fifo->objet->fin=0;
         }*/

          while (mfifo_free_memory(fifo) < len+sizeof(size_t)+1) {  //1
          printf("processus %d en attente\n", (int) getpid());
           pthread_cond_wait( & fifo->objet->wcond ,& fifo->objet->mutex );
         }

         int indexEcrire = majEcriture(fifo,len);
         pthread_mutex_unlock( & fifo->objet->mutex );

          int size = ecrire(fifo->objet, buf, len, indexEcrire);

  pthread_cond_broadcast( & fifo->objet->rcond );

  if(!size) {
    if(fifo->objet->debut == -1) {
      fifo->objet->debut = 0;
    }
  }

  
//}
return size;
}

/*-------------------------------------------------------mfifo_trywrite-----------------------------------------------------------*/
int mfifo_trywrite(mfifo *fifo,const void*buf, size_t len){

   if (len+1 > fifo->objet->capacity) 
     { 
      perror("lonngeur du message > capacity de mfifo !");
      errno = EAGAIN ; 
      return -1;
     }  

     if(pthread_mutex_trylock( & fifo->objet->mutex ) != 0) {
         perror("erreur trylock (trywrite)");
         return -1;
     }
       
          if (mfifo_free_memory(fifo) < len+sizeof(size_t) +1)
            {
              pthread_mutex_unlock( & fifo->objet->mutex );
            	perror("pas assez d'espace!");
            	return -1;
            } 

         int indexEcrire = majEcriture(fifo,len);
         pthread_mutex_unlock( & fifo->objet->mutex );

          int size = ecrire(fifo->objet, buf, len, indexEcrire);

  pthread_cond_broadcast( & fifo->objet->rcond );

  if(!size) {
    if(fifo->objet->debut == -1) {
      fifo->objet->debut = 0;
    }
  }

  
//}
return size;
}
/*--------------------------------------------------------mfifo_write_partial-------------------------------------------------------------*/
int mfifo_write_partial(mfifo *fifo, const void *buf, size_t len)
{
	size_t taille = len ;
	int pos = 0 ;   //sert a controler les positions dans buf

   if (len+1 > fifo->objet->capacity) 
     { 
      perror("lonngeur du message > capacity de mfifo !");
      errno = EAGAIN ; 
      return -1;
     }  
   
    if(pthread_mutex_trylock( & fifo->objet->mutex ) != 0) {
         perror("erreur trylock");
         return -1;
     }
    
      //cas ou on a suffisament d'esapce pour tous le message
      if (mfifo_free_memory(fifo) > len+sizeof(size_t)+1)
      {
      	  int indexEcrire = majEcriture(fifo,len);
      	  pthread_mutex_unlock( & fifo->objet->mutex );
      	  
      	  return ecrire(fifo->objet, buf, len, indexEcrire);
          
      }
      //
          int free_memory ;
         while (mfifo_free_memory(fifo) < taille + sizeof(size_t)+1) {
         	   
         	   if(mfifo_free_memory(fifo)==0 || mfifo_free_memory(fifo) < sizeof(size_t)+2 )  //ecire la taille et au moins un octet plus un octet vide pour fin 
         	    {  
               	 printf("%s%zu%s\n","------il reste ", taille ," octets a ecrire----------" );
         	   	  printf("processus %d en attente\n", (int) getpid());
         	   	  pthread_cond_wait( & fifo->objet->wcond ,& fifo->objet->mutex );
         	   }
         	   else 
         	   {
         	   	  //printf("%s\n","----------je suis dans le else " );
         	   	  free_memory = mfifo_free_memory(fifo);
         	   	  taille = taille - (free_memory -sizeof(size_t) -1); //taille du msg a ecire //1octet de fin
         	   
         	   	  int indexEcrire = majEcriture(fifo,free_memory-sizeof(size_t)-1);  //ecrire dans tous les octets dispo
                pthread_mutex_unlock( & fifo->objet->mutex ); 
                pthread_cond_broadcast( & fifo->objet->rcond );               
 
                  ecrire(fifo->objet, buf + pos, free_memory-sizeof(size_t)-1, indexEcrire);
                  pos = pos + free_memory -sizeof(size_t) -1;   //position du prochain caractere dans buf a ecrire dans mfifo
                  
                  /* if(pthread_mutex_trylock( & fifo->objet->mutex ) != 0) {
                        perror("erreur trylock");
                        return -1;
                   }*/
                      printf("%s\n","--write partial---" ); 
                      printf("debut = %zu\n",fifo->objet->debut);
                       printf("fin = %zu\n",fifo->objet->fin);

                   if (taille == 0 )   //on a tous ecrit
                   	  { 
                   	  	pthread_mutex_unlock( & fifo->objet->mutex );
                   	    return 0;
                      }
         	   }
               
               }

          pthread_mutex_unlock( & fifo->objet->mutex );
          pthread_cond_broadcast( & fifo->objet->rcond );               
return 0;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------*/
int mfifo_trylock(mfifo *fifo)
{
  if(pthread_mutex_trylock( & fifo->objet->read_mutex ) != 0) {
         perror("mfifo_lock");
         return -1;
     }   
   fifo->objet->verrou = 1;

  return 0;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------*/
int mfifo_unlock(mfifo *fifo)
{
  if(pthread_mutex_unlock( & fifo->objet->read_mutex )!= 0) {
         perror("mfifo_unlock");
         return -1;
     }  
       fifo->objet->verrou = 0;
       pthread_cond_broadcast( & fifo->objet->rcond );//a  verifier not sure 

  return 0;  
}
/*----------------------------------------------------------------------------------------------------------------------------------*/
int mfifo_lock(mfifo *fifo)
{
  while (fifo->objet->verrou == 1);
  
  if(pthread_mutex_trylock( & fifo->objet->read_mutex ) != 0) {
         perror("mfifo_lock");
         return -1;
     }             

  fifo->objet->verrou = 1;  
  return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*lit d'abord la taille du message à lire 
  puis le message lui meme, qui sera copié dans le buffer
  retourne le nombre d'octets lus
*/
size_t lire(mfifo * fifo, void *buf, int l, int indexLire ) {

  size_t lenMsg;
 
  memcpy(&lenMsg, fifo->objet->memory+indexLire, sizeof(size_t));

  printf("%ld la longeur du message lu\n",lenMsg);

  if(l < lenMsg) {
    errno = EMSGSIZE;
    perror("lire");
    return -1;
  }
    memcpy( buf, fifo->objet->memory+indexLire+sizeof(size_t), lenMsg);
    printf(" PROCESSUS %d lit le message %s de longeur %ld\n",getpid(),(char*)buf,lenMsg);

 return lenMsg;

}
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*retourne l'index à partir de quel le processus peur lire dans memmory
 modifie debut pour l'avancer du nombre d'octet du message courant
*/
int majLecture(mfifo * fifo, size_t l) {

  //size_t lenMsg;
  //memcpy(&lenMsg, file->files->fileMsg+file->files->first, sizeof(size_t));

  int old = fifo->objet->debut;
  fifo->objet->count--;
    //printf("LECTURE PAR LE PROCESSUS %d -------------- %ld et encien %d message longeur %ld\n",getpid(),(file->files->first+lenMsg+sizeof(size_t))%msg_capacite(file),old,lenMsg);

  fifo->objet->debut = (fifo->objet->debut+l+sizeof(size_t))%mfifo_capacity(fifo);
  //printf(" LECTURE PAR LE PROCESSUS %d -------------- NOMBRE DE MESSAGE %ld  debut %d fin %d\n",  getpid(), fifo->objet->count,fifo->objet->debut,fifo->objet->fin);
  
  return old;
}
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*fonction qui calcule le nombre d'octets 
 (seulement ceux des messages les tailles non inclues) 
 total dans memory*/

size_t octet_msg(mfifo *fifo) {
  size_t taille_msg=(fifo->objet->fin-fifo->objet->debut)-fifo->objet->count*sizeof(size_t);
  
    if (taille_msg<0)
      return taille_msg + fifo->objet->capacity;

  return taille_msg;
}
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*fonction appelée si le processus appelant n'a pas fait de lock pour l'objet mfifo*/
size_t mfifo_read_simple(mfifo *fifo,void *buf, size_t len)
{
size_t l;
size_t k=octet_msg(fifo);
//printf("msg %zu \n",k);
//printf("len %zu \n",len);
int x=(len-k);
//printf("différence %d \n",x);

if (x>0){
   l=octet_msg(fifo);   
}
else{
   l=len;
}
  pthread_mutex_lock( & fifo->objet->read_mutex );
  while(fileVide(fifo)) {
     pthread_cond_wait( & fifo->objet->rcond ,& fifo->objet->read_mutex );
  }
//printf("avant read_mutex");
 int indexLire = majLecture(fifo,l);
 //printf("apres read_mutex");
         pthread_mutex_unlock( & fifo->objet->read_mutex );
          size_t size = lire(fifo, buf, l, indexLire);
  pthread_cond_broadcast( & fifo->objet->wcond );

  return size;
}
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*fonction appelée si le processus appelant a fait un lock pour l'objet mfifo*/
size_t mfifo_read_lock (mfifo *fifo,void *buf, size_t len)
{
  size_t nbr_caractere = octet_msg(fifo);

  if (nbr_caractere < len)
     return -1;
  
   int indexLire = majLecture(fifo,len);
   size_t size = lire(fifo, buf, len, indexLire);   

return size;
}
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*foncton mfifo_read demandée 
 fait appel a mfifo_read_somple et mfifo_read_lock
 selon est ce que le processus appelant a vérouiller l'objet mfifo 
 avant l'appel a mfifo_read ou pas
*/
size_t mfifo_read(mfifo *fifo,void *buf, size_t len)
{
   size_t size;

     if (fifo->objet->verrou == 0)
         size = mfifo_read_simple(fifo, buf,len);
     else 
       size = mfifo_read_lock(fifo, buf,len);   

return size;
}