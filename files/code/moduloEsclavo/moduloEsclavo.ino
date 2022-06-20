#include <SoftwareSerial.h>
#include "CTBot.h"
#include "Utilities.h" // for int64ToAscii() helper function

String ssid  = "Orange-D51A"    ; // REPLACE mySSID WITH YOUR WIFI SSID
String pass  = "643ACA24"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "2048651549:AAEkJqTwos9Y6iDIGYeG_ZnVa7GBK5Yg2rY"   ; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
uint32_t myId = 605994901;

SoftwareSerial mySerial(4, 5); //RX TX

CTBot myBot;
TBMessage msg;

String comandos="";

void comunicarMensaje(){
  if(myBot.getNewMessage(msg)){
    if(msg.text=="/help"){
      myBot.sendMessage(myId,comandos);
    }
    else{
      Serial.print("Se ha recibido un mensaje //  ");
      Serial.println(msg.text);
      mySerial.println(msg.text);
    } 
  }
}

void recibirSerial(){
  if(mySerial.available()>0){
    String recieve=mySerial.readStringUntil('\n');
    delay(100);
    myBot.sendMessage(myId,recieve);
  }
}

void setup() {
  // initialize the Serial
  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);
  
  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  comandos+="/love: Enviar un corazón\n";
  comandos+="/happy: El perrito se pone contento\n";
  comandos+="/sad:El perrito se pone triste\n";
  comandos+="/sleep: El perrito se pone a dormir\n";
  comandos+="/gm: Imprime un mensaje de buenos días\n";
  comandos+="/gn: Imprime un mensaje de buenas noches\n";
  comandos+="/my: Imprime el mensaje miss you\n";
  comandos+="/lv: Imprime un te quiero\n";
  comandos+="/time: Imprime la hora\n";
  comandos+="/ok: Simula la pulsación del botón ok del perrito\n";
  comandos+="/back: Simula la pulsación del botón back del perrito\n";
  comandos+="/setAlarm: Simula la pulsación del botón setAlarm del perrito\n";
  comandos+="Cualquier otro texto, lo imprime en el perrito";

  myBot.sendMessage(myId,"Se sincronizó");
  myBot.sendMessage(myId,comandos);
  
}

void loop() {
  
  comunicarMensaje();
  delay(200);
  recibirSerial();
  delay(200);

}
