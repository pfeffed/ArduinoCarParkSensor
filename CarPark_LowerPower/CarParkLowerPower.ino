// ---------------------------------------------------------------------------
// Simple parking sensor for an arduino with a HC-SR04 ultrasonic range finder
// ---------------------------------------------------------------------------

#include <NewPing.h>

// Define threshold distances in cm for ping sensor.
#define WARNING_DISTANCE 60 // Inside this range, show yellow light.
#define STOP_DISTANCE    20 // Inside this range, show red light.

// Ultrasonic Rangefinder Configuration
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6   // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Output for tricolor led
#define RED_PIN   9   // Red LED,   connected to digital pin 9
#define GREEN_PIN 10  // Green LED, connected to digital pin 10
#define BLUE_PIN  11  // Blue LED,  connected to digital pin 11

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
int lastStatus = -1;
boolean sleepStatus = false;
unsigned long timeLastStatusChange = millis();

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pinMode(RED_PIN,   OUTPUT);   // sets the pins as output
  pinMode(GREEN_PIN, OUTPUT);   
  pinMode(BLUE_PIN,  OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);    // turn the LED on the board off to save an itty bit of power
}

void loop() {
  if((timeLastStatusChange + 30000) < millis()){
    sleepStatus = true;
  }
  if(sleepStatus)
    delay(1000); // Sleep mode... wait 1 second between pings to save power.
  else
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  int dist = uS / US_ROUNDTRIP_CM;
  if(dist == 0){ // nothing in range
    updateLastStatus(0);
    setLight(255,255,255);  // off
  }
  else if(dist > 60){
    updateLastStatus(3);
    setLight(255,0,255); // green
  }
  else if(dist > 20){
    updateLastStatus(2);
    setLight(0,0,255); // yellow
  }
  else if(dist <= 20){
    updateLastStatus(1);
    setLight(0,255,255); // red
  }
  else{
    updateLastStatus(-1);
    setLight(255,255,255);
  }
}

void setLight(int r, int g, int b){
  Serial.print("Last Change");
  Serial.println(timeLastStatusChange);
  Serial.print("Timestamp");
  Serial.println(millis());
  if(sleepStatus){
    r=255;
    g=255;
    b=255;
  }
  analogWrite(RED_PIN, r);   // Write current values to LED pins
  analogWrite(GREEN_PIN, g); 
  analogWrite(BLUE_PIN, b);  
}

void updateLastStatus(int currStatus){
  if(lastStatus != currStatus){
    lastStatus = currStatus;
    sleepStatus = false;
    timeLastStatusChange = millis();
  }
}
