#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
  pid_t child_id1,child_id2;

  child_id1 = fork();

/*  if (child_id1 < 0) {
    exit(EXIT_FAILURE);
  }*/

  if (child_id1 == 0) {
    // this is child
	if (child_id2 = fork() == 0){
    char *argv[] = {"mkdir", "/home/fyan/Modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
}

 	 if (child_id2 = fork() == 0) {
    //this is parents
	sleep(5);// memberi jeda 5 detik untuk membuat direktori sedaap setelah indomie
    char *argv[] = {"mkdir", "/home/fyan/Modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }
}



	if (child_id2 = fork() == 0){
	sleep(6); //memberi jeda 1 detik setelah membuat direktori sedaap
char *exec[] = {"unzip", "/home/fyan/Modul2/jpg.zip", NULL};
execv("/usr/bin/unzip",exec);
}



return 0;

}



