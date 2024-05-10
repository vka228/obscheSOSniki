#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(5, 7);
int data[3];  

const int xPin = A0;
const int yPin = A1;
const int buttonPin = 4;

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
int tumblrState = 0;
int ptmrState = 0;

int speed = 0;
int direction = 0;
                                
void setup(){
    radio.begin();
    Serial.begin(9600);
    radio.setChannel(4);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    
    pinMode(xPin, INPUT);
    pinMode(yPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}
void loop(){
    xPosition = analogRead(xPin);
    yPosition = analogRead(yPin);
    buttonState = digitalRead(buttonPin);
    

    data[0] = xPosition;
    data[1] = yPosition;
    data[2] = !buttonState;
    radio.write(&data, sizeof(data));
    if(radio.write(&data, sizeof(data))){
      Serial.println("OK");
      
      // указывая сколько байт массива мы хотим отправить. с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){ приняты;}else{не приняты приёмником;}
    }
    Serial.println("Xj:" + String(xPosition) + "; Yj:" + String(yPosition) + "; Btn:" + String(buttonState));
    //Serial.println(ptmrState);
    //Serial.println(speed);
    delay(100);
}
