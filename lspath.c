#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include<unistd.h>
#include<sys/stat.h>
int main(int argc, char* argv[])
{
   
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;
    struct stat mystat;


    dp=opendir(argv[1]);

     for(count = 0; (dptr = readdir(dp)) != NULL ; count++)
    { 
        stat(dptr->d_name,&mystat);
        if(dptr->d_name[0]!='.')
        printf("%s \n ",dptr->d_name);
    }
    closedir(dp);
}

