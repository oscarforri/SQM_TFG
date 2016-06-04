#include <FreqCount.h>
#include <DHT.h>
#include <Wire.h>
#include <EEPROM.h>


#define nsec 30 //Es necesari aquest factor de escala perque el programa calcula per FreqCount.begin(1000);
#define A 20

#define DHTPIN 7
#define DHTTYPE DHT22

#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527

float count;
float freq;
float Msq;

int last_addr;
int addr;
int a;
float data_dht;
int second;
int minute;
int hour;
int weekDay;
int monthDay;
int month;
int year;
float t;
float h;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  FreqCount.begin(30000);  //Temps de mostreig, si aqui va un 1000, no es fica nsec=1, si es 30000 nsec serà 30 (factor de escala)
  dht.begin();
  Wire.begin();
  setDateTime(); //Iniciem el rellotge amb la hora que hem configurat en aquesta funció.
  last_addr = initial_addr();
}

void loop() {
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    freq = count / float (nsec);
    //freq = count; //Si temps de mostratge es 1000 es directe   
    Msq = A - 2.5 * log10(freq);
    Serial.print("Frequencia: ");
    Serial.print(freq);
    Serial.print(" Hz");
    Serial.print("    Magnitud: ");
    Serial.print(Msq);
    Serial.print(" MPSAS    ");
    data_dht = get_dht();
    printDate();
    save_data(monthDay, month, year, hour, minute, t, h, Msq);
  }
  else {
    Serial.print("Error de lectura");
  }
  delay (1000);
}

/*-----------------------------DS1307-------------------------------*/

/*Funció per ajustar la hora i començar el funcionament del rellotge*/
void setDateTime(){
  
  byte second =     15; //0-59
  byte minute =      37; //0-59
  byte hour =        11; //0-23
  byte weekDay =    7; //1-7
  byte monthDay =    22; //1-31
  byte month =       05; //1-12
  byte year  =       16; //0-99
 
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop Oscillator
 
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
 
  Wire.write(zero); //start

  Wire.endTransmission();
}

/*Conversió de numeros decimals normals a decimals codificats en binari*/
byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
  }
  
/*Conversió de decimals codificats en binari a numeros decimals normals*/
byte bcdToDec(byte val)  {
  return ( (val/16*10) + (val%16) );
  }

/*Funció per mostrar la data*/
int printDate(){
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
 
  Wire.requestFrom(DS1307_ADDRESS, 7);
 
  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
 
  //print the date EG   3/1/11 23:59:59
  Serial.print(0,0);
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(year);
  //Serial.print(0,1);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
  return monthDay, month, year,hour, minute, second;
  }
 

/*---------------------------DHT22---------------------------------*/

/*Funció per llegir temperatura i humitat del sensor DHT22*/
float get_dht(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h)){
    Serial.println("Error al llegir el sensor DHT");
    } else {
      Serial.print("Humitat: ");
      Serial.print(h);
      Serial.print("%    ");
      Serial.print("Temperatura: ");
      Serial.print(t);
      Serial.print("*C   ");
      }
  return h, t;
  }

/*---------------------SAVE DATA TO EEPROM---------------------------*/

void save_data(int monthDay, int month, int year, int hour, int minute, float t, float h, float Msq){
    addr = last_addr;
    EEPROM.write(addr, monthDay);
    next_addres();
    EEPROM.write(addr, month);
    next_addres();
    EEPROM.write(addr, year);
    next_addres();
    EEPROM.write(addr, hour);
    next_addres();
    EEPROM.write(addr, minute);
    next_addres();
    save_floatdata(t);
    next_addres();
    save_floatdata(h);
    next_addres();
    save_floatdata(Msq);
    next_addres();
    last_addr = addr;
    EEPROM.write(0,highByte(last_addr));
    EEPROM.write(1,lowByte(last_addr));
}

void next_addres(){
 addr ++;
    if (addr=1024){
      addr=3;
    }
}

void save_floatdata(float val){
 a= val * 100;
 byte lowbyte;
 byte highbyte;
 lowbyte = lowByte(a);
 highbyte = highByte(a); 
 EEPROM.write(addr, highbyte);
 addr ++;
 EEPROM.write(addr, lowbyte);
}

unsigned int initial_addr(){
  byte highByte  = EEPROM.read(0);
  byte lowByte = EEPROM.read(1);
  return ((highByte << 0) & 0xFF) + ((lowByte <<8) & 0xFF00);
}

