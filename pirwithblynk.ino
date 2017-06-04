#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#include <ESP8266WiFi.h>
#include <BlynkSimpleStream.h>

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "ssid";
const char* pass = "pwd";

#define ledPin BUILTIN_LED
#define pirPin 2 // Input for HC-S501
int pirValue; // Place to store read PIR Value
int sentNotification = false;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "token";

WiFiClient wifiClient;

// This function tries to connect to the cloud using TCP
bool connectBlynk()
{
  wifiClient.stop();
  return wifiClient.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
}

// This function tries to connect to your WiFi network
void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


void setup()
{
  // Debug console
  Serial.begin(115200);

  delay(10);

  // setup GPIO pins
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);


  connectWiFi();

  connectBlynk();

  Blynk.begin(wifiClient, auth);
}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Reconnect to Blynk Cloud
  if (!wifiClient.connected()) {
    connectBlynk();
    return;
  }

  getPirValue();
  Blynk.run();

}



/***************************************************
   Get PIR data
 **************************************************/
void getPirValue(void)
{
  pirValue = digitalRead(pirPin);
  if (pirValue)
  {
    if (!sentNotification) {
      Serial.println("==> Motion detected");
      Blynk.notify("T==> Motion detected");
      sentNotification = true;
    }
  } else {
    sentNotification = false;
  }
  digitalWrite(ledPin, pirValue);
}
