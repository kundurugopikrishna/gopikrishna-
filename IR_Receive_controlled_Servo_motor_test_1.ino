int irSensorPin = 7; // IR sensor output connected to pin 7
int relayPin = 3;    // Relay IN pin connected to pin 3
int irSensorValue = 0; // Variable to store IR sensor value

void setup() {
  pinMode(irSensorPin, INPUT);  // IR sensor is input
  pinMode(relayPin, OUTPUT);    // Relay is output
  digitalWrite(relayPin, LOW);  // Ensure the relay is off initially
  Serial.begin(9600);           // Initialize serial communication
}

void loop() {
  irSensorValue = digitalRead(irSensorPin); // Read the IR sensor
  Serial.println(irSensorValue);           // Print the IR sensor value to the serial monitor

  if (irSensorValue == HIGH) {
    // Object detected
    
    digitalWrite(relayPin, HIGH);
    delay(1000);// Turn relay ON (motor starts)
  } else {
    // No object detected
    digitalWrite(relayPin, LOW); 
       delay(1000);
  }
}





/*VCC: Connect to Arduino 5V.
GND: Connect to Arduino GND.
IN: Connect to a digital pin on the Arduino (e.g., D3).
COM (Common): Connect to the positive terminal of the 9V battery.
NO (Normally Open): Connect to one terminal of the motor.
GND of Battery: Connect to the other terminal of the motor.
Flyback Diode:*/
