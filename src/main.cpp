#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
#include <EEPROM.h>

char *ssid;
char *pass;
const char *broker = "192.168.1.111";
const char *outTopic = "PLANTATION";

WiFiClient espClient;
PubSubClient client(espClient);

int cont = 0;

// void setupWifi()
// {
//   delay(100);
//   Serial.print("\nConnecting to");
//   Serial.println(ssid);

//   WiFi.begin(ssid, pass);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(100);
//     Serial.print(".");
//   }

//   Serial.print("\nConnected to");
//   Serial.println(ssid);
// }
// void reconnect()
// {
//   while (!client.connected())
//   {
//     Serial.print("\nConnecting to");
//     Serial.println(broker);
//     if (client.connect("1"))
//     {
//       Serial.print("\nConnected to");
//       Serial.println(broker);
//     }
//     else
//     {
//       Serial.println("\nTrying connect again");
//       delay(5000);
//     }
//   }
// }

// void sendTheStupidMessage()
// {
//   char msg[50];
//   sprintf(msg, "%d", cont);
//   Serial.println("Sending the message");
//   client.publish(outTopic, msg);
//   cont++;
//   if (cont == 100)
//   {
//     cont = 0;
//   }
//   delay(2000);
// }

int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}

int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // the character may appear in a weird way, you should read: 'only one backslash and 0'
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

bool getWifiConnectionData()
{
  String Vssid;
  String Vpass;
  int ssidOffset = readStringFromEEPROM(0, &Vssid);
  if (ssidOffset == 1)
  {
    return false;
  }
  readStringFromEEPROM(ssidOffset, &Vpass);
  char tempSsid[Vssid.length() + 1];
  char tempPass[Vpass.length() + 1];
  Vssid.toCharArray(tempSsid, Vssid.length() + 1);
  Vpass.toCharArray(tempPass, Vpass.length() + 1);
  ssid = tempSsid;
  pass = tempPass;
  return true;
}

void setWifiConnectionData(const String &ssid, const String &password)
{
  int str1AddrOffset = writeStringToEEPROM(0, ssid);
  writeStringToEEPROM(str1AddrOffset, password);
  getWifiConnectionData();
}

void setup()
{
  Serial.begin(9600);
  // setupWifi();
  // client.setServer(broker, 1234);
}

void loop()
{
  setWifiConnectionData("Wa Leleu", "123");

  Serial.print(ssid);
  Serial.print(pass);
  // getWifiConnectionData();
  // if (!client.connected())
  // {
  //   reconnect();
  // }
  // client.loop();
  // sendTheStupidMessage();
}
