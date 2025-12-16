# IoT_HealthMonitor_ESP32

## Deskripsi Proyek

IoT_HealthMonitor_ESP32 merupakan proyek Internet of Things (IoT) berbasis mikrokontroler ESP32 yang dirancang untuk melakukan pemantauan kondisi kesehatan secara real-time. Sistem ini mampu membaca data fisiologis berupa detak jantung dan suhu tubuh menggunakan sensor kesehatan, kemudian menampilkan hasilnya melalui layar OLED.

Proyek ini disusun untuk memenuhi tugas mata kuliah Internet of Things (IoT) serta diikutsertakan dalam lomba IoT yang diselenggarakan oleh HMJ Informatika ITB Widya Gama Lumajang.

---

## Latar Belakang

Pemanfaatan teknologi IoT di bidang kesehatan memungkinkan proses monitoring kondisi tubuh dilakukan secara lebih efektif dan efisien. Dengan dukungan ESP32 yang memiliki kemampuan pemrosesan data serta konektivitas nirkabel, sistem monitoring kesehatan dapat dikembangkan sebagai solusi sederhana namun fungsional untuk pemantauan kondisi vital manusia.

---

## Tujuan Proyek

- Mengimplementasikan konsep Internet of Things (IoT) menggunakan ESP32
- Merancang sistem monitoring detak jantung dan suhu tubuh
- Menampilkan data kesehatan secara real-time
- Menjadi media pembelajaran penerapan IoT di bidang kesehatan

---

## Komponen Utama

1. **ESP32**  
   Berfungsi sebagai pusat pengendali sistem yang bertugas membaca data dari sensor, memproses data, serta mengendalikan output sistem.

2. **Pulse Sensor**  
   Digunakan untuk mendeteksi detak jantung pengguna dengan membaca sinyal denyut nadi.

3. **Sensor Suhu DS18B20**  
   Digunakan untuk mengukur suhu tubuh secara digital dengan tingkat akurasi yang baik.

4. **OLED I2C**  
   Berfungsi sebagai media tampilan untuk menampilkan informasi detak jantung dan suhu tubuh secara real-time.

---

## Komponen Pendukung

- **Buzzer**  
  Digunakan sebagai indikator suara atau peringatan tertentu pada sistem.

- **Breadboard**  
  Media perakitan rangkaian sementara tanpa perlu melakukan penyolderan.

- **Kabel Jumper**  
  Digunakan untuk menghubungkan antar komponen dalam rangkaian.

- **Resistor**  
  Berfungsi untuk membatasi arus listrik agar komponen bekerja dengan aman.

- **Kapasitor**  
  Digunakan untuk menstabilkan tegangan dan mengurangi noise pada rangkaian.

---

## Cara Kerja Sistem

1. Pulse Sensor membaca sinyal detak jantung pengguna.
2. Sensor DS18B20 membaca suhu tubuh.
3. ESP32 menerima dan memproses data dari kedua sensor.
4. Hasil pengolahan data ditampilkan pada layar OLED.
5. Buzzer dapat aktif sebagai indikator sesuai kondisi tertentu.

---

## Perangkat Lunak (Software)

- Arduino IDE
- Board ESP32
- Library Pulse Sensor
- Library OneWire dan DallasTemperature (DS18B20)
- Library OLED (I2C)

---

## Struktur Folder

IoT_HealthMonitor_ESP32/
│── src/ # Source code program
│── docs/ # Dokumentasi proyek
│── README.md # Dokumentasi utama repository

---

## Cara Menjalankan Program

1. Rakit seluruh komponen sesuai dengan perancangan sistem.
2. Hubungkan ESP32 ke komputer.
3. Buka Arduino IDE.
4. Pastikan board ESP32 dan library telah terpasang.
5. Unggah program ke ESP32.
6. Amati data detak jantung dan suhu pada layar OLED.

---

## Tim Pengembang

- Akbar Ivan Jaelani (222140013)
- Ahmad Hidayatul Kabir Ali (222140079)
- Agung Budi Prasetyo (222140047)
- M. Ufri Hidayatullah (222140089)
- Muhammad Salman Farisi (222140058)
- Muhammad Anyq (222140063)

---

## Penutup

Proyek IoT_HealthMonitor_ESP32 merupakan implementasi penerapan konsep Internet of Things (IoT) dalam bidang kesehatan dengan memanfaatkan mikrokontroler ESP32 dan sensor fisiologis. Melalui perancangan dan implementasi sistem ini, diharapkan dapat memberikan pemahaman praktis mengenai integrasi perangkat keras, pemrosesan data sensor, serta penyajian informasi kesehatan secara real-time. Dokumentasi ini diharapkan dapat menjadi referensi pembelajaran dan dasar pengembangan sistem monitoring kesehatan berbasis IoT di masa mendatang.
