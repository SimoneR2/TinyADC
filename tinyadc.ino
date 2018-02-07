#include <SoftwareSerial.h>

//Software serial, 0=RX 1=TX
SoftwareSerial mySerial (0, 1);

long average = 0;
int counter = 0;

void setup() {
  mySerial.begin(9600); //begin serial at 9600 baud
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  delay(15000); //this delay is to let the esp8266 booting
}

void loop() {
  int offset = analogRead(A3); //reading Vout reference (- in the schematic)
  int temp = analogRead(A2); //readign Vout (+ in the schematic)
  temp = temp - offset; //calculating offset
  average = average + temp; //average
  counter++; //counter to do the average
  if (counter > 15) { //if enough data then calculate the average
    average = average / counter;
    char str[3];
    char sign;
    float temperatura = ((average * 500.0) / 1024); //convert bit to voltage (mV)
    if (temperatura < 0) { //storing the sign
      sign = '-';
      temperatura = temperatura * (-1.0); //since we saved the sign, we can invert our temperature
    }
    else sign = '+';
    int garb = (temperatura * 10); //cast float to int (ex. 12.3 float -> 123 int)
    itoa(garb, str, 10); //Turn value into a character array
    mySerial.print('a'); //Serial trasmission start char
    mySerial.print(sign); //sign
    if (temperatura >= 10) { 
      mySerial.print(str);
    }
    else if (temperatura < 1.0) { //to avoid serial problem we need to add two zero if the number is < 1.0
      mySerial.print('0');
      mySerial.print('0');
      mySerial.print(str);
    }
    else {
      mySerial.print('0');
      mySerial.print(str);
    }
    counter = 0; //reset variable
    average = 0;
  }
  delay(200); //delay to catch data
}

