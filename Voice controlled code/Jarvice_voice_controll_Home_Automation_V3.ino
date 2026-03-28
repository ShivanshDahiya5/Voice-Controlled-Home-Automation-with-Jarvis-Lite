const int relayPin = 3;       // Relay connected to Pin 3
const int statusLedPin = 13;  // Onboard LED for status indication

const bool RELAY_ACTIVE_LOW = true;  // true = Active LOW relay
                                      // false = Active HIGH relay

String voiceCommand = "";
bool lightState = false;
unsigned long lastCharTime = 0;
const int commandTimeout = 300;


void relayON() {
  if(RELAY_ACTIVE_LOW) {
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
  }
  digitalWrite(statusLedPin, HIGH);
  lightState = true;
}

void relayOFF() {
  if(RELAY_ACTIVE_LOW) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  digitalWrite(statusLedPin, LOW);
  lightState = false;
}

void printStartupMessage() {
  Serial.println();
  Serial.println("╔══════════════════════════════════════════════════╗");
  Serial.println("║            JARVIS VOICE CONTROL SYSTEM           ║");
  Serial.println("║              Arduino UNO R4 Minima               ║");
  Serial.println("║                    Created By                    ║");
  Serial.println("║ Ganesh Prajapati / The DC Minds YouTube channel  ║");
  Serial.println("║          Please Like Share & Subscribe           ║");
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.println("║  Bluetooth: Pin 0 (RX) & Pin 1 (TX)              ║");
  Serial.println("║  Relay: Pin 3                                    ║");
  Serial.println("║  Status LED: Pin 13                              ║");
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.print("║  Relay Type: ");
  if(RELAY_ACTIVE_LOW) {
    Serial.println("ACTIVE LOW                          ║");
  } else {
    Serial.println("ACTIVE HIGH                         ║");
  }
  Serial.println("╚══════════════════════════════════════════════════╝");
  Serial.println();
  Serial.println("╔══════════════════════════════════════════════════╗");
  Serial.println("║              SUPPORTED COMMANDS                  ║");
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.println("║  ON:  light on, lights on, led on, on karo,      ║");
  Serial.println("║       chalu karo, jala do, turn on               ║");
  Serial.println("║  OFF: light off, lights off, led off, off karo,  ║");
  Serial.println("║       band karo, bujha do, turn off              ║");
  Serial.println("╚══════════════════════════════════════════════════╝");
  Serial.println();
}

void setup() {
  // Pin Setup
  pinMode(relayPin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);
  
  // Initially OFF
  relayOFF();
  
  // USB Serial (Computer/Serial Monitor)
  Serial.begin(9600);
  

  unsigned long startTime = millis();
  while(!Serial && (millis() - startTime < 3000)) {
    // Wait maximum 3 seconds for Serial connection
    // Agar 3 second mein connect nahi hua, toh aage badho
  }
  
  // Small delay for stability
  delay(500);
  
  // Hardware Serial1 (Bluetooth on Pin 0, 1)
  Serial1.begin(9600);
  
  // Print Startup Message (only if Serial connected)
  if(Serial) {
    printStartupMessage();
    Serial.println(">>> JARVIS: Initializing...");
  }
  
  // Ready Signal - 3 Blinks
  for(int i = 0; i < 3; i++) {
    digitalWrite(statusLedPin, HIGH);
    delay(200);
    digitalWrite(statusLedPin, LOW);
    delay(200);
  }
  
  if(Serial) {
    Serial.println(">>> JARVIS: Ready for your command, Sir!");
    Serial.println();
  }
}

