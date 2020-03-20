#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int main(){
  pid_t child_id1,child_id2;

  child_id1 = fork();

  if(child_id1 == 0){
    if(child_id2 = fork() == 0){
      char *argv[] = {"mkdir","/home/fyan/Modul2/indomie",NULL};
      execv("/bin/mkdir",argv);
    }
    sleep(5);
    if(child_id2 = fork() == 0){
      char *argv[] = {"mkdir","/home/fyan/Modul2/sedaap",NULL};
      execv("/bin/mkdir",argv);
    }
    if(child_id2 = fork() == 0){
      chdir("/home/fyan/Modul2");
      char *exc[] = {"unzip","jpg.zip",NULL};
      execv("/usr/bin/unzip",exc);
    }
  }



sleep(2);
struct dirent *der;
DIR *dir = opendir("/home/fyan/Modul2/jpg");
if(dir == NULL)
{
return 0;
}

while((der = readdir(dir))!= NULL)
{
	
	char filepath[100];
	struct stat typestat;
		strcpy(filepath,"/home/fyan/Modul2/jpg/");
		strcat(filepath,der->d_name);
		printf("%s \n",filepath);
		if(stat(filepath,&typestat) == 0)
		{
			if(typestat.st_mode & S_IFDIR)
			{
				if(child_id1 = fork() == 0)
				{
					char *argv[] = {"mv",filepath,"/home/fyan/Modul2/indomie",NULL};
					execv("/bin/mv",argv);
				}
			}
			else if (typestat.st_mode & S_IFREG)
			{
				
				if (child_id1 = fork() == 0)
				{
					char *argv[] = {"mv",filepath,"/home/fyan/Modul2/sedaap",NULL};
					execv("/bin/mv",argv);
				}
			}
		}
	

}
closedir(dir);
}
