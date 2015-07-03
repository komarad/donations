const int sensorPin = A0;
const float tresholdFactor = .5;
const long transactionIntervalTime = 10000; // 10 seconds
const int LEDPin = 9;

int sensorValue = 0;
int sensorMaxValue = 0;
long lastReading = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  
  sensorValue = analogRead(sensorPin);
  long currentMillis = millis();
  Serial.println(sensorValue);
  if(currentMillis < 5000) { // calibration
    if(sensorValue > sensorMaxValue) {
      sensorMaxValue = sensorValue;
    }
    if(currentMillis%1000 < 500) {
      analogWrite(LEDPin,255);
    } else {
      analogWrite(LEDPin,0);
    }
 
  } else if (sensorValue <= sensorMaxValue*tresholdFactor
       && lastReading + transactionIntervalTime < currentMillis) {
    // put your main code here, to run repeatedly:
    lastReading = currentMillis;
    Serial.println('@');
 
  } else if (lastReading != 0
      && lastReading + transactionIntervalTime >= currentMillis) {
    analogWrite(LEDPin,255);

  } else if(currentMillis%1000 < 100 || currentMillis%1000 < 300 && currentMillis%1000 > 200) {
      analogWrite(LEDPin,255);
  } else {
      analogWrite(LEDPin,0);
  }

}
