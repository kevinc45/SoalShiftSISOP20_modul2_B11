#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <syslog.h>
#include <wait.h>
#include <string.h>

static void daemonize(){
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) exit(EXIT_FAILURE);
	if (pid > 0) exit(EXIT_SUCCESS);
	umask(0);
	sid = setsid();
	if (sid < 0) exit(EXIT_FAILURE);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}


int main(int argc, char* argv[]){
	int mode,count;
	char currenttime[35], fname[35], link[50], zipper[35];
	int status1, status2, status3;

	if (argc!=2){
		printf("Jumlah masukan salah!\n");
		return 0;
	}

	if(argv[1][1] == 'a') mode=1;
	else if(argv[1][1] == 'b') mode=0;
	else {
		printf ("Argumen salah!\n");
		return 0;
	}
	
	daemonize();
	
	int stat;
	FILE* killer;
	killer = fopen("killer.sh", "w");
	fprintf(killer, "#!/bin/bash\nkill %d\necho \'Program berhasil dimatikan\'\nrm \"$0\"", getpid());
	fclose(killer);
	pid_t child;
	child = fork();
	if(child < 0) exit(EXIT_FAILURE);
	if(child == 0) execl("/bin/chmod", "chmod", "u+x", "killer.sh", NULL);
	while(wait(&stat) > 0);

	for(;;){
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		
		sprintf(currenttime, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		pid_t child=fork();
		if(child < 0) exit(EXIT_FAILURE);
		if(child == 0) execl("/bin/mkdir", "mkdir", currenttime, NULL);
		wait(&status1);

		child = fork();
		if (child<0) exit(EXIT_FAILURE);
		if (child == 0){
			if (mode) prctl(PR_SET_PDEATHSIG, SIGHUP);
			chdir(currenttime);
			count = 20;
			while (count--){
				time_t t2 = time(NULL);
				struct tm tm = *localtime(&t2);
				sprintf (fname, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
				sprintf (link, "https://p...content-available-to-author-only...m.photos/%ld", (t2 % 1000) + 100);

				pid_t child3 = fork();
				if (child3<0) exit(EXIT_FAILURE);
				if (child3 == 0){
					execl("/usr/bin/wget", "wget", link, "-O",fname, "-o", "/dev/null", NULL);
					exit(EXIT_SUCCESS);
				}
				sleep(5);
			}
			
			while(wait(&status2)>0);
			strcpy(zipper, currenttime);
			strcat(zipper, ".zip");		
			chdir("..");
			
			pid_t child4 = fork();
			if (child4<0) exit(EXIT_FAILURE);
			if (child4 == 0) execl("/usr/bin/zip", "zip", "-r", zipper, currenttime, NULL);

			while (wait(&status3)>0);
			execl("/bin/rm", "rm", "-rf", currenttime, NULL);
			
		}
		sleep(30);
	}
}
