
//define tilt1 variables
int count = 0;

//define tilt2 variables
int count2 = 0;

// constant define
int TILT_1_ENABLE = 1;
int TILT_2_ENABLE = 11;
int LEAD_ENABLE = 25;

// define pins asign
int TILT_1_PIN = 2;
int TILT_2_PIN = 8;
int LEAD_PIN = 3;

// threashold tilt sensor count
int LIMIT = 1;

// grobal refresh rate
int TILT_INTERVAL = 10;
int FRAME_RATE = 3;
int frame_count = 0;

void setup() {
 Serial.begin(9600);
 
 pinMode(TILT_1_PIN, INPUT);
 pinMode(TILT_2_PIN, INPUT);    

 pinMode(LEAD_PIN, INPUT_PULLUP);
}

void loop() {

  // get tilt sensor value
  int tilt = digitalRead(TILT_1_PIN);
  int tilt2 = digitalRead(TILT_2_PIN);

  //get lead val (pullup)
  int lead_val = digitalRead(LEAD_PIN);

  //count up tilt 
  if(tilt == HIGH) {
    count ++;
  } else {
    count = 0;
  }

  //count up tilt2
  if(tilt2 == HIGH) {
    count2++;
  } else {
    count2 = 0;
  }

 // check lead sensor value and send value.
 if(lead_val == LOW) {
  Serial.println(LEAD_ENABLE);
 }

 // check interval tilt sensor 
 if(frame_count % TILT_INTERVAL == 0) {

  // update & send information titl sensor 1
  if(count == LIMIT){
    count = 0;
    Serial.print(TILT_1_ENABLE);
  }

  // update & send information titl sensor 2
  if(count2 == LIMIT){
    count2 = 0;
    Serial.print(TILT_2_ENABLE);
  }
 }

  frame_count++;
  delay(FRAME_RATE);
 
}
