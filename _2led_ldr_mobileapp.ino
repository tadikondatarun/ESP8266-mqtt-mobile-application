#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "connected";
const char* password = "anvesh1998";
#define mqtt_server  "mqtt.dioty.co"
#define OLED_RESET 1
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int value;
WiFiClient espClient;
PubSubClient client(espClient);
char photoresistance[50];
void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int oled()
{
  int value=analogRead(A0);
  display.setCursor(10, 15);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.println("PHOTORESISTANCE=");
  //display.print("  ");
  display.setCursor(26, 26);
  display.setTextSize(3);
  display.print(value);
  display.display(); 
  return value;
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char* a=topic;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if(strcmp(topic,"/taditarun123@gmail.com/led1")==0)
  {
    if((char)payload[0]=='t')
    {
  
    digitalWrite(12, HIGH); 
    
  } else
  {
    digitalWrite(12, LOW);
  } 
  }
  
  if (strcmp(topic,"/taditarun123@gmail.com/led2")==0)
  
    {
      if((char)payload[0]=='t')
      {
    digitalWrite(13, HIGH);
    
  } else 
  {
    digitalWrite(13, LOW); 
}
    }
}


void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
     client.connect("clientId","taditarun123@gmail.com","0f63a696");
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/taditarun123@gmail.com/ldr", "its working");
      client.subscribe("/taditarun123@gmail.com/led1");
      client.subscribe("/taditarun123@gmail.com/led2");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //  client.setCallback(callback);
}

void loop() {

  if (!client.connected())
  {
    reconnect();
    
  }
  
  //int value = analogRead(A0);
 // oled();
 int value1=oled();
  client.loop();
  snprintf (photoresistance, 75, "PHOTORESISTANCE=%d", value1);

client.setCallback(callback);
  Serial.print("Publish message: ");
  //    Serial.println(msg);
  client.publish("/taditarun123@gmail.com/ldr", photoresistance);
  delay(1000);

}
