# Final Project Struktur Data dan Analisa Algoritma Collision Simulation
Project mata kuliah Struktur Data dan Analisa Algoritma, membuat program collision bola bergerak dan memantul dengan bahasa pemrograman C++. 
Mata kuliah ini diampu oleh bapak Arta Kusuma Hernanda,S.T.,M.T

Nama : Moh. Nabil Zarif Rofif
NRP  : 5024241038

# Overview
Project ini mensimulasikan N-body physics menggunakan Euler Integration untuk pergerakan. Untuk deteksi tabrakan, saya membandingkan performa antara metode Brute Force dengan optimasi Quadtree yang membagi ruang simulasi secara rekursif untuk mengurangi jumlah pengecekan yang tidak perlu. Sesuatu yang membedakan project ini dengan collision simulation yang lain adalah saya membuat jika bola memantul pada batas vertikal maka bola akan semakin kencang, sedangkan jika memantul pada batas horizontal maka akan semakin lambat.

## Tool Yang Digunakan
- SFML 3.0.2
- C++ 17

## Algoritma Yang Digunakan
#### **Brute Force**
Algoritma ini berfungsi mengambil satu bola dan mengecek bola tersebut terhadap semua bola lainnya.

#### **Quadtree**
Algoritma ini memecah layar menjadi kotak-kotak kecil secara rekursif. Dan hanya mengambil bola yang berada di kotak yang sama, sehingga tidak perlu mengecek semua bola.

## Penjelasan Penggunaan Algoritma
| Metode | Kompleksitas Waktu | Deskripsi |
| :--- | :--- | :--- |
| **Brute Force** | $O(N^2)$ | Membandingkan semua pasangan partikel yang ada ($\frac{N(N-1)}{2}$ cek per *frame*).  |
| **Quadtree** | $O(N \log N)$ | Membagi ruang menjadi node rekursif untuk mempercepat pencarian partikel, dan hanya memeriksa partikel di area terdekat/diminta.  |

## How To Switch Algorythm?
Untuk project kali ini, saya menggunakan tombol spasi untuk switch antar algoritma. Baik untuk quadtree ke bruteforce dan sebaliknya, bruteforce ke quadtree.

## Kesimpulan
Proyek simulasi *N-body collision* ini mendemonstrasikan efektivitas struktur data spasial dalam menangani interaksi banyak objek menggunakan C++ dan SFML 3.0. Melalui perbandingan *real-time*, algoritma **Quadtree** terbukti jauh lebih unggul dalam menjaga stabilitas performa dengan kompleksitas rata-rata $O(N \log N)$, berbeda signifikan dengan metode **Brute Force** ($O(N^2)$) yang mengalami penurunan *frame rate* drastis saat jumlah objek meningkat. Selain menerapkan logika fisika dinamis pada dinding pembatas, proyek ini menyimpulkan bahwa implementasi partisi ruang (*spatial partitioning*) adalah solusi esensial untuk optimasi komputasi dalam pengembangan simulasi grafis atau *game engine*.

# Lampiran
---
## Kode Algoritma Bruteforce
<img width="587" height="166" alt="Image" src="https://github.com/user-attachments/assets/7f4eb530-9b8a-4a59-ae2e-2ce372783bb0" />

## Kode Algoritma Quadtree
### Bagian Logika
<img width="734" height="543" alt="Image" src="https://github.com/user-attachments/assets/faa8fbae-aeed-4228-a598-e971933deee5" />

<img width="627" height="502" alt="Image" src="https://github.com/user-attachments/assets/f3737495-921d-4163-9980-ceef77307813" />

<img width="491" height="348" alt="Image" src="https://github.com/user-attachments/assets/2cbc40ee-ec99-4ddc-a706-5c50c4f220db" />

### Bagian Eksekusi
<img width="546" height="307" alt="Image" src="https://github.com/user-attachments/assets/135a1a40-d903-451a-87cf-2bec905cd514" />










