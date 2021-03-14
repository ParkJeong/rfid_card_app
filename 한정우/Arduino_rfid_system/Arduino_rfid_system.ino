#include <Servo.h>  // 서보 라이브러리
#include <SPI.h>    // RFID를 위한 SPI 라이브러리
#include <MFRC522.h>// RFID 라이브러리

#define SERVO_PIN 4 //서보 PIN
Servo myservo;      //서보 라이브러리
int count=0;
byte temp_id[10][4];
unsigned long time;
int Buzzer_Pin = 5;
int numTones = 4;
int tones[] = {277.1826, 391.9954, 349.2282, 466.1638};//c#,g,f,a#
int tones2[] = {466.1638, 349.2282, 391.9954, 277.1826};//거꾸로

#define SS_PIN 10   //RFID SDA PIN
#define RST_PIN 9   //RFID Reset PIN
MFRC522 rfid(SS_PIN, RST_PIN); //RFID 라이브러리


/* 등록된 RF CARD ID */
#define PICC_0 0xF9
#define PICC_1 0xF2
#define PICC_2 0x90
#define PICC_3 0x6D



void setup() { 
  //시리얼 모니터 시작
 Serial.begin(9600);
 SPI.begin(); // SPI 시작
 rfid.PCD_Init(); // RF 모듈 시작

 myservo.attach(SERVO_PIN); //서보 시작
 myservo.write(0); //초기 서보 모터를 0도로 맞춤
 pinMode(Buzzer_Pin, OUTPUT);
}
 
void loop() {
  
  //char c=Serial.read();
  //if(c==10){
  //  Serial.print("10이 입력되었습니다.");
  //  }

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
     
    }
    //서보 0도로 되돌림
    else if(count==1){
    myservo.write(0);
    count=0;
    Serial.println(F("투입구가 닫혔습니다!!"));
    time=millis()-time;
    Serial.println(time);
    for (int i = 0; i < numTones; i++) {
      //numTones의 수만큼 반복
      //부저시작
      tone(Buzzer_Pin, tones2[i]);
      // 0.3초 동안 대기
      delay(300);
    }
    //부저종료
     noTone(Buzzer_Pin);

    
    }
  }else{
    //등록된 카드가 아니라면 시리얼 모니터로 ID 표시
    Serial.println(F("This is an unconfirmed Card."));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);


    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      temp_id[0][i] = rfid.uid.uidByte[i];
    }
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
