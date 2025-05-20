#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Define new RX and TX pins for the Bluetooth module
#define RX_PIN 2  // Define your chosen RX pin here
#define TX_PIN 3  // Define your chosen TX pin here

//SDA =A4
//SCL=A5

// Initialize I2C LCD at address 0x27 with 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27 is commonly used, but you should confirm the address of your display

// Initialize software serial on the defined RX and TX pins
SoftwareSerial Bluetooth(RX_PIN, TX_PIN); // RX, TX

void setup() {
    // Start serial communication for Bluetooth
    Bluetooth.begin(9600);
    Serial.begin(9600);

    // Initialize the LCD
    lcd.begin();
    lcd.backlight();

    // Initial message to the Serial Monitor
    Serial.println("Waiting for data");

    // Display "Waiting for data" on the LCD to show it’s ready
    lcd.setCursor(0, 0);
    lcd.print("Waiting for data");
}

void loop() {
    // Check if data is available from the Bluetooth module
    if (Bluetooth.available()) {
        lcd.clear();               // Clear the display
        String message = "";        // Variable to store the incoming message

        // Read the incoming message until the newline character
        while (Bluetooth.available()) {
            char c = Bluetooth.read();
            if (c == '\n') break;   // End reading on newline
            message += c;
        }

        // Print received message to Serial Monitor
        Serial.print("Received: ");
        Serial.println(message);

        // Display the received message on the LCD
        lcd.setCursor(0, 0);         // Start at the first line
        lcd.print("Message:");       // Display "Message:" on the first line
        lcd.setCursor(0, 1);         // Move to the second line
        lcd.print(message);          // Display the actual message

        delay(2000);                 // Delay to allow reading of the message
        lcd.clear();                 // Clear LCD after 2 seconds
        lcd.setCursor(0, 0);
        lcd.print("Waiting for data"); // Ready for the next message
    }
}
