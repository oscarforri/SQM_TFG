#include <EEPROM.h>
#include <DHT.h>

#define DHTTYPE DHT11 
#define DHTPIN 3

DHT dht(DHTPIN, DHTTYPE);

int addr = 0;
float t;
float h;
byte value;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  
  int val = analogRead(0) / 4;

  Serial.print("Temp=");
  Serial.print(t);
  Serial.println(" *C");
  Serial.print("Humidity=");
  Serial.print(h);
  Serial.println("% ");
  delay(2000);  

  save_t();
  read_data();
  }

void save_t(){
  EEPROM.write(addr, t);
  addr = addr + 1;
  if (addr == EEPROM.length()) {      //last address Arduino Uno --> 1023 (1kB EEPROM storage)
    addr = 0;
    }
  delay(100);
  Serial.println("Save t correctly!");
  }

void read_data(){
  int index = 0;
  while (index <= EEPROM.length()) {
    value = EEPROM.read(index);
    Serial.print(value);
    Serial.print("//");
    EEPROM[ index ] += 1;
    index++;
    delay(10);
//
    }
  }
