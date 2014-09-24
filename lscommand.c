#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>    
#include <pwd.h>  
#include <grp.h>  
#include <time.h>  
#include <locale.h>  
#include <langinfo.h>  
#include <stdint.h>  
#include <fcntl.h>

#define FALSE 0
#define TRUE !FALSE

extern  int alphasort();                        //Inbuilt sorting function
static char perms_buff[30];
char pathname[MAXPATHLEN];



void die(char *msg)
{
  perror(msg);
  exit(0);
}



int file_selecto(struct direct   *entry)                             /*function to select other than hidden files ex: .c, .h, .o */
{
     char *ptr;
     char *rindex(const char *s, int c );
     if ((strcmp(entry->d_name, ".")== 0) ||(strcmp(entry->d_name, "..") == 0))  return (FALSE);
 
				                                                         
     ptr = rindex(entry->d_name, '.');
     if ((ptr != NULL) && ((strcmp(ptr, ".c") == 0) ||(strcmp(ptr, ".h") == 0) ||(strcmp(ptr, ".o") == 0) ))
	return (TRUE);
     else
	return(FALSE);
}
 

  
const char *get_perms(mode_t mode)                      /* mode type */
{  
       char ftype = '?';  
  
       if (S_ISREG(mode)) ftype = '-';                  /*checking the flags type */
       if (S_ISLNK(mode)) ftype = 'l';  
       if (S_ISDIR(mode)) ftype = 'd';  
       if (S_ISBLK(mode)) ftype = 'b';  
       if (S_ISCHR(mode)) ftype = 'c';  
       if (S_ISFIFO(mode)) ftype = '|';  
  
       sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c", ftype,  
       mode & S_IRUSR ? 'r' : '-',                                           /* readable */
       mode & S_IWUSR ? 'w' : '-',                                           /* writable */
       mode & S_IXUSR ? 'x' : '-',                                           /* excutable */ 
       mode & S_IRGRP ? 'r' : '-',                                           
       mode & S_IWGRP ? 'w' : '-',  
       mode & S_IXGRP ? 'x' : '-',  
       mode & S_IROTH ? 'r' : '-',  
       mode & S_IWOTH ? 'w' : '-',  
       mode & S_IXOTH ? 'x' : '-',  
       mode & S_ISUID ? 'U' : '-',                                           
       mode & S_ISGID ? 'G' : '-',  
       mode & S_ISVTX ? 'S' : '-');  
  
       return (const char *)perms_buff;  
}   
  
   
static int  
one (const struct dirent *unused)  
{  
              return 1;  
} 


int file_select(struct direct *entry)                                     /*selection of hidden files */
{
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return (FALSE);
    else
        return (TRUE);
}


int excecute_a()                                                            /* function for excecuting ls-a */
{
        int count,i;
        struct direct **files;

        if(!getcwd(pathname, sizeof(pathname)))
        die("Error getting pathname\n");

        printf("Current Working Directory = %s\n",pathname);
        count = scandir(pathname, &files, file_select, alphasort);

                                                                            /* If no files found, make a non-selectable menu item */
        if(count <= 0)
        die("No files in this directory\n");
    
        printf("Number of files = %d\n",count);
        for (i=1; i<count+1; ++i)
        printf("%s\n ",files[i-1]->d_name);
        printf("\n"); /* flush buffer */
        exit(0);
        }

