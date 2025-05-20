// Define the pin for the ultrasonic sensor
const int trigPin = 7;  // Trigger pin for the sensor
const int echoPin = 8;  // Echo pin for the sensor

// Variable to keep track of the people count
int peopleCount = 0;
long previousDistance = 0; // To store the previous distance for detecting motion

void setup() {
  // Initialize the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize serial communication
  Serial.begin(9600);

  // Initial message to the Serial Monitor
  Serial.println("People Count System");
  updateSerialMonitor();
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

  // Logic for counting people based on distance thresholds
  if (previousDistance > 300 && distance < 300) {  // Something enters the sensor range
    peopleCount++;
    updateSerialMonitor();
  }
//else if (previousDistance < 200 && distance > 200) {  // Something exits the sensor range
//    peopleCount--;
//    updateSerialMonitor();
//  }

  // Save the current distance for the next loop iteration
  previousDistance = distance;

  delay(500); // Delay to avoid reading too frequently and to give time for motion detection
}

void updateSerialMonitor() {
  // Print the current people count to the Serial Monitor
  Serial.print("People In: ");
  Serial.println(peopleCount);

  if (peopleCount == 0) {
    Serial.println("Please Visit!");
  }
}
