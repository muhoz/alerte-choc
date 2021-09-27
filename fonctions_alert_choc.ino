//declaration of libraries
#include <SoftwareSerial.h>  //library to permit the communication between the SIM808 and the arduino

//****declaration of constants
#define KNOCK_PIN 6     //pin number that will be used for knock sensor
#define led_buzzer 12   //pin number that will be used for red led
#define led_verte 8     //pin number that will be used for green led
#define bouton 7        //pin number that will be used for push button


SoftwareSerial mySerial(5, 4);  //communication port(TX,RX)

//******declaration of variables

unsigned long previousMillis;
unsigned long time_out = 10000;
char phone_no[] = "XXXXXXXXX";  //phone number to be used 
String data[5];
String state, timegps, latitude, longitude;  
String gps  = "GPS";
String mes = "";      //message received
boolean debug = true;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);

  //*******initialisation of the SIM808
  mySerial.println("AT"); //Hello packet
  updateSerial();

  mySerial.println("AT+CGNSPWR=1");  //activate de power of the SIM808
  updateSerial();

  mySerial.println("AT+CGNSSEQ=RMC");  
  updateSerial();

  mySerial.println("AT+CMGF=1\r"); //text configuration
  updateSerial();

  mySerial.println("AT+CNMI=1,2,0,0,0"); 
  updateSerial();


  pinMode(led_buzzer, OUTPUT);
  pinMode(KNOCK_PIN, INPUT);
  pinMode(led_verte, OUTPUT);
  pinMode(bouton, INPUT);

}

void loop() {
  
  if (digitalRead(KNOCK_PIN)) {  //if there is no choc
    noTone(led_buzzer);     
    digitalWrite(led_buzzer, LOW);
    digitalWrite(led_verte, HIGH);
  }
  else{                 //if there is a choc
    buz_sound();
    digitalWrite(led_verte, LOW);
    previousMillis = millis();
    Serial.println(previousMillis);
  
  while ((millis() - previousMillis) < time_out) { //listen to a receive message in the 10s after the hit. We get out of the loop by two ways:
                                                   //Firtsly, if the button is pushed,
                                                   //Secondly, if a message("stop") is received 
      buz_sound();
      if (digitalRead(bouton) == HIGH )
      { goto ExitL; } //exit the loop
      while (mySerial.available() > 0 ) {
        mes = mySerial.readString();
        if (mes.indexOf("stop") > 0 ) {
          digitalWrite(led_buzzer, LOW);
          goto ExitL;
        }
      }
       
    }

while (digitalRead(bouton) == LOW) { //if there is no message received and no body pushed the button, then we can conlude that we have to send the SOS with gps position of the hit
      Serial.println("Temps dépassé, Envoie du message");
      //delay(000);
      buz_sound();
      data[1] = "";
      data[2] = "";
      data[3] = "";
      data[4] = "";
      sendTabData("AT+CGNSINF", 1000, debug);   //listen to the response of the AT command and retrieve the latitude and longitude
 if (state != 0) {
        mySerial.print("AT+CMGS=\"");       //insert the phone number
        mySerial.print(phone_no);
        mySerial.println("\"");
        delay(300);
        mySerial.print("collision detected  ");   //message to be sent
        mySerial.print("http://maps.google.com/maps?q=loc:"); //message to be sent
        mySerial.print(latitude);  //message to be sent
        mySerial.print(",");       //message to be sent
        mySerial.print (longitude);   //message to be sent
        delay(200);
        mySerial.println((char)26);   
        delay(200);
        mySerial.println();
      }
      else{
         Serial.println("GPS Initialising...");
      }  
  }
  ExitL: Serial.println("fin");
    updateSerial();
}

}

//function to see the communication between the two modules (Arduino - Sim808)
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}

//function to retrieve information (state, latitude, longitude...) from the response of the AT command
void sendTabData(String command , const int timeout , boolean debug) {
  buz_sound();
  mySerial.println(command);
  long int time = millis();
  int i = 0;

  while ((time + timeout) > millis()) {
    while (mySerial.available()) {
      char c = mySerial.read();
      if (c != ',') {
        data[i] += c;
        delay(100);
      } else {
        i++;
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
} exitL:
  if (debug) {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude = data[4];
  }
}

//function to make buzzer sound
void buz_sound(){
  tone(led_buzzer,3000,5000);
  digitalWrite(led_buzzer,HIGH);
}
 
