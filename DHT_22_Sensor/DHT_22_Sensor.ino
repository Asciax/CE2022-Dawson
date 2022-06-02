#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>

#define dhtpin 5 //DHT Sensor Pin
#define dhttype 22 //DHT Sensor Type that is used

DHT dht(dhtpin, dhttype); // Instantiating the DHT Sensor object and initializing the sensor.

void setup() {
  Serial.begin(9600);

  dht.begin();

}

void loop() {
  float humid = dht.readHumidity() ; // Method to extract the humidity recorded by the sensor.
  float temp = dht.readTemperature(); // Read temperature in the default unit, which is Celsius.

  if (isnan(humid) || isnan(temp) ){
    Serial.println("INCOMPLETE DHT DATA"); //Error message in case either one of humid or temp is not recorded
  }

  Serial.println(temp);
  Serial.println(humid);

  delay(3000);
}
