int incomingByte = 0;   // for incoming serial data

void setup() {
  pinMode(4, OUTPUT);   //Arduino UNOの13番のピンをスイッチ制御に使う
  Serial.begin(9600);      // 9600bpsでシリアルポートを開く
}

void loop() {
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
         // read the incoming byte:
         incomingByte = Serial.read();

         // say what you got:
         Serial.print("I received: ");
         Serial.println(incomingByte, DEC);

         if(incomingByte == 1) {
            digitalWrite(4, HIGH);   // スイッチをONする
            delay(10);
            digitalWrite(4, LOW);   // スイッチをONする
            delay(10);
         } 
         
//         if(incomingByte == 0) {
//            digitalWrite(4, LOW);   // スイッチをONする
//         }
  }
}
