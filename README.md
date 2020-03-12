# SoalShiftSISOP20_modul2_B11
## Kelompok
 - Achmad Sofyan Pratama (05111840000061)
 - Kevin Christian Hadinata (05111840000066)


# Pembahasan soal 1
>Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:  
a. Program menerima 4 argumen berupa:  
i. Detik: 0-59 atau * (any value)  
ii. Menit: 0-59 atau * (any value)  
iii. Jam: 0-23 atau * (any value)  
iv. Path file .sh  
b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai  
c. Program hanya menerima 1 config cron  
d. Program berjalan di background (daemon)  
e. Tidak boleh menggunakan fungsi system()  
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.

## **Solution!**
Hmm, suatu program cron tanpa cron? Seharusnya bukan masalah.
  
Karena soal nomor 1  merupakan suatu kesatuan, lebih baik kita lihat langsung program lengkapnya.

```c
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
```

Cukup panjang dan membingungkan? Tenang saja! Bisa dicermati satu-persatu, kok!

**1 : FUNGSI DAEMON**
```c

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
```
Penjelasan lengkap mengenai Daemon dapat kalian baca [di link berikut](https://github.com/AZakyH/sisop-modul-2). Selebihnya, menurut sumber [Wikipedia](https://en.wikipedia.org/wiki/Daemon_(computing)),
```
Daemon adalah program komputer yang berjalan sebagai background process, dibanding harus berada dalam kontrol langsung oleh pengguna secara interaktif.
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTcyOTE5Mzc4MCwtMTIzNjI3NTY3LDcyMj
I5NDE3OV19
-->