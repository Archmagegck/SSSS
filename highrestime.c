#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/unistd.h>
inline unsigned long long GetNTime()
{
        __asm ("RDTSC");
}

int main()
{
        long long time = GetNTime();
        printf( "%qi\n",time );
        return 0;
}
