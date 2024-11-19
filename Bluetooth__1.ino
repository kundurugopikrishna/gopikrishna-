void setup() {
  // Initialize Serial communication for the Serial Monitor
  Serial.begin(9600); // Baud rate for Serial Monitor
  
  Serial.println("Bluetooth Communication Started");
}

void loop() {
  // Check if data is available from Bluetooth (or Serial Monitor)
  if (Serial.available()) {
    // Read the data from Serial (Bluetooth or Serial Monitor)
    String receivedData = Serial.readStringUntil('\n'); // Read until newline character
    
    // Print received data to the Serial Monitor
    Serial.print("Received: ");
    Serial.println(receivedData);
    
    // Optionally, send a response back to Bluetooth
//    Serial.print("Echo: ");
//    Serial.println(receivedData);
  }
}
