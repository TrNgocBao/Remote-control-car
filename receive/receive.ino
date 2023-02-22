#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h> // Thư viện nrf24l01
#define enA 3
#define in1 2
#define in2 4
#define enB 5
#define in3 6
#define in4 7

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
char receivedData[32] = "";
int  xAxis, yAxis;
int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {   
    radio.read(&receivedData, sizeof(receivedData)); // đọc dữ liệu mà mạch phát gửi tới
    xAxis = atoi(&receivedData[0]); // chuyển đổi dữ liệu sang dạng số nguyên (trục x)
    Serial.print("x: "); Serial.println(xAxis);
    delay(10);
    radio.read(&receivedData, sizeof(receivedData));
    yAxis = atoi(&receivedData[0]);// chuyển đổi dữ liệu sang dạng số nguyên (trục y)
    Serial.print("y: "); Serial.println(yAxis);
    delay(10);
  }
  
 
  if (yAxis < 460) {
    // xe tien khi y < 460
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    motorSpeedA = map(yAxis, 460, 0, 0, 255);
    motorSpeedB = map(yAxis, 460, 0, 0, 255);
  }
  else if (yAxis > 550) {
    // xe chạy lui
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    motorSpeedA = map(yAxis, 550, 1023, 0, 255);
    motorSpeedB = map(yAxis, 550, 1023, 0, 255);
  }
   else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  if (xAxis < 460) {
    int xMapped = map(xAxis, 460, 0, 0, 255);
    // xe rẻ trái
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
     if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 550) {
    int xMapped = map(xAxis, 550, 1023, 0, 255);
    // xe rẻ phải
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
     if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA);
  analogWrite(enB, motorSpeedB); 
}
