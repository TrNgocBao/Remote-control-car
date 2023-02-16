#include <SPI.h>
#include <nRF24L01.h> 
#include <RF24.h>  // RF24L01
RF24 radio(9,10); // CE, CSN
const byte address[6] = "00001";
char xyData[32] = "";
String xAxis, yAxis;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  xAxis = analogRead(A1); // x của joystick
  yAxis = analogRead(A0); // y

  Serial.print("x: "); Serial.println(xAxis);
  Serial.print("y: "); Serial.println(yAxis);
  
  xAxis.toCharArray(xyData, 5); 
  radio.write(&xyData, sizeof(xyData)); 
  
  yAxis.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
  delay(20);
}
