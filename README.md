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
<br/>
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEyMzYyNzU2Nyw3MjIyOTQxNzldfQ==
-->