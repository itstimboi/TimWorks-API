#include <TWAPI/TWAPI.h>

int TWAPI_Init( void )
{
    return TWAPI_AudioInit();
}

void TWAPI_ShutDown( void )
{
    TWAPI_AudioShutdown();
}