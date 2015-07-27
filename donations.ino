const int sensorPin = A0;
const float tresholdFactor = .90;
const long calibrationIntervalTime = 2000; // 2 seconds
const long transactionIntervalTime = 6000; // 6 seconds
const long confirmationIntervalTime = 1000; // 2 seconds

const int LEDPin1 = 9;
const int LEDPin2 = 10;
const int LEDPin3 = 11;

const int LEDPinBtn1 = 2;
const int LEDPinBtn2 = 3;
const int LEDPinBtn3 = 4;

const int BTNPin1 = 5;
const int BTNPin2 = 7;
const int BTNPin3 = 8;

int sensorValue = 0;
int sensorMaxValue = 0;
long lastReading = 0;
int selectedAmount = 0;
boolean signalSentToProcessing = false;
boolean isCardRemoved = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  pinMode(LEDPinBtn1, OUTPUT);
  pinMode(LEDPinBtn2, OUTPUT);
  pinMode(LEDPinBtn3, OUTPUT);
  pinMode(BTNPin1, INPUT_PULLUP);
  pinMode(BTNPin2, INPUT_PULLUP);
  pinMode(BTNPin3, INPUT_PULLUP);
}

void loop() {
  
  sensorValue = analogRead(sensorPin);
  long currentMillis = millis();
//Serial.println(sensorValue);
  if(currentMillis < calibrationIntervalTime) { // calibration
    if(sensorValue > sensorMaxValue) {
      sensorMaxValue = sensorValue;
    }
    if(currentMillis%1000 < 500) {
      analogWrite(LEDPin1,255);
      analogWrite(LEDPin2,255);
      analogWrite(LEDPin3,255);
    } else {
      analogWrite(LEDPin1,0);
      analogWrite(LEDPin2,0);
      analogWrite(LEDPin3,0);
    }
 
  } else if (isCardRemoved
       && sensorValue <= sensorMaxValue*tresholdFactor
       && (lastReading == 0 || lastReading + transactionIntervalTime + confirmationIntervalTime < currentMillis)) {
    
    isCardRemoved = false;
    lastReading = currentMillis;

  } else if (!isCardRemoved
      && lastReading != 0
      && lastReading + transactionIntervalTime >= currentMillis
      && sensorValue <= sensorMaxValue*tresholdFactor) {

      if(currentMillis%3000 <= 1000) {
        analogWrite(LEDPin1,255);
        analogWrite(LEDPin2,0);
        analogWrite(LEDPin3,0);
      } else if (currentMillis%3000 <= 2000) {
        analogWrite(LEDPin1,0);
        analogWrite(LEDPin2,255);
        analogWrite(LEDPin3,0);
      } else {
        analogWrite(LEDPin1,0);
        analogWrite(LEDPin2,0);
        analogWrite(LEDPin3,255);
      }

  } else if (!isCardRemoved
      && lastReading != 0
      && lastReading + transactionIntervalTime >= currentMillis
      && sensorValue > sensorMaxValue*tresholdFactor) {

      isCardRemoved = true;
      lastReading = 0; // reset if "card" is taken away "early"
  
  } else if (!isCardRemoved
      && lastReading != 0
      && lastReading + transactionIntervalTime + confirmationIntervalTime >= currentMillis) { 

      if(!signalSentToProcessing) {
        Serial.println('@');
        signalSentToProcessing = true;
      }
      
      if(lastReading + transactionIntervalTime - currentMillis > -300
        || lastReading + transactionIntervalTime - currentMillis < -600) {
        analogWrite(LEDPin1,255);
        analogWrite(LEDPin2,255);
        analogWrite(LEDPin3,255);
      } else {
        analogWrite(LEDPin1,0);
        analogWrite(LEDPin2,0);
        analogWrite(LEDPin3,0);
      }

  } else {
      analogWrite(LEDPin1,0);
      analogWrite(LEDPin2,0);
      analogWrite(LEDPin3,0);
      signalSentToProcessing = false;
      if(sensorValue > sensorMaxValue*tresholdFactor) {
        isCardRemoved = true;
      }
  }


  // Amount Selection Block:
  if (digitalRead(BTNPin1) == LOW) {     
    // turn LED off:    
    digitalWrite(LEDPinBtn1, HIGH);
    digitalWrite(LEDPinBtn2, LOW);
    digitalWrite(LEDPinBtn3, LOW);
    selectedAmount = 1;
  } else if (digitalRead(BTNPin2) == LOW) {
    // turn LED on:
    digitalWrite(LEDPinBtn1, LOW);
    digitalWrite(LEDPinBtn2, HIGH);
    digitalWrite(LEDPinBtn3, LOW);
    selectedAmount = 2; 
  } else if (digitalRead(BTNPin3) == LOW) {
    // turn LED on:
    digitalWrite(LEDPinBtn1, LOW);
    digitalWrite(LEDPinBtn2, LOW);
    digitalWrite(LEDPinBtn3, HIGH);
    selectedAmount = 3; 
  }

}
