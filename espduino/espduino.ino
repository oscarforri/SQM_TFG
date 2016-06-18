/**
 * \file
 *       ESP8266 MQTT Bridge example
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
#include <SoftwareSerial.h>
#include <espduino.h>
#include <mqtt.h>

SoftwareSerial debugPort(2, 3); // RX, TX
ESP esp(&Serial, &debugPort, 4);
MQTT mqtt(&esp);
boolean wifiConnected = false;
int hum;
float temp;
float mag;

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      debugPort.println("WIFI CONNECTED");
      mqtt.connect("192.168.1.135", 1883, false);
     //mqtt.connect("192.168.1.36", 1883, false);
      wifiConnected = true;
      //or mqtt.connect("host", 1883); /*without security ssl*/
    } else {
      wifiConnected = false;
      mqtt.disconnect();
    }

  }
}

void mqttConnected(void* response)
{
  debugPort.println("Connected");
 // mqtt.subscribe("/topic/0"); //or mqtt.subscribe("topic"); /*with qos = 0*/
 // mqtt.subscribe("/topic/1");
 // mqtt.subscribe("/topic/resposta",0);
  //mqtt.publish("/topic/0", "data0");
  //mqtt.subscribe("/topic", 0);
  //mqtt.subscribe("/topic");
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
  mqtt.subscribe("web_actual", 0);
 
}

void mqttDisconnected(void* response)
{

}
void mqttData(void* response)
{
  RESPONSE res(response);
  
  debugPort.print("Received: topic=");
  String topic = res.popString();
  debugPort.println(topic);

  debugPort.print("data=");
  String data = res.popString();
  debugPort.println(data);

}
void mqttPublished(void* response)
{

}
void setup() {
  Serial.begin(19200);
  debugPort.begin(19200);
  esp.enable();
  Serial.print("voidsetup");
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  debugPort.println("ARDUINO: setup mqtt client");
  if(!mqtt.begin("DVES_duino", "admin", "Isb_C4OGD4c3", 120, 1)) {
    debugPort.println("ARDUINO: fail to setup mqtt");
    while(1);
  }


  debugPort.println("ARDUINO: setup mqtt lwt");
  mqtt.lwt("/lwt", "offline", 0, 0); //or mqtt.lwt("/lwt", "offline");

/*setup mqtt events */
  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.disconnectedCb.attach(&mqttDisconnected);
  mqtt.publishedCb.attach(&mqttPublished);
  mqtt.dataCb.attach(&mqttData);

  /*setup wifi*/
  debugPort.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  esp.wifiConnect("MOVISTAR_DC6A","uhfyPYyejc6CQqjANQGH");


  debugPort.println("ARDUINO: system started");
}

void loop() {
    
  esp.process();
  if(wifiConnected) {
     byte second = 32;
    byte minute = 16;
    byte hour = 23;
    byte monthDay = 23;
    byte month = 6;
    byte year = 16;

   unsigned long freq_mg = 408687512535; //Valor del sensor TSL
   float Magnitut = 21.1358;        //Magnitut calculada
   float humitat = 70.10;         //Humitat
   float temperatura = 25.50;     //Temperatura

    mqtt.subscribe("web_actual");
    
   //char message_buff[100];
   //String pubString = "{\"report\":{\"Data\" \"" + String(monthDay) + "/" + String(month) + "/" + String(year) + "\"},{\"Temperatura\" \"" + String(temperatura)+"\"},{\"Humitat\" \"" + String(humitat)+"\"},{\"Magnitut\" \"" + String(Magnitut)+"\"},{\"Freq\" \"" + String(freq_mg)+"\"}}";
   //pubString.toCharArray(message_buff, pubString.length()+1);
   //mqtt.publish("/topic/envio_actual", message_buff);
   //mqtt.publish("/topic/0", "data0");
    delay(5000);
  }
}
