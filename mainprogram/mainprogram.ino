#include <Arduino.h>
#include <FreqCounter.h>
#include <Math.h>

//Definició de pins
# define TSL235 5


//Constants
int period = 2000;
float area = 0.0135; //TSL235 area    TSL237 --> 0.0092
float A = 22;

unsigned long frequency;
float irradiance;
float mag;
long pulses;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TSL235, INPUT);
  Serial.println("Provant el TSL235");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("frequency: ");
  Serial.print(get_frequency(), DEC);
  Serial.print("  Hz");
  Serial.print("irradiance: ");
  Serial.print(get_irradiance(), DEC);
  Serial.print("  uW/cm2");
  Serial.print("magnitud: ");
  Serial.print(get_mag(), DEC);
  Serial.println("  MPSAS");

  delay(1500);
  
}

//Get frequency whith FreqCounter library
long get_frequency() {
  FreqCounter::f_comp = 0; // Cal Value / Calibrate with professional Freq Counter
  FreqCounter::start(period);  // Gate Time
  while (FreqCounter::f_ready == 0)
    pulses = FreqCounter::f_freq;
  delay(20);
  frequency = (pulses*1000)/(period*area);
  return frequency;
}

//Frequency to Irradiance
long get_irradiance(){
  //irradiance = (0.5037 * frequency - 0.1737)*0.95;
  irradiance = (frequency / 1000)*0.5*0.95;
  return (irradiance);
}

//Irradiance to Magnitud
float get_mag(){
  mag = A - 2.5 * log10(irradiance);
  return (mag);
}



