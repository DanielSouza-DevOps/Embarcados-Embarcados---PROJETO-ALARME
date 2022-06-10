#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
/*-------- Configurações de Wi-fi----------- */
const char* ssid = "id";  /* Substitua pelo nome da rede */
const char* password =  "senha";    /* Substitua pela senha */
const char* mqtt_server = "184.73.163.145";
               

WiFiClient espClient;
PubSubClient client(espClient);


const int lamp = 2;

long now = millis();
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(topic=="Alarme/ifsp"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, HIGH);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, LOW);
        Serial.print("Off");
      }
  }
  Serial.println();
}
 
void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
  
    if (client.connect("ESPClient")) {
      Serial.println("connected");  
      client.subscribe("Alarme/ifsp");
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
  pinMode(lamp, OUTPUT);  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESPClient");
}
