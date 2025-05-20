#include <LedControl.h>

// Define the pin connections
const int DIN_PIN = 2;
const int CLK_PIN = 3;
const int CS_PIN = 4;

// Initialize the LedControl library
// Parameters: (DIN pin, CLK pin, CS pin, number of displays)
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); 

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Initialize MAX7219
    lc.shutdown(0, false);    // Wake up MAX7219
    lc.setIntensity(0, 8);    // Set brightness level (0 is min, 15 is max)
    lc.clearDisplay(0);       // Clear display on startup
}

void loop() {
    // Check if data is available in the serial buffer
    if (Serial.available() > 0) {
        // Read incoming data from Serial Monitor
        String data = Serial.readStringUntil('\n');

        // Display data on dot matrix
        showText(data);
    }
}

// Function to display text on dot matrix
void showText(String text) {
    lc.clearDisplay(0); // Clear display before showing new text

    for (int i = 0; i < text.length(); i++) {
        char character = text[i];

        // Display each character on the matrix for a short time
        displayChar(character);
        delay(500); // Adjust delay for faster/slower scrolling
    }
}

// Function to map digits to dot matrix patterns
void displayChar(char character) {
    byte digit[8];

    // Map each digit to an 8-byte pattern
    switch (character) {
        case '0':
            {
                byte digit0[8] = {0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00}; // Pattern for '0'
                memcpy(digit, digit0, 8);
            }
            break;
        case '1':
            {
                byte digit1[8] = {0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00}; // Pattern for '1'
                memcpy(digit, digit1, 8);
            }
            break;
        case '2':
            {
                byte digit2[8] = {0x62, 0x51, 0x49, 0x49, 0x46, 0x00, 0x00, 0x00}; // Pattern for '2'
                memcpy(digit, digit2, 8);
            }
            break;
        case '3':
            {
                byte digit3[8] = {0x22, 0x41, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00}; // Pattern for '3'
                memcpy(digit, digit3, 8);
            }
            break;
        case '4':
            {
                byte digit4[8] = {0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00}; // Pattern for '4'
                memcpy(digit, digit4, 8);
            }
            break;
        case '5':
            {
                byte digit5[8] = {0x2F, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00}; // Pattern for '5'
                memcpy(digit, digit5, 8);
            }
            break;
        case '6':
            {
                byte digit6[8] = {0x3E, 0x49, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00}; // Pattern for '6'
                memcpy(digit, digit6, 8);
            }
            break;
        case '7':
            {
                byte digit7[8] = {0x01, 0x01, 0x71, 0x09, 0x07, 0x00, 0x00, 0x00}; // Pattern for '7'
                memcpy(digit, digit7, 8);
            }
            break;
        case '8':
            {
                byte digit8[8] = {0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00}; // Pattern for '8'
                memcpy(digit, digit8, 8);
            }
            break;
        case '9':
            {
                byte digit9[8] = {0x06, 0x49, 0x49, 0x49, 0x3E, 0x00, 0x00, 0x00}; // Pattern for '9'
                memcpy(digit, digit9, 8);
            }
            break;
        default:
            {
                byte empty[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Blank character for unsupported chars
                memcpy(digit, empty, 8);
            }
            break;
    }

    // Display the pattern on the dot matrix
    for (int row = 0; row < 8; row++) {
        lc.setRow(0, row, digit[row]);
    }
}
