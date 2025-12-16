// ===== BLYNK SETUP =====
#define BLYNK_TEMPLATE_ID "TMPL6gh9G0xvO"
#define BLYNK_TEMPLATE_NAME "Health Monitor"
#define BLYNK_AUTH_TOKEN "F6BtfeFifJPDsU9dd5aZGDO-7UN-M-B-"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ====================== WIFI & BLYNK ======================
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "t";         // ubah sesuai WiFi kamu
char pass[] = "12341234";  // ubah sesuai password WiFi kamu

// ====================== PIN & SENSOR ======================
#define ONE_WIRE_BUS 4
#define PULSE_PIN 34
#define BUZZER 18
#define BUZZER_CHANNEL 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ====================== OLED ======================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ====================== VARIABEL ======================
float baseline = 0;
int threshold = 0;
unsigned long lastBeatTime = 0;
int bpm = 0;
bool pulseDetected = false;
bool heartBeat = false;
float suhuOffset = 0.3;
int frame = 0;
bool systemOn = true;

// Buzzer timing
unsigned long lastPulseTime = 0;
unsigned long lastWarningTime = 0;
unsigned long warningInterval = 200;
bool warningActive = false;

// ====================== FUNGSI RESET SISTEM ======================
void resetSystem() {
  lastBeatTime = 0;
  bpm = 0;
  pulseDetected = false;
  heartBeat = false;
  frame = 0;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 18);
  display.println("SELAMAT");
  display.setCursor(36, 38);
  display.println("DATANG");
  display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("Kalibrasi sensor...");
  display.display();

  long total = 0;
  for (int i = 0; i < 50; i++) {
    total += analogRead(PULSE_PIN);
    delay(5);
  }
  baseline = total / 50.0;

  display.clearDisplay();
  display.setCursor(30, 28);
  display.println("Selesai!");
  display.display();
  delay(500);
}

// ====================== BLYNK ON/OFF ======================
BLYNK_WRITE(V2) {
  bool newState = param.asInt();
  if (newState && !systemOn) {
    systemOn = true;
    Serial.println("Sistem: ON (Blynk)");
    resetSystem();
  } else if (!newState && systemOn) {
    systemOn = false;
    Serial.println("Sistem: OFF (Blynk)");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 28);
    display.println("SISTEM MATI");
    display.display();
    ledcWriteTone(BUZZER_CHANNEL, 0);
  }
}

// ====================== FUNGSI TAMBAHAN ======================
void drawHeart(int x, int y, bool beat) {
  int r = beat ? 7 : 5;
  display.fillCircle(x, y, r, SSD1306_WHITE);
  display.fillCircle(x + 10, y, r, SSD1306_WHITE);
  display.fillTriangle(x - 5, y + 3, x + 15, y + 3, x + 5, y + 18, SSD1306_WHITE);
}

void drawThermometer(int x, int y, float suhu) {
  display.drawRect(x, y - 20, 6, 18, SSD1306_WHITE);
  display.fillCircle(x + 3, y, 4, SSD1306_WHITE);
  int tinggi = map((int)suhu, 20, 40, 0, 16);
  tinggi = constrain(tinggi, 0, 16);
  display.fillRect(x + 1, y - tinggi, 4, tinggi, SSD1306_WHITE);
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(115200);
  sensors.begin();

  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER, BUZZER_CHANNEL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED tidak terdeteksi!");
    for (;;);
  }

  Blynk.begin(auth, ssid, pass);
  resetSystem();
}

// ====================== LOOP ======================
void loop() {
  Blynk.run();

  if (systemOn) {
    // --- Baca suhu ---
    sensors.requestTemperatures();
    float suhu = sensors.getTempCByIndex(0);
    if (suhu != DEVICE_DISCONNECTED_C) suhu += suhuOffset;
    else suhu = 0;
    // --- Pulse sensor adaptif ---
    static float smoothValue = 0;
    int rawValue = analogRead(PULSE_PIN);

    // Smoothing untuk stabilitas sinyal
    smoothValue = (smoothValue * 0.8) + (rawValue * 0.2);

    // Baseline mengikuti kondisi jari perlahan
    baseline = (baseline * 0.995) + (smoothValue * 0.005);
    threshold = baseline + 100;  // Ambang deteksi adaptif

    if (smoothValue > threshold && !pulseDetected) {
      pulseDetected = true;
      unsigned long now = millis();

      // Hitung BPM
      if (lastBeatTime > 0) {
        int interval = now - lastBeatTime;
        if (interval > 250 && interval < 2000)
          bpm = 60000 / interval;
      }
      lastBeatTime = now;
      heartBeat = true;

      // Buzzer detak jantung singkat
      ledcWriteTone(BUZZER_CHANNEL, 1500);
      lastPulseTime = now;

    } else if (smoothValue < threshold) {
      pulseDetected = false;
      heartBeat = false;
    }

    // Matikan buzzer detak setelah 80ms
    if (millis() - lastPulseTime >= 80) {
      ledcWriteTone(BUZZER_CHANNEL, 0);
    }

    // --- Warning ---
    warningActive = (suhu > 37.5 || bpm > 100);
    if (warningActive && millis() - lastWarningTime >= warningInterval) {
      ledcWriteTone(BUZZER_CHANNEL, 1500);
      lastWarningTime = millis();
    }
    if (!warningActive && millis() - lastPulseTime >= 80) {
      ledcWriteTone(BUZZER_CHANNEL, 0);
    }

    // --- OLED TAMPILAN ---
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("HEALTH MONITOR");
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    drawHeart(14, 36, heartBeat);
    drawThermometer(112, 44, suhu);

    display.setTextSize(2);
    display.setCursor(45, 16);
    display.print(bpm);
    display.setTextSize(1);
    display.print(" BPM");

    display.setTextSize(2);
    display.setCursor(45, 38);
    display.print(suhu, 1);
    display.setTextSize(1);
    display.print(" C");

    if (warningActive) {
      if ((frame / 2) % 2 == 0) {
        display.setCursor(36, 54);
        display.println("PERINGATAN!");
      }
    } else {
      display.setCursor(42, 54);
      display.println("Status: Normal");
    }

    display.display();
    frame++;

    // --- Kirim data ke Blynk ---
    static unsigned long lastSend = 0;
    if (millis() - lastSend > 500) {
      if (bpm > 0) Blynk.virtualWrite(V0, bpm);
      float suhuDisplay = round(suhu * 10) / 10.0;
      Blynk.virtualWrite(V1, suhuDisplay);
      lastSend = millis();
    }

    // --- Debug di Serial Monitor ---
    Serial.print("BPM="); Serial.print(bpm);
    Serial.print(" | Suhu="); Serial.print(suhu);
    Serial.print(" | ADC="); Serial.println(rawValue);

  } else {
    // Sistem OFF
    display.clearDisplay();
    display.display();
    ledcWriteTone(BUZZER_CHANNEL, 0);
  }
}