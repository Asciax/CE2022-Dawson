#include <SoftwareSerial.h>

#include <TinyGPS++.h>


// NEO 6M GPS module pins
#define gpsRXPin 5
#define gpsTXPin 6

//GPS Baud frequency
int GPSBaud = 9600;

//Creating TinyGPS++ Object
TinyGPSPlus gps;

//Transforming 2 Digital pins into serial pins with SoftwareSerial Module, creating a port named "gpsSerial"
SoftwareSerial gpsSerial(gpsRXPin, gpsTXPin);

double latitude, longitude, alt,  kmperh;
bool isListeningCheck = false; //Variable to perform a singular check to see if the Software Serial is listening.

void setup() {
  Serial.begin(9600);

  pinMode(gpsRXPin, INPUT);
  pinMode(gpsTXPin, OUTPUT);

  // Initiating the software serial port with the gps' assigned baud.
  gpsSerial.begin(GPSBaud);
  
}

void gpsData(double latitude, double longtitude, double alt, double kmperh) {
  if (gps.location.isValid()) {
    ::latitude = gps.location.lat();
    ::longitude = gps.location.lng();
    ::alt = gps.altitude.meters();
  }
  else {
    Serial.println("GPS : Location data not available");
  }

  if (gps.speed.isValid()) {
    ::kmperh = gps.speed.kmph();
  }
  else {
    Serial.println("GPS : Speed data not available");
  }
}

void loop() {
  gpsSerial.listen();
  Serial.println(gpsSerial.overflow());
  Serial.println(gpsSerial.available());
  
  if ( (gpsSerial.isListening()) && (isListeningCheck == false) ) {
    Serial.println("GPS Soft. Serial is listening");
    isListeningCheck = true;
  }
  
  if (gpsSerial.available() ) {
    Serial.println("gpsSerial available");
  }
  else {
    Serial.println("gpsSerial NOT available");
  }
    
  while (gpsSerial.available() > 0) {
    Serial.write(gpsSerial.read());
    if (gps.encode(gpsSerial.read())) {
      gpsData(latitude, longitude, alt, kmperh);

      Serial.print("Latitude: ");
      Serial.println(latitude);
      Serial.print("Longitude: ");
      Serial.println(longitude);
      Serial.print("Altitude: ");
      Serial.println(alt);
      Serial.print("KMPH: ");
      Serial.println(kmperh);
    }
  if (gpsSerial.available() == 0) {
    Serial.print("Characters processed: ");
    Serial.println(gps.charsProcessed());
    Serial.print("Sentences that failed checksum:"); // thus possibly lost some data
    Serial.println(gps.failedChecksum());
    Serial.print("Sentences that passed checksum: ");
    Serial.println(gps.passedChecksum());
    
    }
  }
// Snippet of code from arduiniana.org/libraries/tinygpsplus

// Debug: if we haven't seen lots of data in 5 seconds, something's wrong.
if (millis() > 5000 && gps.charsProcessed() < 10) // uh oh
{
  Serial.println("ERROR: not getting any GPS data!");
  // dump the stream to Serial
  Serial.println("GPS stream dump:");
  while (true) // infinite loop
    if (gpsSerial.available() > 0) // any data coming in?
      Serial.write(gpsSerial.read());
}
// End of snippet from arduiniana.org/libraries/tinygpsplus

}
