                                                                                               #include <Servo.h>  // 서보 라이브러리
#include <SPI.h>    // RFID를 위한 SPI 라이브러리
#include <MFRC522.h>// RFID 라이브러리
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <string.h>  
#include <ESP8266.h>
#include <SoftwareSerial.h>

#define SERVO_PIN 4 //서보 PIN
Servo myservo;      //서보 라이브러리
int count=0;
byte temp_id[10][4];
unsigned long time;
int Buzzer_Pin = 5;
int numTones = 4;
int tones[] = {277, 392, 349, 466};//c#,g,f,a#
int tones2[] = {466, 349, 392, 277};//거꾸로
int tones3[]={349, 349, 349, 349};

#define SS_PIN 10   //RFID SDA PIN
#define RST_PIN 9   //RFID Reset PIN
MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리


/* 등록된 RF CARD ID */
#define PICC_0 0x73
#define PICC_1 0x03
#define PICC_2 0x67
#define PICC_3 0x3C

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SSID        "B631"  

#define PASSWORD    "park0429"  

#define SERVERIP   "192.168.0.7"

SoftwareSerial mySerial(7, 6); /* RX:D10, TX:D9 */ 

void setup() { 
  //시리얼 모니터 시작
 Serial.begin(9600);
 SPI.begin(); // SPI 시작
 rfid.PCD_Init(); // RF 모듈 시작

 myservo.attach(SERVO_PIN); //서보 시작
 myservo.write(0); //초기 서보 모터를 0도로 맞춤
 pinMode(Buzzer_Pin, OUTPUT);

 // initialize the LCD
 lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();

  //랜덤시드
  randomSeed(analogRead(A0));


  Serial.setTimeout(5000);  
  mySerial.begin(9600);   
  Serial.println("ESP8266 connect");

  //boolean connected=false;  
  // for(int i=0;i<10;i++)  
   //{  
    //   if(connectWiFi())  
     //  {  
      //   connected = true;  
       //  break;  
       //}  
   //}  
     
   //if (!connected){while(1);}  
   //delay(5000);  
    
   //mySerial.println("AT+CIPMUX=0");
   
}
 
void loop() {
  
  lcd.setCursor(0,0);
  lcd.print("Waste System!");
  
  //카드가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  //RF카드의 ID가 인식 안되었다면 더이상 진행하지 말고 빠져나감
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  
  Serial.print(F("PICC type: "));
  //PICC 타입 읽어오기
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  
  //ID인증 절차(수정예정)
  if ((rfid.uid.uidByte[0] == PICC_0) &&
      (rfid.uid.uidByte[1] == PICC_1) &&
      (rfid.uid.uidByte[2] == PICC_2) &&
      (rfid.uid.uidByte[3] == PICC_3) ) {

   
    Serial.println(F("This is a confirmed Card."));
   
    if(count==0){
    count++;
    printHex(rfid.uid.uidByte, rfid.uid.size);    
    
    Serial.println(F("투입구가 열렸습니다!!"));
    time = millis();   
    //서보 90도로 이동
    myservo.write(90);
    
    for (int i = 0; i < numTones; i++) {
      //numTones의 수만큼 반복
      //부저시작
      tone(Buzzer_Pin, tones[i]);
      // 0.3초 동안 대기
      delay(300);
    }
    //부저종료
     noTone(Buzzer_Pin);

     //String cmd = "AT+CIPSTART=\"TCP\",\"";  
     //cmd += SERVERIP;  
     //cmd += "\",80";  
     //Serial.println(cmd);  
     //mySerial.println(cmd);  
     //if(mySerial.find("Error"))  
     //{  
     //    Serial.println( "TCP connect error" );  
     //    return;  
     //}
     int my_ran;
     my_ran=random(1,10);
     lcd.setCursor(0,1);
     lcd.print(my_ran);
     //String temp;
     //temp=String(my_ran);
     //cmd = "GET /insert_data.php?num="+temp+"\r\n";  
     //mySerial.print("AT+CIPSEND=");  
     //mySerial.println(cmd.length());  
     //Serial.println(cmd);  
    
     //if(mySerial.find(">"))  
     //{  
       //Serial.print(">");  
       //}else  
       //{  
         //mySerial.println("AT+CIPCLOSE");  
         //Serial.println("connect timeout");  
         //delay(1000);  
        // return;  
       //}  
         
       //mySerial.print(cmd);  
       //delay(200);  
       //Serial.find("+IPD");  
       //while (Serial.available())  
       //{  
        // char c = Serial.read();  
        // mySerial.write(c);  
        // if(c=='\r') mySerial.print('\n');  
       //}  
       //Serial.println("====");
    //delay(1000);
    
    }
    //서보 0도로 되돌림
    else if(count==1){
    myservo.write(0);
    count=0;
    Serial.println(F("투입구가 닫혔습니다!!"));
    //time=millis()-time;
    //Serial.println(time);
    for (int i = 0; i < numTones; i++) {
      //numTones의 수만큼 반복
      //부저시작
      tone(Buzzer_Pin, tones2[i]);
      // 0.3초 동안 대기
      delay(300);
      
    }
    //부저종료
    noTone(Buzzer_Pin);
    lcd.setCursor(0,1);
    lcd.print("Door closed!");
    delay(5000);
    lcd.clear();
    }
  }else{
    //등록된 카드가 아니라면 시리얼 모니터로 ID 표시
    Serial.println(F("This is an unconfirmed Card."));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);

    for (int i = 0; i < numTones; i++) {
      //numTones의 수만큼 반복
      //부저시작
      tone(Buzzer_Pin, tones3[i]);
      // 0.3초 동안 대기
      delay(300);
    }
    //부저종료
    noTone(Buzzer_Pin);
  }
  //종료
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  //다시 시작!

}

//16진수 표시
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.print("\n");
}

boolean connectWiFi()  
{  
   //mySerial.println("AT+CWMODE=1");  
     
   String cmd="AT+CWJAP=\"";  
   cmd+=SSID;  
   cmd+="\",\"";  
   cmd+=PASSWORD;  
   cmd+="\"";  
   mySerial.println(cmd);  
   Serial.println(cmd);  
   delay(3000);  
    
   if(mySerial.find("OK"))  
   {  
     Serial.println("OK, Connected to WiFi.");  
     return true;  
   }  
   else  
   {  
     Serial.println("Can not connect to the WiFi.");  
     return false;  
   }  
 }
