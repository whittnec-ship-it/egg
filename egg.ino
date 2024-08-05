//TO DO: Investigate Power Reduction Register, watchdog

// Basic demo for accelerometer readings from Adafruit LIS3DH
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <avr/sleep.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// Low Power 5Khz data rate needs faster SPI, and calling setPerformanceMode & setDataRate
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, 2000000);
// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

/*
Captures orientation from LIS3DH and prints it out
*/
void calculateOrientation () {
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(lis.x);
  Serial.print("  \tY:  "); Serial.print(lis.y);
  Serial.print("  \tZ:  "); Serial.print(lis.z);

  Serial.println();

  delay(15000);              // readings every 15 seconds; TO DO: what is the best interval?
}

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("LIS3DH test!");

  // more stuff making sure LIS3DH is ready
  if (!lis.begin(0x18)) {
    Serial.println("Couldn't start LIS3DH");
    while (1) yield(10);
  }
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  //Serial.print("Range = "); Serial.print(2 << lis.getRange());
  //Serial.println("G");

  /*
  lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER); // TO DO: investigate low power mode (although by the name, low power sounds best)
  Serial.print("Performance mode set to: ");
  switch (lis.getPerformanceMode()) {
    case LIS3DH_MODE_NORMAL: Serial.println("Normal 10bit"); break;
    case LIS3DH_MODE_LOW_POWER: Serial.println("Low Power 8bit"); break;
    case LIS3DH_MODE_HIGH_RESOLUTION: Serial.println("High Resolution 12bit"); break;
  }

  lis.setDataRate(LIS3DH_DATARATE_50_HZ);  // TO DO: investigate best data rate
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("1.6 Khz Low Power"); break;
  }
  */
  
}

void loop() {

  calculateOrientation ();

  // sleep stuff
  set_sleep_mode (SLEEP_MODE_PWR_DOWN); // SLEEP_MODE_PWR_DOWN established to be best for power savings
  noInterrupts ();           // timed sequence follows
  sleep_enable();

  // disable ADC
  ADCSRA = 0;    // TO DO: investigate if this is relevant
  
  //lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  //Serial.print("X:  "); Serial.print(lis.x);
  //Serial.print("  \tY:  "); Serial.print(lis.y);
  //Serial.print("  \tZ:  "); Serial.print(lis.z);

  /* Or....get a new sensor event, normalized */
  //sensors_event_t event;
  //lis.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  //Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  //Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  //Serial.println(" m/s^2 ");

  //Serial.println();
  
  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS); 
  interrupts ();             // guarantees next instruction executed
  sleep_cpu ();              // sleep within 3 clock cycles of above

  sleep_disable();

}