void loop() {
  
  while(Serial1.available()) {
    char c = Serial1.read();
    
    if(c != '\n' && c != '\r') {
      voiceCommand += c;
    }
    
    lastCharTime = millis();
  }
  
  if(voiceCommand.length() > 0 && (millis() - lastCharTime > commandTimeout)) {
    
    // Clean the command
    voiceCommand.toLowerCase();
    voiceCommand.trim();
    
    // Remove extra spaces
    while(voiceCommand.indexOf("  ") >= 0) {
      voiceCommand.replace("  ", " ");
    }
    
    // Display received command (if Serial connected)
    if(Serial) {
      Serial.println("┌──────────────────────────────────────────────────┐");
      Serial.print("│ Received: ");
      Serial.println(voiceCommand);
      Serial.println("├──────────────────────────────────────────────────┤");
    }
    
    if(voiceCommand == "1" ||
       voiceCommand == "on" ||
       
       // Basic commands
       voiceCommand.indexOf("light on") >= 0 ||
       voiceCommand.indexOf("lights on") >= 0 ||
       voiceCommand.indexOf("led on") >= 0 ||
       voiceCommand.indexOf("bulb on") >= 0 ||
       voiceCommand.indexOf("lamp on") >= 0 ||
       
       // Hindi style commands - "karo"
       voiceCommand.indexOf("on karo") >= 0 ||
       voiceCommand.indexOf("on kar") >= 0 ||
       
       // Hindi style commands - "kar do"
       voiceCommand.indexOf("on kar do") >= 0 ||
       voiceCommand.indexOf("on kardo") >= 0 ||
       
       // Chalu commands
       voiceCommand.indexOf("chalu karo") >= 0 ||
       voiceCommand.indexOf("chalu kar") >= 0 ||
       voiceCommand.indexOf("chalu kar do") >= 0 ||
       voiceCommand.indexOf("chalu kardo") >= 0 ||
       voiceCommand.indexOf("chalu") >= 0 ||
       
       // Jala do commands
       voiceCommand.indexOf("jala do") >= 0 ||
       voiceCommand.indexOf("jala de") >= 0 ||
       voiceCommand.indexOf("jalao") >= 0 ||
       voiceCommand.indexOf("jala lo") >= 0 ||
       
       // English style
       voiceCommand.indexOf("turn on") >= 0 ||
       voiceCommand.indexOf("switch on") >= 0 ||
       voiceCommand.indexOf("power on") >= 0) {
      
      relayON();
      
      if(Serial) {
        Serial.println("│ ★ ACTION: LIGHT ON                              │");
        Serial.println("│ ★ JARVIS: Right away Sir, Light is ON!          │");
        Serial.println("└──────────────────────────────────────────────────┘");
      }
      
      Serial1.println("Light ON");
    }
    
    else if(voiceCommand == "0" ||
            voiceCommand == "off" ||
            voiceCommand == "of" ||
            
            // Basic commands
            voiceCommand.indexOf("light off") >= 0 ||
            voiceCommand.indexOf("lights off") >= 0 ||
            voiceCommand.indexOf("led off") >= 0 ||
            voiceCommand.indexOf("bulb off") >= 0 ||
            voiceCommand.indexOf("lamp off") >= 0 ||
            
            // Voice recognition mistakes (off → of)
            voiceCommand.indexOf("light of") >= 0 ||
            voiceCommand.indexOf("lights of") >= 0 ||
            voiceCommand.indexOf("led of") >= 0 ||
            voiceCommand.indexOf("bulb of") >= 0 ||
            voiceCommand.indexOf("lamp of") >= 0 ||
            
            // Hindi style commands - "karo"
            voiceCommand.indexOf("off karo") >= 0 ||
            voiceCommand.indexOf("off kar") >= 0 ||
            voiceCommand.indexOf("of karo") >= 0 ||
            voiceCommand.indexOf("of kar") >= 0 ||
            
            // Hindi style commands - "kar do"
            voiceCommand.indexOf("off kar do") >= 0 ||
            voiceCommand.indexOf("off kardo") >= 0 ||
            voiceCommand.indexOf("of kar do") >= 0 ||
            voiceCommand.indexOf("of kardo") >= 0 ||
            
            // Band commands
            voiceCommand.indexOf("band karo") >= 0 ||
            voiceCommand.indexOf("band kar") >= 0 ||
            voiceCommand.indexOf("band kar do") >= 0 ||
            voiceCommand.indexOf("band kardo") >= 0 ||
            voiceCommand.indexOf("bnd karo") >= 0 ||
            voiceCommand.indexOf("bnd kar") >= 0 ||
            voiceCommand.indexOf("band") >= 0 ||
            
            // Bujha do commands
            voiceCommand.indexOf("bujha do") >= 0 ||
            voiceCommand.indexOf("bujha de") >= 0 ||
            voiceCommand.indexOf("bujhao") >= 0 ||
            voiceCommand.indexOf("bujha lo") >= 0 ||
            
            // English style
            voiceCommand.indexOf("turn off") >= 0 ||
            voiceCommand.indexOf("turn of") >= 0 ||
            voiceCommand.indexOf("switch off") >= 0 ||
            voiceCommand.indexOf("switch of") >= 0 ||
            voiceCommand.indexOf("power off") >= 0 ||
            voiceCommand.indexOf("power of") >= 0) {
      
      relayOFF();
      
      if(Serial) {
        Serial.println("│ ★ ACTION: LIGHT OFF                             │");
        Serial.println("│ ★ JARVIS: Done Sir, Light is now OFF!           │");
        Serial.println("└──────────────────────────────────────────────────┘");
      }
      
      Serial1.println("Light OFF");
    }
    
    else if(voiceCommand.indexOf("status") >= 0 ||
            voiceCommand.indexOf("kya haal") >= 0 ||
            voiceCommand.indexOf("kaisa hai") >= 0 ||
            voiceCommand.indexOf("light status") >= 0 ||
            voiceCommand.indexOf("kya chal raha") >= 0 ||
            voiceCommand.indexOf("state") >= 0) {
      
      if(Serial) {
        Serial.print("│ ★ JARVIS: Sir, light is currently ");
        if(lightState) {
          Serial.println("ON          │");
        } else {
          Serial.println("OFF         │");
        }
        Serial.println("└──────────────────────────────────────────────────┘");
      }
      
      if(lightState) {
        Serial1.println("Light is ON");
      } else {
        Serial1.println("Light is OFF");
      }
    }
    
    else {
      if(Serial) {
        Serial.println("│ ✗ Unknown command                                │");
        Serial.println("│   Try: 'light on' or 'light off'                 │");
        Serial.println("└──────────────────────────────────────────────────┘");
      }
      
      Serial1.println("Unknown command");
    }
    
    if(Serial) {
      Serial.println();
    }
    
    // Clear command for next input
    voiceCommand = "";
  }
}