##Compilar y ejecutar con la configuración de Arduino Nano

#include <SoftwareSerial.h>
#include <MD_MAX72xx.h>
#include <RTClib.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>

//##################INICIALIZAMOS VARIABLES Y OBJETOS#########################

//---CONSTANTES---
#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 2  //N de módulos que se conectan de seguido

#define DELAY_ANIMATION 200

//Botones
#define sensorPin A0

//LED
#define CLK_PIN   7
#define DATA_PIN  6
#define CS_PIN    5

//---OBJETOS---
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);
SoftwareSerial ESPserial(3, 2); // RX | TX


// Declaramos un RTC DS3231, MODULO HORA
RTC_DS3231 rtc;

int hora,minut;
boolean alarmaPuesta;

//---VARIABLES GLOBALES
//Botones
int sensorValue;
int tecla;

const int tonePin = 9;

//Dibujos
const byte corazon[] = {0x00,0x66,0x99,0x81,0x81,0x42,0x24,0x18};
const byte alegre[] = {0x00,0x18,0x3c,0x7e,0x66,0x42,0x00,0x00};
const byte triste[] ={0x00,0x00,0x42,0x66,0x7e,0x3c,0x18,0x00};
const byte dormidoL[] = {0xf0,0x20,0x40,0xf0,0x0f,0x02,0x04,0x0f};
const byte dormidoR[]= {0x0f,0x02,0x04,0x0f,0xf0,0x20,0x40,0xf0};
String gn = "good night";
String gm = "good morning";
String my = "miss you";
String lv = "love you";

