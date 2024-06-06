const int ledSet1 = D1; // GPIO5
const int ledSet2 = D2; // GPIO4
const int ledSet3 = D3; // GPIO0
const int ledSet4 = D4; // GPIO2
const int ldrPin = A0; // Analog pin

const int lightThreshold = 512; // Adjust this value as needed

void setup() {
  // Initialize LED pins as outputs
  pinMode(ledSet1, OUTPUT);
  pinMode(ledSet2, OUTPUT);
  pinMode(ledSet3, OUTPUT);
  pinMode(ledSet4, OUTPUT);

  // Initialize Serial for debugging
  Serial.begin(115200);
}

void loop() {
  // Read the value from the LDR
  int lightValue = analogRead(ldrPin);
  Serial.print("Light value: ");
  Serial.println(lightValue);

  // Check if the light value is below the threshold
  if (lightValue < lightThreshold) {
    // Light up each set of LEDs with a 1-second delay between each set
    digitalWrite(ledSet1, HIGH);
    delay(1000);
    digitalWrite(ledSet2, HIGH);
    delay(1000);
    digitalWrite(ledSet3, HIGH);
    delay(1000);
    digitalWrite(ledSet4, HIGH);
  } else {
    // Turn off all LEDs if the light value is above the thresholds
    digitalWrite(ledSet1, LOW);
    digitalWrite(ledSet2, LOW);
    digitalWrite(ledSet3, LOW);
    digitalWrite(ledSet4, LOW);
  }

  // Add a small delay to avoid flooding the serial monitor
  delay(100);
}

