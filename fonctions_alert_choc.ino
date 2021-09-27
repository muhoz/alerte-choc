#include <SoftwareSerial.h>
#define KNOCK_PIN 6
#define led_buzzer 12
#define led_verte 8
#define bouton 7

unsigned long previousMillis;
unsigned long time_out = 10000;
SoftwareSerial mySerial(5, 4);
char phone_no[] = "698967838"; 
String data[5];
String state, timegps, latitude, longitude;
String gps  = "GPS";
String mes = "";
boolean debug = true;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CGNSPWR=1"); 
  updateSerial();

  mySerial.println("AT+CGNSSEQ=RMC"); 
  updateSerial();

  mySerial.println("AT+CMGF=1\r"); 
  updateSerial();

  mySerial.println("AT+CNMI=1,2,0,0,0"); 
  updateSerial();


 pinMode(led_buzzer, OUTPUT);
  pinMode(KNOCK_PIN, INPUT);
  pinMode(led_verte, OUTPUT);
  pinMode(bouton, INPUT);

}

void loop() {
  if (digitalRead(KNOCK_PIN)) {
    noTone(led_buzzer);     
    digitalWrite(led_buzzer, LOW);
    digitalWrite(led_verte, HIGH);
  }
  else{
    buz_sound();
    digitalWrite(led_verte, LOW);
     previousMillis = millis();
    Serial.println(previousMillis);
  
  while ((millis() - previousMillis) < time_out) {
      buz_sound();
      if (digitalRead(bouton) == HIGH )
      { goto ExitL; }
      while (mySerial.available() > 0 ) {
        mes = mySerial.readString();
        if (mes.indexOf("stop") > 0 ) {
          digitalWrite(led_buzzer, LOW);
          goto ExitL;
        }
      }
       
    }

while (digitalRead(bouton) == LOW) {
      Serial.println("Temps dépassé, Envoie du message");
      //delay(000);
      buz_sound();
      data[1] = "";
      data[2] = "";
      data[3] = "";
      data[4] = "";
      sendTabData("AT+CGNSINF", 1000, debug);
 if (state != 0) {
        mySerial.print("AT+CMGS=\"");
        mySerial.print(phone_no);
        mySerial.println("\"");
        delay(300);
        mySerial.print("collision detected  ");
        mySerial.print("http://maps.google.com/maps?q=loc:");
        mySerial.print(latitude);
        mySerial.print(",");
        mySerial.print (longitude);
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

void buz_sound(){
  tone(led_buzzer,3000,5000);
  digitalWrite(led_buzzer,HIGH);
}
 
