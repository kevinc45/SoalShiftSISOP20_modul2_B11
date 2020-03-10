#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

static void daemonize(){
pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}


int main (int argc, char *argv[]){

daemonize();

time_t t = time(NULL);
struct tm tm = *localtime(&t);
int detik = -1;
int menit = -1;
int jam = -1;

if (argc != 5) {
printf ("Invalid input\n");
return 0;
}

if (argv[1][0] != '*') detik = atoi(argv[1]);
if (argv[2][0] != '*') menit = atoi(argv[2]);
if (argv[3][0] != '*') jam = atoi(argv[3]);

while (1){
t = time(NULL);
tm = *localtime(&t);
if ((tm.tm_hour == jam || jam == -1) && (tm.tm_min == menit || menit == -1) && (tm.tm_sec == detik || detik == -1)) {
if (fork()==0)execl("/bin/bash", "bash", argv[argc-1], NULL);}
sleep(1);
}
}
