#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YOUR_WIFI_NAME_HERE";
char pass[] = "YOUR_WIFI_PASSWORD_HERE";

#define DATA_SIZE 45
char data_buffer[DATA_SIZE];
char *saveptr;

int actuatorw1 = 12;
int actuatorw2 = 14;

BlynkTimer timer;

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode(actuatorw1, OUTPUT);
  pinMode(actuatorw2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(actuatorw1, LOW);
  digitalWrite(actuatorw2, LOW);

  digitalWrite(LED_BUILTIN, LOW);

  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  Serialcheck();
  timer.run();
}


BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  if (pinValue == 1)
  {
    Door_Lock();
  }

  else if (pinValue == 0)
  {
    Door_Unlock();
  }
}

void Door_Lock()
{
  digitalWrite(actuatorw1, HIGH);
  digitalWrite(actuatorw2, LOW);
  timer.setTimeout(2000L, Actuatoroff); // after 2 seconds actuator will turn off
}

void Door_Unlock()
{
  digitalWrite(actuatorw1, LOW);
  digitalWrite(actuatorw2, HIGH);
  timer.setTimeout(2000L, Actuatoroff); // after 2 seconds actuator will turn off
}

void Actuatoroff()
{
  digitalWrite(actuatorw1, LOW);
  digitalWrite(actuatorw2, LOW);
}

void Serialcheck()
{
  memset(data_buffer, 0 , sizeof(data_buffer));
  if (Serial.available() > 0)
  {
    Serial.readBytes(data_buffer, DATA_SIZE);
    String command = strtok_r(data_buffer, ",", &saveptr);

    if (command == "door")
    {
      char* errorCheck;
      long DoorStatus = strtol(strtok_r(NULL, ",", &saveptr), &errorCheck, 10);

      if (DoorStatus == 1)
      {
        Serial.println("LOCK");
        Door_Lock();
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if (DoorStatus == 0)
      {
        Serial.println("UNLOCK");
        Door_Unlock();
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    else if (command == "reset")
    {
      ESP.restart();
    }
  }
}