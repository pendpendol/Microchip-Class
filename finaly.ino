#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiManager.h> 
  WiFiClient client ;
  HTTPClient http;
  WiFiManager wm;


//Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
//#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//DHT
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Deklarasi variabel
  int cold    = 18;
  int hot     = 40;
  int IR      = A0;
  int value;
  int night   = 200;
  int merah   = D8;
  int kuning  = D7;
  int hijau   = D6;
  int buzzer  = D0;
  
const char server[] = "http://pend.rsxms.com/receiver.php?";
   
void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
   WiFi.mode(WIFI_STA);
   wm.resetSettings();
   Serial.begin(115200); 
   bool res;
    
    res = wm.autoConnect("SmartAquarium","password"); // Ubah SSID dan password sesuai yang kalian mau

    if(!res) {
        Serial.println("Failed to connect");
       
    } 
    else {
   
        Serial.println("connected...yeey :)");
    }


  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  dht.begin();

  pinMode(IR, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(merah, OUTPUT);
  pinMode(kuning, OUTPUT);
  pinMode(hijau, OUTPUT);
}

String statuss(){
  if (value > night) {
    String stat = "Siang";
    return stat;
  } else {
    String stat = "Malam";
    return stat; 
  }
}

void loop() {
  display.setTextSize(1);
  
  //Membaca sensor Infra Red & DHT
  value = analogRead(IR);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(WiFi.status()== WL_CONNECTED){


  http.begin(client, server);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String httpRequestData = "Humidity="+ (String) h
  + "&Temperature= " + (String)t
  + "& Day=" + statuss();
  
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  int httpResponseCode = http.POST(httpRequestData);

  if(httpResponseCode>0) {
    Serial.print("HTTP Response Code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print ("Error Code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
else {
  Serial.println("WiFi Disconnected");
}
delay(3000);
  
  if (isnan(h) || isnan(t)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //layar
  display.setCursor(10,0);
  display.println("Michrochip System");
  
  //Kelembaban Sekitar
  Serial.print("Humidity : ");
  Serial.print(h);
  Serial.println(" %");
 
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Humidity   : ");
  display.print(h);
  display.print(" %");

if (t > cold && t < hot){
     display.setCursor(0, 30);
     display.print("Temperature: ");
     display.print(t);
     display.println(" C");
     display.setCursor(25, 40);
     display.println("Suhu Normal");
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" C");
     Serial.println("Suhu Normal");
     digitalWrite(kuning, HIGH);
     delay       (1000);
     digitalWrite(kuning, LOW);    
  }
  
  if (t > hot){
     display.setCursor(0, 30);
     display.print("Temperature: ");
     display.print(t);
     display.println(" C");
     display.setCursor(25, 40);
     display.println("Suhu Tinggi");
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" C");
     Serial.println("Suhu Tinggi");
       digitalWrite(buzzer, HIGH);
       digitalWrite(merah, HIGH);
       delay       (1000);
       digitalWrite(merah, LOW);
       
       
    }
    if (t < cold){
     display.setCursor(0, 30);
     display.print("Temperature: ");
     display.print(t);
     display.println(" C");
     display.setCursor(25, 40);
     display.println("Suhu Rendah");
     Serial.print("Temperature : ");
     Serial.print(t);
     Serial.println(" C");
     Serial.println("Suhu Rendah");
       digitalWrite(hijau, HIGH);
       delay       (1000);
       digitalWrite(hijau, LOW);
    }
    
    if ( value > night){
      display.setCursor(30,50);
      display.println("MALAM HARI");
      Serial.println("MALAM HARI");
      Serial.println(value);
    }
    
    else {
    display.setCursor(30,50);
    display.println("SIANG HARI");
    Serial.println("SIANG HARI");
    Serial.println(value); 
    }
    
  display.display();
  delay(1000);
  display.clearDisplay();
       

  }
  
