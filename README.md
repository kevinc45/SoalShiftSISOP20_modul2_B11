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
<br/>  
  
**2: ARGC & ARGV**
```c
int main (int argc, char *argv[])
```
Saat masuk ke program, kamu pasti mendapati ada yang aneh di bagian *int main*. Yap, **argc** dan **argv**! Sebagai **penerima informasi dari *command line* menuju program**, argc dan argv berperan aktif untuk memberikan argumen yang diberikan user melalui *command line*. Informasi lengkap dapat dibaca [di sini](http://crasseux.com/books/ctutorial/argc-and-argv.html).
  
Mengutip dari situs tersebut, kira-kira seperti ini cara kerja argc dan argv:
```
For example, the command line  

gcc -o myprog myprog.c

would result in the following values internal to GCC:

  

argc

4
  

`argv[0]`

`gcc`  

`argv[1]`

`-o`  

`argv[2]`

`myprog`  

`argv[3]`

`myprog.c`
```

**argv** merupakan *array* yang berisikan *pointer of char*, dalam penerapannya, **argv** dapat dipanggil seperti *two-dimensional array*. Contohnya juga terdapat di dalam *solution code* berupa:

```c
if (argv[1][0] != '*') detik = atoi(argv[1]);
if (argv[2][0] != '*') menit = atoi(argv[2]);
if (argv[3][0] != '*') jam = atoi(argv[3]);
``` 
  
Potongan *code* di atas menunjukkan bahwa kita ingin mendeteksi *char* apa yang terdapat di kolom [1], [2], atau [3], di urutan ke-[0] atau pertama.
*Code* tersebut difungsikan untuk mengubah simbol **asterisk( * )** menjadi *integer value* melalui fungsi **atoi( )**.

*Apa yang dilakukan oleh fungsi atoi( )?*
Menurut situs [tutorialpoint.com](https://www.tutorialspoint.com/c_standard_library/c_function_atoi.htm),
```
Fungsi ini me-return integral value yang telah dikonversi sebagai int.
```
Potongan kode tersebut diperlukan untuk memudahkan perbandingan, apakah *input* yang diberikan oleh *user* berisi *asterisk* atau bukan. Apabila bukan, maka *array of char* yang didapat melalui **argv** akan dikonversikan ke dalam *int*.  
  <br/>
  
**3: CURRENT TIME**
Dengan menggunakan fungsi **```time()```** dan **```localtime()```** dalam *library* **```<time.h>```**, kita dapat menemukan waktu lokal tempat kita berada sekarang.

Pendefinisian 'detik', 'menit', dan 'jam' dengan *value* **-1** dilakukan agar inputan *asterisk* yang diberikan oleh *user* dapat dengan mudah dideteksi karena sudah bernilai *default* sebesar **-1**.
<br/>
  
**4: THE MAIN PROGRAM**
```c
while (1){
t = time(NULL);
tm = *localtime(&t);
if ((tm.tm_hour == jam || jam == -1) && (tm.tm_min == menit || menit == -1) && (tm.tm_sec == detik || detik == -1)) {
if (fork()==0)execl("/bin/bash", "bash", argv[argc-1], NULL);}
sleep(1);
}
```
Program ini akan melakukan pengecekan tiap detik, apakah waktu sekarang sama dengan waktu yang diminta oleh *user* (mirip seperti cara kerja **cron**). Oleh karena itu, *condition* yang terdapat di dalam ***if*** bernilai *true* apabila:
1. Waktunya sesuai dengan waktu sekarang, atau
2. *User* meng-*input* **asterisk ( * )**.
  
Pemberian *value* ulang terhadap variabel **t** dan **tm** dilakukan agar program tetap *update* perihal *current time* meski sedang berada di dalam ***while***.
  
Bagaimana dengan ***if (fork( ) == 0)*** ?
Fungsi **```execl```** akan membuat program berakhir saat selesai berjalan. Kita tidak ingin itu terjadi karena harusnya program kita tetap berjalan hingga *user* melakukan ***kill***.
Jadi, penggunaan *fork* tersebut diperlukan agar fungsi **```execl```** berjalan dan menyelesaikan *process child* saja.

**```sleep (1)```** digunakan untuk mengatur agar fungsi *while* berjalan setiap satu detik.  
<br/>
  
**5: INVALID INPUT**
Untuk mengantisipasi *user* yang salah memasukkan *input* sesuai dengan jumlah argumen yang diminta, diberikan:
```c
if (argc != 5) {
printf ("Invalid input\n");
return 0;
}
``` 
sebagai pendeteksi apakah jumlah *input* yang diberikan *user* tidak sama persis dengan jumlah *input* yang diminta.
Hal tersebut dapat terlihat apabila **argc** tidak bernilai 5.
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTI2NjAyMzk1NywtMTQ4MTU2MTA2NiwtMT
IzNjI3NTY3LDcyMjI5NDE3OV19
-->

# Pembahasan soal 2
> Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat sebuah program.
> a. Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].
> b. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].
c. Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
e. Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

>Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi.
Buatlah program dalam mimpi Kiwa jadi nyata!

>Catatan:
>- Tidak boleh memakai system().
>- Program utama harus ter-detach dari terminal

>Hint:
>- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran tertentu
>- Epoch Unix bisa didapatkan dari time()

## **Solution!**
Wah, Kiwa! Kamu menyusahkan saja, ya?
Tidak perlu khawatir karena permasalahannya berhasil diselesaikan (walaupun harus berkorban jam tidur).
  
