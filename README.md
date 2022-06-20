- [Español](#arduino-dogbot-es)
- [English](#arduino-dogbot-en)

# Arduino DogBot (ES)
## Explicación del proyecto 
La idea de este proyecto es conectar a dos personas mediante un robot programado en arduino. El principal propósito está en profundizar sobre la comunicación serial entre dos procesadores arduino siguiendo el modelo maestro-esclavo, además de entrar en el mundo de la conectividad wifi y los bots de telegram usando arduino y sus librerias.
## Materiales necesarios
- Placa NoceMCU (1u)
- Placa Arduino Nano (1u)
- Módulo matriz led 8x8 MAX7219 (2u)
- Teclado botón 3x4 (1u)
- Reloj en tiempo real DS1307 I2C (RTC) (1u)
- Zumbador (1u)
- Cable USB hembra a Micro usb y Mini usb (1u)
- Resistencia 1kΩ (3u)
- Placa pcb (1u)
- Cables alambre (17u)

## Esquema conexiones
![Esquema de conexinoes](/files/images/conections.jpg)

## Explicación funcionamiento
#### Módulo esclavo (NodeMCU)
Su principal función es conectarse a la red wifi para comunicarse con el bot de telegram. Para ello utilizaremos las librerias *SoftwareSerial*, *CTBot* y *utilities*.
Para configurar nuestras conexiones tendremos que introducir los datos del router a conectar en los parámetros indicados. Además tendremos que crear un bot para añadirlo a nuestras conexiones. Esto lo prodremos hacer siguiendo el tutorial incluido en referencias. 
El código se basa en dos funciones: 
    *comunicarMensaje* : En el caso de que recibamos un mensaje del bot de ayuda devolvemos al bot un mensaje con los comandos necesarios para usar el bot; o en el caso de que sea otro mensaje, se lo comunicamos a la placa de arduino nano mediante la comunicación serial.
    *recibirSeñal* : Se encarga de comprobar si hemos recibido un mensaje por la comunicación serial, y en caso de hacerlo lo reenviamos al bot.
    
#### Módulo maestro (Arduino nano)
Este módulo tiene un código más complejo y extenso. Su función es tratar las acciones recibidas por el módulo esclavo o por la entrada de teclado y luego comunicarlas de nuevo por la comunicación serial. Para ello utilizaremos las bibliotecas *SoftwareSerial*, *MD_MAX72xx* , *RTClib* , *Wire* , *Time* y *TimeLib*.
El funcionamiento principal se representa en los módulos led que hacen de ojos, que mostrarán expresiones o mensajes según se reciban las señales asociadas, ya sea por teclado o mensaje. También tenemos la oportunidad de ver la hora actual o programar una alarma que hará sonar al zumbador (en este caso se ha programado una melodía similar a la canción [My Chemical Romance - Na Na Na](https://www.youtube.com/watch?v=egG7fiE89IU) .

## Video funcionamiento
[![Alt text](https://img.youtube.com/vi/jPMBlZ14dIc/0.jpg)](https://www.youtube.com/watch?v=jPMBlZ14dIc)

## Códigos de animaciones matriz led
**Bot telegram**
- /love: Enviar un corazón
- /happy: El perrito se pone contento
- /sad:El perrito se pone triste
- /sleep: El perrito se pone a dormir
- /gm: Imprime un mensaje de buenos días
- /gn: Imprime un mensaje de buenas noches
- /my: Imprime el mensaje miss you
- /lv: Imprime un te quiero
- /time: Imprime la hora
- /ok: Simula la pulsación del botón ok del perrito
- /back: Simula la pulsación del botón back del perrito
- /setAlarm: Simula la pulsación del botón setAlarm del perrito
- Cualquier otro texto, lo imprime en el perrito

**Códigos arduino**
![Animaciones estáticas](/files/images/animations/Page1.jpg)
![Animaciones dinámicas1](/files/images/animations/Page5.jpg)
![Animaciones dinámicas2](/files/images/animations/Page6.jpg)
![Animaciones dinámicas3](/files/images/animations/Page7.jpg)
![Animaciones dinámicas4](/files/images/animations/Page8.jpg)

## Medidas carcasa robot
![Medidas generales](/files/images/measures/Page3.jpg)
![Medidas generales](/files/images/measures/Page4.jpg)
Estas medidas son en caso de que se quiera hacer la carcasa en madera contrachapada (como yo la hice), en caso de quererla imprimir en 3d, he aportado un [modelo  para poder imprimirlo](/files/model3D/modelo.blend), además para estructurar las piezas. 

## Referencias
- [Crear bot telegram y conectarlo a nuestra placa.](https://www.youtube.com/watch?v=1uulbeZ3egw)
- [Alarma/despertador arduino](https://www.instructables.com/AlarmaDespertador-Con-Arduino/) 
- [Chat Bot telegram con arduino y ESP8266](https://electrojoan.com/chat-bot-telegram-con-esp8266/)
- [Como enviar mensajes con arduino a nuestro movil](https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/110-como-enviar-mensajes-desde-arduino-a-nuestro-movil-con-telegram)
- [Matriz LED con arduino y MAX7219-MAX7221](https://programarfacil.com/blog/arduino-blog/matriz-led-arduino-max7219/)
- [Reloj arduino RTC](https://programarfacil.com/blog/arduino-blog/reloj-con-arduino-rtc/)
- [Conectar NoceMCU a arduino](https://borrowbits.com/2017/11/como-comunicar-arduino-con-nodemcu-parte-i-conexion-serieuart/)


# Arduino DogBot (EN)
