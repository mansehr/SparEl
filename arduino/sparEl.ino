/*
This sketch reads from a pin and calculactes the delta between two signals.
The delta is sent to a host over serial and then 
There is a counter
*/


// Number of buckets used
#define buckets 64
// Number of milliseconds per bucket
#define millisPerBucket 60000
// Read status from pin 13
#define signalPin 13
#define statusLedPin 2

// Holds the last read time which will be used to calculate a delta
unsigned long lastRead = 0;
// is the light still on
boolean lastLight = false;
// Last read for bucket
unsigned long lastBucketRead = 0;
// timer buckets
unsigned short ticksPerMinute[buckets];
// Bucket pointer
unsigned short bucketPtr = 0;

// Incomming byte from serial
int incomingByte = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(signalPin, INPUT);
  pinMode(statusLedPin, OUTPUT);
  
  ticksPerMinute[bucketPtr] = 0;
  
  // Loop until signal on pin
  while(!digitalRead(signalPin));

  lastBucketRead = lastRead = millis();
}


inline void serialEvent() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == 0) {
      digitalWrite(statusLedPin, LOW);
    } else {
      digitalWrite(statusLedPin, HIGH);
    }
  }
}

// Change to the next bucket
inline void changeBucket() {
  // Send bucket data 
  Serial.print("b:");
  Serial.println(ticksPerMinute[bucketPtr]);

  // Move to next bucket but check boundries first
  if(++bucketPtr >= buckets) {
    bucketPtr = 0;
  }
  
  // Reset next bucket
  ticksPerMinute[bucketPtr] = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  const unsigned long now = millis();
  boolean lightNow = digitalRead(signalPin);
  if (lastLight == false && lightNow == true) {
    Serial.print("d:");
    Serial.println(now-lastRead);
    lastRead = now;
    ++ticksPerMinute[bucketPtr];
  }
  lastLight = lightNow;
  if((now-lastBucketRead) > millisPerBucket) {
    changeBucket();
    lastBucketRead = now;
  }
  serialEvent();
}
