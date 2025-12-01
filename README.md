# Final Project Struktur Data dan Analisa Algoritma Collision Simulation
Project mata kuliah Struktur Data dan Analisa Algoritma, membuat program collision bola bergerak dan memantul dengan bahasa pemrograman C++. 
Mata kuliah ini diampu oleh bapak Arta Kusuma Hernanda,S.T.,M.T

Nama : Moh. Nabil Zarif Rofif
NRP  : 5024241038

# Overview
Project ini mensimulasikan N-body physics menggunakan Euler Integration untuk pergerakan. Untuk deteksi tabrakan, saya membandingkan performa antara metode Brute Force dengan optimasi Quadtree yang membagi ruang simulasi secara rekursif untuk mengurangi jumlah pengecekan yang tidak perlu. Sesuatu yang membedakan project ini dengan collision simulation yang lain adalah saya membuat jika bola memantul pada batas vertikal maka bola akan semakin kencang, sedangkan jika memantul pada batas horizontal maka akan semakin lambat.

# Tool Yang Digunakan
- SFML 3.0.2
- C++ 17

# Algoritma Yang Digunakan
#### **Brute Force**
Algoritma ini berfungsi mengambil satu bola dan mengecek bola tersebut terhadap semua bola lainnya.

#### **Quadtree**
Algoritma ini memecah layar menjadi kotak-kotak kecil secara rekursif. Dan hanya mengambil bola yang berada di kotak yang sama, sehingga tidak perlu mengecek semua bola.