//ANIMACIONES DE CUANDO NO HACE NADA
const byte parpadeo[][8] = {{0x00,0x3c,0x3c,0x3c,0x3c,0x3c,0x00,0x00},{0x00,0x00,0x3c,0x3c,0x3c,0x3c,0x00,0x00},{0x00,0x00,0x00,0x3c,0x7e,0xff,0x00,0x00},{0x00,0x00,0x00,0x00,0x7e,0xff,0x00,0x00},{0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00}};
const byte mirarIzq[][8] = {{0x00,0x3c,0x3c,0x3c,0x3c,0x3c,0x00,0x00},{0x00,0x78,0x78,0x78,0x78,0x78,0x00,0x00},{0x78,0x78,0x78,0x78,0x78,0x00,0x00,0x00},{0xf0,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00}};
const byte parpadeoIzq[][8] = {{0xf0,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00},{0x00,0xf0,0xf0,0xf0,0xf0,0x00,0x00,0x00},{0x00,0x00,0xf0,0xf8,0xfc,0x00,0x00,0x00},{0x00,0x00,0x00,0xf8,0xfe,0x00,0x00,0x00},{0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00}};
const byte mirarDer[][8] = {{0x00,0x3c,0x3c,0x3c,0x3c,0x3c,0x00,0x00},{0x00,0x1e,0x1e,0x1e,0x1e,0x1e,0x00,0x00},{0x1e,0x1e,0x1e,0x1e,0x1e,0x00,0x00,0x00},{0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00}};
const byte parpadeoDer[][8] = {{0x0f,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00},{0x00,0x0f,0x0f,0x0f,0x0f,0x00,0x00,0x00},{0x00,0x00,0x0f,0x1f,0x3f,0x00,0x00,0x00},{0x00,0x00,0x00,0x1f,0x7f,0x00,0x00,0x00},{0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00}};
const byte vacio[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const byte lleno[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
int animacionActual;
int posicionAnterior;
int tiempo;
//{0x,0x,0x,0x,0x,0x,0x,0x}
//AUXILIARES
boolean stop; //Nos servirá para parar cuando se pulse un botón

//###################FUNCIONES AUXILIARES#############################

/*
 * Función que leerá los botones y devolverá el número del botón pulsado !!!!!!
 */
int keypad(){

  sensorValue=analogRead(sensorPin);
  Serial.print("sensorValue:");
  Serial.println(sensorValue);
  
  if(sensorValue>=1000){
    //Serial.print("my ");
    ESPserial.println("Miss you");
    slide_text(200,my);
    restaurarOjos();
    return 1;
  }
  else if(sensorValue>=900 && sensorValue<1000){
    //Serial.print("corazon ");
    ESPserial.println("corazoncito");
    dibujitos(corazon,false);
    restaurarOjos();
    return 2;
  }
  else if(sensorValue>=820 && sensorValue<900){
    //SI SE PULA OK, SE PONDRÁ EL MODO AHORRO DE ENERGIA, LO QUE DESCARTIVARÁ QUE SE IMPRIMAN LOS OJOS
    if(animacionActual>10){ //EL ahorro de energia está activado, lo desactivamos
      animacionActual=0;
    }else{ //Ahorro energía desactivado, lo activamos
      animacionActual=20;
      restaurarOjos();
    }
    return 3;
  }
  else if(sensorValue>=750 && sensorValue<820){
    //Serial.print("gm ");
    ESPserial.println("Buenos dias");
    slide_text(200,gm);
    restaurarOjos();
    return 4;
  }
  else if(sensorValue>=700 && sensorValue<750){
    ESPserial.println("Dormidita");
    dibujitos(dormidoL,false);
    restaurarOjos();
    return 5;
  }
  else if(sensorValue>=670 && sensorValue<700){
    ponerDespertador();
    restaurarOjos();
    return 6;
  }
  else if(sensorValue>=615 && sensorValue<670){
    //Serial.print("gn ");
    ESPserial.println("Buenas noches");
    slide_text(200,gn);
    restaurarOjos();
    return 7;
  }
  else if(sensorValue>=590 && sensorValue<615){
    ESPserial.println("Triste");
    dibujitos(triste,false);
    restaurarOjos();
    return 8;
  }
  else if(sensorValue>=555 && sensorValue<590){
    mostrarHora();
    restaurarOjos();
    return 9;
  }
  else if(sensorValue>=525 && sensorValue<555){
    //Serial.print("lv ");
    ESPserial.println("Te quiero");
    slide_text(200,lv);
    restaurarOjos();
    return 10;
  }
  else if(sensorValue>=500 && sensorValue<525){
    //Serial.print("alegre ");
    ESPserial.println("Contenta");
    dibujitos(alegre,false);
    restaurarOjos();
    return 11;
  }
  else if(sensorValue<500 && sensorValue>100){
    return 12;
  }
  else{
    Serial.println("No se ha pulsado nada");
    return -1; 
  }
  
}
/*
*Función que comprobará si ha llegado algún mensaje y realizará la accion correspondiente !!!!
*/
int mensajeRecibido(){
  
  if(ESPserial.available()>0){
    
    stop=false;
    
      String recieve=ESPserial.readStringUntil('\n');
      recieve.trim();
      delay(100);
      Serial.print("Se ha recibido un dato "); //Borras más tarde
      Serial.println(recieve);
      if(recieve=="/love"){
        Serial.print("corazon ");
        dibujitos(corazon,true);
        ESPserial.println("Terminado love");
        restaurarOjos();
        return 2;
      }
      else if(recieve=="/happy"){
        Serial.print("happy ");
        dibujitos(alegre,true);
        ESPserial.println("Terminado happy");
        restaurarOjos();
        return 11;
      }
      else if(recieve=="/sad"){
        dibujitos(triste,true);
        ESPserial.println("Terminado sad");
        restaurarOjos();
        return 8;
      }
      else if(recieve=="/sleep"){
        dibujitos(dormidoL,true);
        ESPserial.println("Terminado dormido");
        restaurarOjos();
        return 5;
      }
      else if(recieve=="/gm"){
        Serial.print("text gm ");
        while(!stop){
          slide_text(200,gm);
        }
        ESPserial.println("Terminado gm");
        restaurarOjos();        
        return 4;
      }
      else if(recieve=="/gn"){
        Serial.print("text gn ");
        while(!stop){
          slide_text(200,gn);
        }
        ESPserial.println("Terminado gn");
        restaurarOjos();
        return 7;
      }
      else if(recieve=="/my"){
        Serial.print("text my ");
        while(!stop){
          slide_text(200,my);
        }
        ESPserial.println("Terminado my");
        restaurarOjos();
        return 1;
      }
      else if(recieve=="/lv"){
        Serial.print("text lv ");
        while(!stop){
          slide_text(200,lv);
        }
        ESPserial.println("Terminado lv");
        restaurarOjos();
        restaurarOjos();
        return 10;
      }
      else if(recieve=="/time"){
        mostrarHora();
        restaurarOjos();
        return 9;
      }
      else if(recieve=="/ok"){
        return 3;
      }
      else if(recieve=="/back"){
        return 12;
      }
      else if(recieve=="/setAlarm"){
        return 6;
      }
      else{
        while(!stop){
          slide_text(200,recieve);
        }
        ESPserial.println("Terminado text");
        restaurarOjos();
        return 13;
      }
    
  }
  else{
    return 32;
  }
}

/*
 * Desplazar un caracter de derecha a izquierda
 */

int printText(int pos, const String text){
  int w;
  
  for( int i = 0; i < text.length(); i++ ){
    //Comprobamos a ver si se ha pulsado algún botón para parar la animacion
    if(analogRead(sensorPin)>50){
      stop=true;
    }
    
    // imprimir letra
    
    w = mx.setChar( pos, text[i] );
    // la proxima letra empieza donde termina esta
    pos = pos - w; 
    // Se deja una columna entre letras.
    mx.setColumn(pos, B00000000);
    
    pos = pos - 1;
    
    if( pos < 0 )
      break;
      
  }
  mx.update();
  
  return pos;
}

void slide_text(int ms_delay,String text){
  int col = 0;
  int last_pos;
  bool completo = false;
  
  mx.clear();
 
  while( completo == false ){
    last_pos = printText(col, text);
    delay(ms_delay);
    col++;
    if( last_pos > (int)mx.getColumnCount() )
      completo = true;
  }
}

/*
 * Imprime en los LEDs el dibujo indicado
 */
 void dibujitos(const byte dibujo[],boolean bucle){

  // limpiar la pantalla
  mx.clear();

  drawRows(dibujo);

  if(bucle){
    while(analogRead(sensorPin)<50){
      delay(100);
    }
  }
  else{
    delay(5000);
  }
  mx.clear();
  
}

void drawRows( const byte fig[] ){
  if(fig[0]!=0x00){
    for( int i = 0; i < 8; i++ ){
      mx.setRow(0, i, dormidoR[i]);
      mx.setRow(1, i, fig[i]);
    }
  }
  else{
    for( int i = 0; i < 8; i++ ){
      mx.setRow(0, i, fig[i]);
      mx.setRow(1, i, fig[i]);
    }
  }
  
}

//Funciones correspondientes al reloj
void suenaAlarma(){
  DateTime now = rtc.now();
  stop=false;
  if(alarmaPuesta && now.hour()==hora && now.minute()==minut){
    dibujitosAux(lleno);
    while(!stop){
      playSong();
    }
    alarmaPuesta=false;
    dibujitosAux(vacio);
  }
  delay(100);
  
}

void mostrarHora(){
  for(int i = 0;i<2;i++){
    DateTime now = rtc.now();
    String h = String(now.hour());
    h+=":";
    if(now.minute()<10){
      h+="0";
    }
    h+=now.minute();
    slide_text(200,h);
  }
}

void ponerDespertador(){
  int h=hora;
  int m=minut;
  int sensorValue;
  boolean siguiente=false;
  boolean terminado=false;
  Serial.println("despertador");
  delay(1000);
  //Primero se configura la hora
  while(!siguiente && !terminado){
    impresionAlarma(h);
    Serial.print("1.-");
    sensorValue=analogRead(sensorPin);
    Serial.println(sensorValue);
    delay(200);
    
    if(sensorValue>=820 && sensorValue<900){ //Se pulsa OK 3
      hora = h;
      siguiente=true;
    }
    else if(sensorValue<495 && sensorValue>100){ //Se pulsa back 12
      alarmaPuesta=false;
      siguiente=true;
      terminado=true;
    }
    else if(sensorValue>=730 && sensorValue<820){ //Se pulsa up 2
      h = (h+1)%24;
      
      Serial.println(h);
    }
    else if(sensorValue>=615 && sensorValue<670){ //SE pulsa down 10
      h = (h-1)%24;
      if(h==-1){h=23;}
      Serial.println(h);
    }
  }
  
  delay(200);

  //Luego se configuran los minutos
  while(!terminado ){
    sensorValue=analogRead(sensorPin);
    Serial.print("2.-");
    Serial.println(sensorValue);
    delay(200);
    impresionAlarma(m);
    if(sensorValue>=820 && sensorValue<900){ //Se pulsa OK
      minut = m;
      alarmaPuesta=true;
      Serial.print("Alarma puesta: ");
      Serial.print(h);
      Serial.print(":");
      Serial.println(m);
      mx.control(MD_MAX72XX::TEST, true);
      delay(300);
      // desactivando el modo de prueba
      mx.control(MD_MAX72XX::TEST, false);
      terminado=true;
    }
    else if(sensorValue<495 && sensorValue>100 ){ //Se pulsa back
      alarmaPuesta=false;
      terminado=true;
    }
    else if(sensorValue>=730 && sensorValue<820){ //Se pulsa up
      m = (m+1)%60;
      
      Serial.println(m);
    }
    else if(sensorValue>=615 && sensorValue<670){ //SE pulsa down 10
      m = (m-1)%60;
      if(m==-1){m=59;}
      Serial.println(m);
    }
  }
  Serial.print("xxxxxxxxxxx ");
  mx.clear();
}

void impresionAlarma(int n){
  mx.clear();
  //Sabemos que serán dos dígitos.
  int decenas = (int)((n/10)+48);
  int unidades = (n%10)+48;
  
  mx.setChar(14,decenas);
  mx.setChar(6,unidades);
  
}

//*****************************************--------------------------*************************************
void setup() {
  Serial.begin(9600);
  ESPserial.begin(9600);

  // inicializar el objeto mx
  mx.begin();
 
  // poniendo la matriz en modo de prueba
  mx.control(MD_MAX72XX::TEST, true);
  delay(200);
 
  // desactivando el modo de prueba
  mx.control(MD_MAX72XX::TEST, false);
 
  // Establecer intencidad a un valor de 5
  mx.control(MD_MAX72XX::INTENSITY, 2);

  // Comprobamos si tenemos el RTC conectado
   if (! rtc.begin()) {
      Serial.println("No hay un módulo RTC");
      //while (1);
   }
  // Ponemos en hora, solo la primera vez, luego comentar y volver a cargar.
 // Ponemos en hora con los valores de la fecha y la hora en que el sketch ha sido compilado.
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();

  alarmaPuesta=false;
  animacionActual=0;
  posicionAnterior=0;
  tiempo=0;

  
  dibujitosAux(parpadeo[0]);
 
}

void loop() {
  animacionOjos();
  suenaAlarma();
  mensajeRecibido();
  keypad();
}


//*****************************************--------------------------*************************************

void restaurarOjos(){
  if(animacionActual<=5){
    dibujitosAux(parpadeo[0]);
  }
  else if(animacionActual>=6 && animacionActual<=10){
    if(posicionAnterior==0){
      dibujitosAux(parpadeoIzq[0]);
    }else{
      dibujitosAux(parpadeoDer[0]);
    }
  }
  else{
    dibujitosAux(vacio);
  }
}
void animacionOjos(){
  //animacionActual;
  //posicionAnterior;
  //tiempo;

  //----NOS ENCONTRAMOS CON LOS OJOS EN EL CENTRO---
  if(animacionActual<=5){
    tiempo++;
    Serial.print("Tiempo:");
    Serial.println(tiempo);
    if(tiempo>=50){ //!!!!!!!!!!!!!!!!!!!!!!!!!!AJUSTAR
      for(int i=0;i<5;i++){
        dibujitosAux(parpadeo[i]);
        delay(15);
      }
      for(int i=4;i>=0;i--){
        dibujitosAux(parpadeo[i]);
        delay(15);
      }
      tiempo=0;
      animacionActual++;
    }
  }
  else if(animacionActual==6){
    posicionAnterior=random(2);
    Serial.print("Random:");
    Serial.println(posicionAnterior);
    //miramos izq
    if(posicionAnterior==0){
      for(int i=0;i<4;i++){
        dibujitosAux(mirarIzq[i]); 
        delay(15);
      }
      animacionActual++;
    }else if(posicionAnterior==1){ //Miramos der
      for(int i=0;i<4;i++){
        dibujitosAux(mirarDer[i]);
        delay(15);
      }
      animacionActual++;
    }else{ //Seguimos mirando al frente
      animacionActual=0;
      tiempo=0;
    }
  }
  else if(animacionActual>6 && animacionActual<10){
    //Miramos izq
    tiempo++;
    
    if(tiempo>=50){
      if(posicionAnterior==0){
        for(int i=0;i<5;i++){
          dibujitosAux(parpadeoIzq[i]);
          delay(15);
        }
        for(int i=4;i>=0;i--){
          dibujitosAux(parpadeoIzq[i]);
          delay(15);
        }
      }else{ //Miramos der
        for(int i=0;i<5;i++){
          dibujitosAux(parpadeoDer[i]);
          delay(15);
        }
        for(int i=4;i>=0;i--){
          dibujitosAux(parpadeoDer[i]);
          delay(15);
        }
      }
      tiempo=0;
      animacionActual++;
    }
  }

  else if(animacionActual==10){
    if(posicionAnterior==0){
      for(int i=3;i>=0;i--){
        dibujitosAux(mirarIzq[i]);
        delay(15);
      }
    }else if(posicionAnterior==1){ //Miramos der
      for(int i=3;i>=0;i--){
        dibujitosAux(mirarDer[i]);
        delay(15);
      }
    }
    animacionActual=0;
    tiempo=0;
    
  }
  else{
  }
  
  
}

/*
 * Imprime en los LEDs el dibujo indicado
 */
 void dibujitosAux(const byte dibujo[]){

  drawRowsAux(dibujo);
  
}
void drawRowsAux( const byte fig[] ){

    for( int i = 0; i < 8; i++ ){
      mx.setRow(0, i, fig[i]);
      mx.setRow(1, i, fig[i]);
    
    }
  
}

//Sonará la alarma del despertador.
/*En este caso suena la canción "NaNaNaNaNa" de My Chemical Romance. Modificar y codificar por la deseada. Además, periodicamente comprobará si se ha pulsado el botoón para
//que pare de sonar.
*/
void playSong() {

    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,  293,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);

    stop = stop || analogRead(sensorPin)>50;
    delay(360);

    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    //Termina linea 1
    tone(tonePin,349,120);
    delay(120);
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,220,120);
    delay(360);
    tone(tonePin,220,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,220,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,220,120);
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    //Fin linea 2
    
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,220,120);
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    tone(tonePin,220,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120); //C
    delay(120);
    tone(tonePin,220,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,233,120);
    delay(120);
    tone(tonePin,196,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,196,120);
    delay(120);
    tone(tonePin,174,120);
    delay(120);
    tone(tonePin,196,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    tone(tonePin,174,120);
    stop = stop || analogRead(sensorPin)>50;
    //FIN LINEA 3
    
    delay(360);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,220,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,311,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    //FIN LINEA 4
    
    delay(120);
    tone(tonePin,293,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(600);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);
    stop = stop || analogRead(sensorPin)>50;
    //FIN LINEA 5
    
    delay(360);
    tone(tonePin,293,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    delay(360);
    tone(tonePin,293,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,293,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,349,120);
    //FIN LINEA 6
    
    delay(120);
    tone(tonePin,293,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,220,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    //FIN LINEA 7
    delay(120);
    tone(tonePin,196,120);
    delay(120);
    tone(tonePin,220,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,220,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,233,120);
    delay(120);
    tone(tonePin,196,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    tone(tonePin,196,120);
    delay(120);
    tone(tonePin,174,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,196,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,174,120);
    //FIN LINEA 8
    delay(360);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(360);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,220,120);
    delay(120);
    tone(tonePin,261,120);
    delay(360);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,311,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,293,120);
    delay(360);
    tone(tonePin,293,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,293,120);
    //FIN LINEA 9
    delay(360);
    tone(tonePin,261,120);
    delay(600);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
    delay(120);
    tone(tonePin,261,120);
    delay(120);
    stop = stop || analogRead(sensorPin)>50;
    tone(tonePin,261,120);
    delay(120);
    tone(tonePin,261,120);
    stop = stop || analogRead(sensorPin)>50;
}

/* COSAS QUE PUEDEN SER ÚTILES
 * 
// Sacar todas las matrices del modo de bajo consumo 
mx.control(MD_MAX72XX::SHUTDOWN, false);
 
// Establecer a 1 la intensidad de los LEDs
mx.control(MD_MAX72XX::INTENSITY, 1);
 * 
 */