Soal nomor 2 sebenarnya agak rumit karena harus menggerakkan beberapa child sekaligus. Sama halnya dengan nomor 1, kita akan menggunakan daemon untuk membuat program berjalan di *background*.
  
## A
**Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss].**

Terlihat mudah? Yap, pertama-tama, kita harus mengambil waktu saat program dijalankan. Kurang lebih caranya hampir sama dengan nomor 1.
Bedanya, disini kita perlu mengubahnya dalam format timestamp [YYYY-mm-dd_HH:ii:ss].
Untuk melakukan hal tersebut, kami menggunakan ```sprintf``` seperti dalam potongan kode berikut:
```c
sprintf(currenttime, "%d-%02d-%02d_%02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
```
Fungsi ```sprintf``` akan memasukkan string yang terdapat dalam argumen ke-2 (dalam hal ini *timestamp*), ke dalam *array* ```currenttime```.

Untuk membuat sebuah folder baru, kita dapat menggunakan fungsi
```c
execl("/bin/mkdir", "mkdir", currenttime, NULL);
```
Program lalu di-```sleep(30)``` agar berjalan setiap 30 detik.

## B
**Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].**  
  
Di modul yang pertama, kami telah belajar untuk menggunakan ```wget``` dan kami akan menggunakan cara yang sama untuk men-*download* foto-foto ini.

Menggunakan ```sprintf```, kami memiliki 2 *array* yang berisi *timestamp* dan *link* lengkap dari situs https://picsum.photos/ yang mau di-*download*.
  
Karena kita hanya butuh 20 gambar, kita akan melakukan *looping* sebanyak 20 kali untuk keseluruhan proses.
Tentu saja karena gambarnya di-*download* setiap 5 detik, kita akan menggunakan ```sleep(5)```.

## C
**Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).**  
  
Sebelum kita dapat melakukan *zip* pada folder, kita perlu menunggu agar proses yang berjalan dalam folder (proses *download*) selesai terlebih dahulu.
Hal yang sama kita lakukan saat akan men-*delete* folder yang sudah selesai di-*zip*.  Oleh karena itu, kami akan menggunakan ```while(wait(&status)>0)``` agar proses di *child* berakhir terlebih dahulu.

Setelah itu, kita tinggal menjalankan proses ```execl``` dengan isi ```/usr/bin/zip``` dan ```/bin/rm```.

## D & E
**d. Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.**  
  
**e.  Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).**

Agar program dapat dihentikan, diperlukan file ```killer.sh``` yang berjalan sesuai dengan *input* argumen yang diberikan oleh user


# Pembahasan soal 3

>Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).
a. Program buatan jaya harus bisa membuat dua direktori di
“/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
b. Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
“/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
hanya itu tugasnya.
c. Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
pengelompokan, semua file harus dipindahkan ke
“/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
“/home/[USER]/modul2/indomie/”.
d. Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
3 detik kemudian membuat file bernama “coba2.txt”.
(contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.

## **Solution!**


```
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

```

## A 
**Membuat derektori indomie kemudian membuat direktori sedaap dengan perbedaan waktu selama 5 detik** 
``` 
char *argv[] = {"mkdir", "/home/fyan/Modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
    
    sleep(5);// memberi jeda 5 detik untuk membuat direktori sedaap setelah indomie
    
 char *argv[] = {"mkdir", "/home/fyan/Modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
```
Program diatas adalah adalah program saya untuk menjalankan mkdir menggunakan exec
dimana syntax argument 
``` argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }```
dengan menuliskan nama program mkdir yang bertujuan untuk membuat direktori baru, kemudian kita isi tempat dimana 
kita create direktori sedaap dan indomie terssebut, setelah itu kita akhiri dengan NULL untuk memberi tau agar argument di dalatelah selesai.

Setelah itu kita eksekusi menggunakan exec ` execv("/bin/mkdir", argv);`
dimana kita dapat mengetahui bahwa mkdir eksekusi itu berada di /bin/mkdir ?
dengan cara mengetikkan whereis mkdir di terminal, nanti ada keterangan yang akan muncul


## B
**Meng-ekstrak file jpg yang telah di download** 

```sleep(6); //memberi jeda 1 detik setelah membuat direktori sedaap
char *exec[] = {"unzip", "/home/fyan/Modul2/jpg.zip", NULL};
execv("/usr/bin/unzip",exec);
```
Seperti yang kasus yang ada di 3(a) dengan meng-ekstrak jgp.zip menggunakan exec dan fork
dengan cara kita beri argument dulu ```char *exec[] = {"unzip", "/home/fyan/Modul2/jpg.zip", NULL};```
yang berisi unzip kemudian lokasi file zip itu di-ekstrak , lalu NULL untuk memberi bahwa argument didalamnya telah selesai.
Eksekusi command unzip sendiri terletak di /usr/bin/unzip
Untuk mengetahuinya ketikkan whereis unzip di terminal, nanti ada penjelasannya.

sleep(6) itu buat apa? agar mkdir indomie dan unzip jpg.zip tidak dieksekusi bebarengan ( selisih 1 detik )

## C dan D 
Belum Berhasil  
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTkzMDg2MjQ2OCw5Mzk1NTAzNDEsNTQxMT
kzNTczLC00MzA1Njk5ODEsOTAxMDMzNzkwXX0=
-->