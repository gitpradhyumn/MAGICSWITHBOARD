// Include necessary libraries
#include <Arduino.h>

// Define pin numbers for switches and LEDs
const int switchPins[] = {2, 3, 4, 5}; // Switches connected to pins 10, 9, 8, 7
const int ledPins[] = {7, 8, 9, 10};   // LEDs connected to pins 5, 4, 3, 2   


// Array to track the state of each switch
bool switchState[4] = {false, false, false, false};
// Array to track which LED is currently assigned to a switch
int ledAssignment[4] = {-1, -1, -1, -1};
// Flag to indicate if initial startup behavior is complete
bool startupComplete = false;

void setup() {
    // Configure switch pins as input with pull-up resistors
    for (int i = 0; i < 4; i++) {
        pinMode(switchPins[i], INPUT_PULLUP);
    }

    // Configure LED pins as output
    for (int i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW); // Ensure LEDs are off initially
    }
}

void loop() {
    if (!startupComplete) {
        startupComplete = checkStartup();
        return;
    }

    for (int i = 0; i < 4; i++) {
        // Read the state of each switch (LOW means pressed because of pull-up)
        bool currentState = !digitalRead(switchPins[i]);

        if (currentState && !switchState[i]) {
            // If the switch is toggled ON
            assignLED(i);
            switchState[i] = true;
        } else if (!currentState && switchState[i]) {
            // If the switch is toggled OFF
            unassignLED(i);
            switchState[i] = false;
        }
    }
}

bool checkStartup() {
    static bool allSwitchesProcessed[4] = {false, false, false, false};

    for (int i = 0; i < 4; i++) {
        bool currentState = !digitalRead(switchPins[i]);

        if (currentState && !allSwitchesProcessed[i]) {
            // Turn on LED corresponding to switch index
            digitalWrite(ledPins[i], HIGH);
            allSwitchesProcessed[i] = true;
        } else if (!currentState && allSwitchesProcessed[i]) {
            // Turn off LED corresponding to switch index
            digitalWrite(ledPins[i], LOW);
            allSwitchesProcessed[i] = false;
        }
    }

    // Check if all switches have been toggled ON and OFF at least once
    bool allProcessed = true;
    for (int i = 0; i < 4; i++) {
        if (!allSwitchesProcessed[i]) {
            allProcessed = false;
        }
    }

    return allProcessed;
}

void assignLED(int switchIndex) {
    // Find the next available LED slot
    for (int i = 0; i < 4; i++) {
        if (ledAssignment[i] == -1) {
            // Assign the LED to this switch
            ledAssignment[i] = switchIndex;
            updateLEDs();
            return;
        }
    }
}

void unassignLED(int switchIndex) {
    // Find and unassign the LED corresponding to this switch
    for (int i = 0; i < 4; i++) {
        if (ledAssignment[i] == switchIndex) {
            ledAssignment[i] = -1;
            updateLEDs();
            return;
        }
    }
}

void updateLEDs() {
    // Turn off all LEDs
    for (int i = 0; i < 4; i++) {
        digitalWrite(ledPins[i], LOW);
    }

    // Turn on LEDs based on the order of assignments
    for (int i = 0; i < 4; i++) {
        if (ledAssignment[i] != -1) {
            digitalWrite(ledPins[i], HIGH);
        }
    }
}
