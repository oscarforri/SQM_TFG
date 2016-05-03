//Pin definitions
# define TSL235 2
# define LEDpin 13

//Constants
int  period = 2000;
int ScalingFactor = 1;
float area = 0.0092;
float MagSol = -26.64;

//Variables
unsigned long counter = 0;
volatile unsigned long pulses = 0;
unsigned long frequency;
float irradiance;
long long brillo;
unsigned long startTime;
unsigned long currentTime;
unsigned long des;

void setup(){
  Serial.begin(9600);
  pinMode(TSL235,INPUT);
  Serial.println("Provant el TSL235");
  Serial.println("-----------------------------");
  Serial.println("ooOOOOooOOO");
}

void loop(){
  digitalWrite(LEDpin, HIGH);
  counter++;
  Serial.print(counter);
  Serial.print(" ");
  startTime = millis();
  currentTime = millis();
  while (currentTime - startTime <= period){
    attachInterrupt(0, count_pulses, RISING);
    }
  digitalWrite(LEDpin, LOW);
  Serial.print("pulses: ");
  Serial.print(";  ");
  Serial.print("frequency: ");
  Serial.print(getfrequency(), DEC);
  Serial.print("  Hz;  ");
  Serial.print("irradiance: ");
  Serial.print(getirradiance(), DEC);
  Serial.print("  uW/cm2");
  Serial.print("  ;  ");
  Serial.print("BRILLO: ");
  Serial.print(getbrillo(),DEC);
  Serial.print("  ;  ");
  Serial.print("MPSAS: ");
  Serial.println(desitjat(),DEC);

  pulses = 0;                                   // Reset the pulses counter for the next measurement
  delay(1000);
}

void count_pulses(){
  pulses++;
  currentTime=millis();
}

unsigned long getfrequency(){
  frequency = pulses/(period/1000);
  return(frequency);
}

long getirradiance(){
  irradiance = frequency / area;
  return (irradiance);
}

long getbrillo(){
  brillo = irradiance * 24756;
  return (brillo);
}


//Calcul MPSAS No funciona be!!!
long desitjat(){
  des = - MagSol + 21.57 - 2.5*log(brillo);
  return des;
}
