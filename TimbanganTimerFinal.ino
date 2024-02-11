#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DoutPin 2                                           // Pin DT/Dout HX711 module Loadcell
#define clkPin   3                                          // Pin CLK HX711 Module LoadCell
#define tareButtonPin 9                                     // Pin Tare timbangan
#define timerButtonPin 10                                   // Pin Timer 
#define buzzPin 8                                           // Pin Buzzer

#define Kalibrasi 330.09615384                              // Faktor Kalibrasi

int bacaanBerat;                                            // Variabel untuk menyimpan nilai Berat dan menampilkan di layar LCD
unsigned long simpanWaktu = 0;                              // Variabel untuk menyimpan waktu dalam Millis()
unsigned long waktuBerjalan = 0;                            // Variabel untuk menghitung waktu berjalan sejak tombol timer ditekan, rumusnya waktuBerjalan = Millis() - simpanWaktu
bool timerBool = false;                                     // Boolean Timer

LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;

void setup() {
  Serial.begin(9600);
  pinMode(tareButtonPin, INPUT_PULLUP);                     // Deklarasi pin Tare, Timer dan Buzzer
  pinMode(timerButtonPin, INPUT_PULLUP);
  pinMode(buzzPin, OUTPUT);
  
  lcd.init();                                               // Inisialisasi LCD
  lcd.backlight();                                          // Fungsi untuk menyalakan LED LCD display

  scale.begin(DoutPin, clkPin);                             // Inisialisasi module HX7111 Loadcell
  scale.set_scale(Kalibrasi);                               // Kalibrasi timbangan
  scale.tare();                                             // men tare timbangan (angka menjadi mulai dari 0)
}

void loop() {

  if (digitalRead(tareButtonPin) == LOW) {                  // Fungsi Tombol tare, jika tombol ditekan, berapapun berat objek di timbangan, bacaan 
    scale.tare();                                           // akan kembali ke 0
  }

  bacaanBerat = round(scale.get_units());                   // Fungsi untuk mendapatkan nilai bacaan Loadcell dan menyimpannya di variabel bacaanBerat
  tampillcdBerat(bacaanBerat);                              // Menampilkan berat pada LCD Display

  if (digitalRead(timerButtonPin) == LOW) {                 // Fungsi Timer, jika tombol Timer ditekan, maka waktu akan berjalan
    if (!timerBool) {
      simpanWaktu = millis();
      timerBool = true;
    } else {                                                // Jika tombol ditekan untuk kedua kalinya maka waktu akan berhenti
      timerBool = false;
      waktuBerjalan = 0;
    }
  }

  if (timerBool) {                                          // Fungsi Timer, jika tombol Timer ditekan, maka waktu akan berjalan                       
    waktuBerjalan = (millis() - simpanWaktu) / 1000;
    tampillcdTimer(waktuBerjalan);
    if(waktuBerjalan == 30){                                // Fungsi Buzzer, Total lama waktu membuat Kopi dengan metode V60 adalah
      Buzzer();                                             // 2 menit 30 detik atau 150 detik, dengan jeda penuangan setiap 30, 120 dan 150 detik.
    } else if (waktuBerjalan == 120) {                      // dalam pembuatan V60 signature orang berbeda2, silahkan edit timer Buzzer sesuai
      Buzzer();                                             // keinginan.
    } else if (waktuBerjalan == 150) {
      Buzzer();
    } else {
     noTone(buzzPin);
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}

void tampillcdBerat(float bacaanBerat) {                    // Fungsi untuk menampilkan Berat Objek ke LCD Display
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Berat: ");
  lcd.print(bacaanBerat, 0);
  lcd.setCursor(15, 0);
  lcd.print("g");
}

void tampillcdTimer(unsigned long elapsedTime) {            // Fungsi untuk menampilkan waktu yang berjalan/Timer ke LCD Display
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(elapsedTime);
  lcd.print(" s");
}

void Buzzer(){                                              // Fungsi Buzzer
  tone(buzzPin,523,3000);
}