int excecute_l()                                                          /* function for excecuting ls -l */
{             
	int count,i;  
        struct direct **files;  
        struct stat statbuf;  
        char datestring[256];  
        struct passwd pwent;  
        struct passwd *pwentp;  
        struct group grp;  
        struct group *grpt;  
        struct tm time;  
        char buf[1024];  
  
      
  
        count = scandir(pathname, &files, file_selecto, alphasort);  

        
  
        if(count > 0)  
           {  
               printf("total %d\n",count);  
               printf("pathname name is = %s\n" , pathname);
                  for (i=0; i<count; ++i)  
                   {                                  
  
                    if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))  
                            printf(" %s", pwent.pw_name);  
                    else  
                            printf(" %d", statbuf.st_uid);                                       /* User ID of the file's owner */
  
                    if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))  
                            printf(" %s", grp.gr_name);  
                    else  
                            printf(" %d", statbuf.st_gid);                                       /* Group ID of the file's group */  
  
                                                                                                 /* Print size of file. */  
                            printf(" %5d", (int)statbuf.st_size);  
                                                                                  
                            localtime_r(&statbuf.st_mtime, &time);                               /* Time of last data modification */
                                                                                                 /* Get localized date string. */  
                            strftime(datestring, sizeof(datestring), "%F %T", &time);  
  
                            printf(" %s %s\n", datestring, files[i]->d_name);  
                                                                           //}  
  
                            free (files[i]);  
                     }  
  
                            free(files);  
           }                                                                                               
                            exit(0);
 }
int excecute_lp()                                                          /* function for excecuting ls -l */
{             
	int count,i;  
        struct direct **files;  
        struct stat statbuf;  
        char datestring[256];  
        struct passwd pwent;  
        struct passwd *pwentp;  
        struct group grp;  
        struct group *grpt;  
        struct tm time;  
        char buf[1024];  
  
        if(!getcwd(pathname, sizeof(pathname)))  
             die("Error getting pathnamen");  
  
        count = scandir(pathname, &files, file_selecto, alphasort);  
  
        if(count > 0)  
           {  
               printf("total %d\n",count);  
  
                  for (i=0; i<count; ++i)  
                   {  
                     if (stat(files[i]->d_name, &statbuf) == 0)  
                         {  
                                                                                            /* Print out type, permissions, and number of links. */  
                            printf("%10.10s", get_perms(statbuf.st_mode));                      /* File mode (type, perms) */       
                            printf(" %d", statbuf.st_nlink);                                     /* Number of links */
  
                    if (!getpwuid_r(statbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))  
                            printf(" %s", pwent.pw_name);  
                    else  
                            printf(" %d", statbuf.st_uid);                                       /* User ID of the file's owner */
  
                    if (!getgrgid_r (statbuf.st_gid, &grp, buf, sizeof(buf), &grpt))  
                            printf(" %s", grp.gr_name);  
                    else  
                            printf(" %d", statbuf.st_gid);                                       /* Group ID of the file's group */  
  
                                                                                                 /* Print size of file. */  
                            printf(" %5d", (int)statbuf.st_size);  
                                                                                  
                            localtime_r(&statbuf.st_mtime, &time);                               /* Time of last data modification */
                                                                                                 /* Get localized date string. */  
                            strftime(datestring, sizeof(datestring), "%F %T", &time);  
  
                            printf(" %s %s\n", datestring, files[i]->d_name);  
                         }  
  
                            free (files[i]);  
                   }  
  
                            free(files);  
           }                                                                                               
                            exit(0);
}
   

int execute_ls()
{
    	int count,i;
    	struct direct **files;

    	if(!getcwd(pathname, sizeof(pathname)))                                      
        die("Error getting pathname\n");

    	printf("Current Working Directory = %s\n",pathname);
    	count = scandir(pathname, &files,file_selecto,  alphasort);

    	/* If no files found, make a non-selectable menu item */
    	if(count <= 0)
        die("No files in this directory\n");
    
    	printf("Number of files = %d\n",count);
    	for (i=1; i<count+1; ++i)                                                  /*print the files in the current directory */
        printf("%s\n  ",files[i-1]->d_name);
    	printf("\n"); /* flush buffer */
        return 0;

}


int main(int argc, char* argv[])
{
      
      if (argc ==2)
              {
                  if (!strcmp(argv[1], "-a"))                                       /* if user selict "-a" */
                       {                                                    
                          excecute_a();
                       }

                 if (!strcmp(argv[1], "-l"))                                        /* if user selict "-l" */
                       {
                          excecute_lp();                                    
                       }
                                         }
                 if (argc == 3)
                       {
                           strcpy(pathname, argv[2]);
                            excecute_l(); 
                       }
                                                 

 
      if (argc == 1)                                    /*if there is no argument is specify*/
  
      execute_ls();
}
    
                 
