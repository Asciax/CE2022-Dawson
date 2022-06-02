#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>

#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

//RF24 Object
RF24 radio(3,4);//CE(Chip Enable), CSN( Chip Select Not)

//Communication address:
const uint8_t address[6] = {'0','0','0','0','1'}; //Address of the pipe through which the two radio transmitters communicate

#define dhtpin 5 //DHT Sensor Pin
#define dhttype 22 //DHT Sensor Type that is used

DHT dht(dhtpin, dhttype); // Assigning the DHT Sensor.

void setup() {
  printf_begin();
  
  Serial.begin(9600);
  
  radio.begin(); //Initializing 'radio' object that is the nRF24L01
  
  radio.setChannel(76);  // Setting the channel to 2400 + channel Mhz.
  radio.setRetries(15,15); // A delay of 15 * 250 microseconds = 0.004 seconds. The second argument is the number of retries, before giving up.
  
  bool chipConnected = radio.isChipConnected();
  if (chipConnected) {
    Serial.println("Chip is connected to SPI");
  }

  
  //char channel[] = radio.getChannel();
  Serial.print("This transmitter is connected to channel :");
  Serial.println(radio.getChannel());

  radio.setPALevel(RF24_PA_MIN, 1);
  



  //Opening a pipe and setting the adress where the transmitter will write to.
  radio.openWritingPipe(address);

  //This method stops listening for incoming messages and switches the module to transmit mode.
  radio.stopListening(); 

  radio.printPrettyDetails();

  dht.begin(); //Initializing 'dht' object
  
}

void loop() {
  float humid = dht.readHumidity() ; // Method to extract the humidity recorded by the sensor.
  float temp = dht.readTemperature(); // Read temperature in the default unit, which is Celsius.

  

  if (isnan(humid) || isnan(temp) ){
    Serial.println("INCOMPLETE DHT DATA"); //Error message in case either one of humid or temp is not recorded
  }
  char temp_array[7];
  char humid_array[7];
  dtostrf(temp, -6, 2, temp_array);
  dtostrf(humid, -6, 2, humid_array );

  Serial.println(temp_array);
  Serial.println(humid_array);
   
  boolean radioACK_TEMP = radio.write(&temp_array, sizeof(temp_array));
  delay(500);
  boolean radioACK_HUMID = radio.write(&humid_array, sizeof(humid_array));

  if ( (radioACK_TEMP) && (radioACK_HUMID) ) {
    Serial.println("Acknowledgement received for both");
  }

  if ( (radioACK_TEMP == false) || (radioACK_HUMID == false) ) {
    Serial.println("Acknowledgement NOT received for one or both messages");

    bool tx_ok, tx_fail, rx_ready; // Debug variables to find out what happened to the module
    radio.whatHappened(tx_ok, tx_fail, rx_ready);
    
    Serial.print("tx_ok :");
    Serial.println(tx_ok);
    Serial.print("tx_fail :");
    Serial.println(tx_fail);
    Serial.print("rx_ready :");
    Serial.println(rx_ready);
    
   }

  if (radio.failureDetected) {
    Serial.println("Failure Detected");
    radio.failureDetected = 0; //Resetting failure count
  }
   delay(1000);
}
