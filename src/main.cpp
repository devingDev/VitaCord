#define VITASDK

#include "DiscordApp.hpp"
#include <pthread.h>
#include <debugnet.h>



#define DEBUGNET_IP "192.168.1.85"
#define DEBUGNET_PORT 12131
/* debug types
NONE 0
INFO 1
ERROR 2
DEBUG 3
debugNetPrintf(ERROR, "Test error level %d\n", ret);
*/




extern "C"
{
    unsigned int sleep(unsigned int seconds)
    {
        sceKernelDelayThread(seconds*1000*1000);
        return 0;
    }

    int usleep(useconds_t usec)
    {
        sceKernelDelayThread(usec);
        return 0;
    }

    void __sinit(struct _reent *);
}
__attribute__((constructor(101)))
void pthread_setup(void) 
{
    pthread_init();
    __sinit(_REENT);
}
int main(){
	debugNetInit(DEBUGNET_IP, DEBUGNET_PORT, DEBUG);
	
	DiscordApp discordapp;
	discordapp.Start();
	
	
	
	
	return 0;
}



