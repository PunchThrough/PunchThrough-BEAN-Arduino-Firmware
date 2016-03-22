/*
 * This creates a custom advertisement for the Bean.  The bean will continue to advertise as itself, but will switch
 * back and forth with the custom advertisement.  Use this in conjunction with another Bean running observer.ino to share data.
 */


// debounce code
int buttonState;             // the current input
int lastButtonState = LOW;   // the previous read input
long lastDebounceTime = 0;   // the last time we flipped the output pin
long debounceDelay = 50;     // how long to debounce for

static uint8_t customAdvertData[] = {
  // general discoverable mode advertises indefinitely
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // two-byte broadcast
  0x03,   // length of this data including the data type byte
  GAP_ADTYPE_MANUFACTURER_SPECIFIC,
  0xAC,   // arbitrary data so we can make sure this is the Bean we're looking for
  0x00
};

bool enabled = true;
void setup() {
  Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
  Bean.enableCustom();
  Bean.enableConfigSave(false);
  Bean.setAdvertisingInterval(100);
  pinMode(0, INPUT_PULLUP);
  Serial.begin();
}


void loop() {
  
  int reading = digitalRead(0);  
  if(reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;

        if (buttonState == LOW) {
          enabled = !enabled;
          Bean.setLedGreen(enabled ? 0 : 255);
          customAdvertData[6] = enabled ? 1 : 0;
          Bean.setCustomAdvertisement(customAdvertData, sizeof(customAdvertData));
        }  
    }
  }
  lastButtonState = reading;
}