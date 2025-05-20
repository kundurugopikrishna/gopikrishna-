//
//
//
//// the setup function runs once when you press reset or power the board
//void setup() {
//  // initialize digital pin LED_BUILTIN as an output.
//   Serial.begin (115200);
//  pinMode(LED_BUILTIN, OUTPUT);
//}
//
//// the loop function runs over and over again forever
//void loop() {
//   blink();
//   int data;  
// if (Serial.available() > 0)
//   {
//     data= Serial.read();// wait for a second
//     Serial.write(data);
//   }
//}
//
//
//
//void blink()
//{
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);    
//}





//
//
//
//
//const int MAX_STRING_LENGTH = 100; // Maximum length of the input string
//
//void setup() {
//  Serial.begin(115200); // Initialize serial communication
//  pinMode(LED_BUILTIN, OUTPUT); // Set LED_BUILTIN as an output pin
//}
//
//void loop() {
//  blink(); // Call the blink function to blink the LED
//
//  if (Serial.available() > 0) {
//    String inputString = ""; // Initialize an empty string
//
//    // Read characters from the serial buffer
//    while (Serial.available() > 0) {
//      char incomingChar = Serial.read(); // Read a character
//
//      // If the incoming character is a newline, break the loop
//      if (incomingChar == '\n') {
//        break;
//      }
//
//      // Append the character to the input string
//      inputString += incomingChar;
//
//      // Optionally handle string length to avoid overflow
//      if (inputString.length() > MAX_STRING_LENGTH) {
//        Serial.println("Error: Input string too long.");
//        inputString = ""; // Clear the input string if it exceeds the maximum length
//        break;
//      }
//    }
//
//    // Print the received string if it's not empty
//    if (inputString.length() > 0) {
//      Serial.print("Received string: ");
//      Serial.println(inputString);
//    }
//  }
//
//  delay(100); // Small delay to avoid continuous reading
//}
//
//void blink() {
//  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (HIGH is the voltage level)
//  delay(100); // Wait for 100 milliseconds
//  digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage LOW
//  delay(100); // Wait for 100 milliseconds
//}




/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/SerialEvent
*/
String inputString = "";      // A String to hold incoming data
bool stringComplete = false;  // Whether the string is complete

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Initialize LED_BUILTIN as an output pin
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Handle LED blinking
  blink();

  // Print the string when a newline arrives
  if (stringComplete) {
    Serial.println(inputString);
    // Clear the string
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever new data comes in the hardware serial RX.
  This routine runs between each time loop() runs, so using delay inside loop 
  can delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // Get the new byte
    char inChar = (char)Serial.read();
    Serial.write(inChar); // Echo the character back to the serial monitor

    // Add it to the inputString
    inputString += inChar;

    // If the incoming character is a newline, set a flag
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on (HIGH is the voltage level)
  delay(10); // Wait for 500 milliseconds
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED off by making the voltage LOW
  delay(10); // Wait for 500 milliseconds
}
