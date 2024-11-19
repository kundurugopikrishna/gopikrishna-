#include <TM1637Display.h>

// Define the pin for the ultrasonic sensor
const int trigPin = 7;  // Trigger pin for the sensor
const int echoPin = 8;  // Echo pin for the sensor

// Define HW-069 display pins
const int CLK_DISPLAY_PIN = A1; // CLK pin on HW-069
const int DIO_DISPLAY_PIN = A0; // DIO pin on HW-069

// Initialize the HW-069 display
TM1637Display display(CLK_DISPLAY_PIN, DIO_DISPLAY_PIN);

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Initialize the HW-069 display
    display.setBrightness(0x0f);  // Set brightness to max (0x0f is max)

    // Initial message to the Serial Monitor
    Serial.println("Ultrasonic Distance Display");

    // Setup for ultrasonic sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Set pin 10 as output for control
    pinMode(10, OUTPUT);
}

void loop() {
    long duration;

    // Trigger the sensor to start reading
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the pulse
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance using the speed of sound (343m/s)
    int distance = duration * 0.034 / 2;

    // Print the distance to the Serial Monitor (optional for debugging)
    Serial.print("Distance: ");
    Serial.println(distance);

    // Display the distance on the HW-069 display
    display.showNumberDec(distance, true); // true for leading zeroes if needed

    // Turn on pin 10 if distance is greater than 20 cm, otherwise turn it off
    if (distance > 20) {
        digitalWrite(10, HIGH);
    } else {
        digitalWrite(10, LOW);
    }

    delay(1000); // Delay to update the reading every second
}
