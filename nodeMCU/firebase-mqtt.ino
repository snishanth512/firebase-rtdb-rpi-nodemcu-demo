#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <PubSubClient.h>

#define WIFI_SSID "802.11-ntwk"
#define WIFI_PASSWORD "2yiihji3@5"
#define FIREBASE_HOST "iiot-57d9d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "MQoO4tvztVecD9s9RuCVf8lzdezurUmCqL8wbYzm"
const char* mqtt_server = "192.168.0.108";

long randNumber;
int analogVal =0;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;



void callback(char* topic, byte* payload, unsigned int length) {
  String str = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str += (char)payload[i];
  }
  Serial.println();

  int commaIndex = str.indexOf(',');
  int secondCommaIndex = str.indexOf(',', commaIndex + 1);
  int thirdCommaIndex = str.indexOf(',', secondCommaIndex + 1);
  int fourthCommaIndex = str.indexOf(',', thirdCommaIndex + 1);
  int fifthCommaIndex = str.indexOf(',', fourthCommaIndex + 1);
  int sixthCommaIndex = str.indexOf(',', fifthCommaIndex + 1);
  
  String x = str.substring(0, commaIndex);
  String y = str.substring(commaIndex + 1, secondCommaIndex);
  String z = str.substring(secondCommaIndex + 1, thirdCommaIndex); 

  String p1 = str.substring(thirdCommaIndex + 1, fourthCommaIndex); 

  String lt = str.substring(fourthCommaIndex + 1, fifthCommaIndex); 
  String lg = str.substring(fifthCommaIndex + 1, sixthCommaIndex); 
  String al = str.substring(sixthCommaIndex + 1); 
  
  analogVal = analogRead(A0);
  randNumber = random(25,40);
  Firebase.pushFloat("J7-prime/sensors/accX",x.toFloat());
  Firebase.pushFloat("J7-prime/sensors/accY",y.toFloat());
  Firebase.pushFloat("J7-prime/sensors/accZ",z.toFloat());
  Firebase.pushInt("J7-prime/sensors/proximity",p1.toInt());
  Firebase.pushFloat("J7-prime/sensors/latitude",lt.toFloat());
  Firebase.pushFloat("J7-prime/sensors/longitude",lg.toFloat());
  Firebase.pushFloat("J7-prime/sensors/altitude",al.toFloat());
  
  if (Firebase.failed()) {
    Serial.print("Failed to connect");
    Serial.println(Firebase.error());
    Serial.println();
    Serial.println();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("testPub", "hello");
      // ... and resubscribe
      client.subscribe("J7-prime/sensors");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);            
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  {                                       
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         
}

void setup()
{
  Serial.begin(115200);
  delay(10);
  wifiConnect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  randomSeed(analogRead(0));
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(10);
}



void loop()
{  
  if(WiFi.status() != WL_CONNECTED)
  {
    wifiConnect();
  }

  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
