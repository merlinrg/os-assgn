#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(void)
{
    char *curr_dir = NULL;
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    unsigned int count = 0;

    curr_dir = getenv("PWD");
     
    if(curr_dir == NULL)
    {
        printf("\n ERROR : Could not get the working directory\n");
        return -1;
    }

    dp = opendir((const char*)curr_dir);
    if(dp == NULL)
    {
        printf("\n ERROR : Could not open the working directory\n");
        return -1;
    }

     for(count = 0; (dptr = readdir(dp)) != NULL ; count++)
    {
        if(dptr->d_name[0]!='.')
        printf("%s  ",dptr->d_name);
    }
    printf("\n %u", count);
    return 0;
}
