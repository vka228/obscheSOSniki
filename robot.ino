#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_NeoPixel.h>

#define SPEED_1      5
#define DIR_1        4

#define SPEED_2      6
#define DIR_2        7

#define MATRIX_PIN    10
#define LED_COUNT 16
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

RF24 radio(8, 9); // nRF24L01+ (CE, CSN)
int data[3];

void setup() {
  Serial.begin(9600);
  matrix.begin();
  radio.begin();
  radio.setChannel(4);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, 0x1234567890LL);
  radio.startListening();

  // Motor pins setup
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    int xPosition = data[0];
    int yPosition = data[1];
    int btnState = data[2];
    Serial.println("Xj:" + String(xPosition) + "; Yj:" + String(yPosition) + "; Btn:" + String(btnState));



    // чекаем чё там по игреку
    if (yPosition > 400 && yPosition < 600) {
      
      // фигачим влево, если x < 300
      if (xPosition < 400) { 
        digitalWrite(DIR_1, HIGH);
        digitalWrite(DIR_2, LOW); // Opposite direction for turning
        analogWrite(SPEED_1, 255); // Full speed
        analogWrite(SPEED_2, 255); // Full speed
      } 

      // фигачим вправо, если x > 600
      else if (xPosition > 600) { // Move right if xPos > 800
        digitalWrite(DIR_1, LOW); // Opposite direction for turning
        digitalWrite(DIR_2, HIGH);
        analogWrite(SPEED_1, 255); // Full speed
        analogWrite(SPEED_2, 255); // Full speed
      } 
      else{
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);
    }
    }

    // фигачим прямо
    else if (yPosition >= 600){
      if (xPosition > 400 && xPosition < 600){
        digitalWrite(DIR_1, HIGH);
        digitalWrite(DIR_2, HIGH);
        analogWrite(SPEED_1, 255); // Full speed
        analogWrite(SPEED_2, 255); // Full speed
      }
      else{
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);
    }
    }

    // фигачим назад
    else if (yPosition <= 400){
      if (xPosition > 400 && xPosition < 600){
        digitalWrite(DIR_1, LOW);
        digitalWrite(DIR_2, LOW);
        analogWrite(SPEED_1, 255); // Full speed
        analogWrite(SPEED_2, 255); // Full speed
      }

      else{
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);
    }
    }


    // если джойсик в другом месте, то стоп машина
    




    // фигачим факел бля
    if (btnState == 0) {
      colorWipe(matrix.Color(255, 255, 0), 0); // Turn on the lamp
    } 

    else if (btnState == 1) {
      colorWipe(matrix.Color(0, 0, 0), 0); // Turn off the lamp
    } 
    
      
  }
}
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < matrix.numPixels(); i++) {
    // заполняем текущий сегмент выбранным цветом
    matrix.setPixelColor(i, c);
    matrix.show();
    // ждём
    delay(wait);
  }
}