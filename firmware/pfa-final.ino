#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PN532.h>
#include <ESP32Servo.h>

// ---------------- DHT ----------------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Relais ----------------
#define RELAY_PUMP 23
#define RELAY_FAN 19

// ---------------- Soil ----------------
#define SOIL_PIN 34

// ---------------- Limits ----------------
float TEMP_LIMIT = 30.0;
int SOIL_LIMIT = 2000;

// ---------------- PN532 ----------------
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// ---------------- Servo ----------------
Servo doorServo;
#define SERVO_PIN 18

// ---------------- Access ----------------
uint8_t allowedUID[] = {0x04, 0xA1, 0xB2, 0xC3};
int uidLengthRef = 4;

// ---------------- Timer door ----------------
unsigned long doorOpenTime = 0;
bool doorOpen = false;

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);

  digitalWrite(RELAY_PUMP, HIGH);
  digitalWrite(RELAY_FAN, HIGH);

  // ---------------- Servo : tourne UNE SEULE FOIS ici ----------------
  doorServo.attach(SERVO_PIN);
  doorServo.write(150);      // position fermée
  delay(100);              // laisser le temps au servo de se placer
  doorServo.write(90);    // ouvrir
  delay(100);
  doorServo.detach();      // libère le servo après usage
  // -------------------------------------------------------------------

  // RFID
  nfc.begin();

  if (!nfc.getFirmwareVersion()) {
    Serial.println("PN532 NOT FOUND");
    while (1);
  }

  nfc.SAMConfig();
}

// ---------------- UID CHECK ----------------
bool checkUID(uint8_t *uid, uint8_t len) {
  if (len != uidLengthRef) return false;

  for (int i = 0; i < len; i++) {
    if (uid[i] != allowedUID[i]) return false;
  }
  return true;
}

// ---------------- LOOP ----------------
void loop() {

  // --------- DHT ----------
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // --------- Soil ----------
  int soil = analogRead(SOIL_PIN);

  // --------- FAN ----------
  digitalWrite(RELAY_FAN, (temp > TEMP_LIMIT) ? LOW : HIGH);

  // --------- PUMP ----------
  digitalWrite(RELAY_PUMP, (soil < SOIL_LIMIT) ? LOW : HIGH);

  // --------- RFID ----------
  uint8_t uid[7];
  uint8_t uidLength;

  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength
  );

  if (success && checkUID(uid, uidLength)) {
    Serial.println("ACCESS GRANTED");
    // Plus de contrôle servo ici — il a déjà tourné au démarrage
    doorOpen = true;
    doorOpenTime = millis();
  }

  // --------- LCD ----------
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print(" H:");
  lcd.print(hum);

  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print(soil);

  delay(500);
}
