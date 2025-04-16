#include <sys/time.h>

double framerate_get_ticks()
{
    struct timeval	tv;
	
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}
