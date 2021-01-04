#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h> /* pour shm_open */
#include <semaphore.h>

#include <errno.h>  /* pour errno */
#include <sys/types.h> /* pour waitpid */
#include <sys/wait.h> /* pour waitpid */
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>





typedef struct fifo1{
    mode_t permission;
  	size_t capacity;
	size_t debut;
	size_t fin;
	size_t count;
	pid_t pid;

	pthread_mutex_t mutex;
    pthread_cond_t  rcond;
    pthread_cond_t  wcond;
    
    pthread_mutex_t read_mutex;
    int verrou;
	char *memory[];

}fifo1;

typedef struct mfifo {
  int option;
  fifo1 * objet;
}mfifo;

mfifo *mfifo_connect( const char *nom, int options, ...);
mfifo *connect_mfifo_anonym(int options,mode_t permission,size_t capacite );
mfifo *open_mfifo( const char *nom, int options);
mfifo *connect_mfifo_nomme( const char *nom, int options, mode_t permission,size_t capacite );

int mfifo_disconnect(mfifo *fifo);
int mfifo_unlink(const char *nom);

int mfifo_write(mfifo *fifo, const void *buf, size_t len);
int mfifo_trywrite(mfifo *fifo,const void*buf, size_t len);
int mfifo_write_partial(mfifo *fifo, const void *buf, size_t len);

size_t mfifo_read(mfifo *fifo,void *buf, size_t len);


int mfifo_lock(mfifo *fifo);
int mfifo_unlock(mfifo *fifo);
int mfifo_trylock(mfifo *fifo);

void thread_error(const char *file, int line, int code, char *txt);

//initialiser  les mutex et condition
int initialiser_mutex(pthread_mutex_t *pmutex);
int initialiser_cond(pthread_cond_t *pcond);
	



