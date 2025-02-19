#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>  
#include <WiFiClient.h>  
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

const char* ssid = "TP-Link_4B00";
const char* password = "17227896";

unsigned long myChannelNumber = 2844335;
const char* myWriteAPI = "YCNZ9N7TDP7QRLJ3";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Đang kết nối WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi đã kết nối!");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Lỗi! Không đọc được dữ liệu từ cảm biến DHT.");
    delay(2000);
    return;
  }

  Serial.print("Nhiệt độ: ");
  Serial.print(temperature);
  Serial.println("°C");

  int x = ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPI);
  if (x == 200) {
    Serial.println("Dữ liệu nhiệt độ đã gửi thành công lên ThingSpeak!");
  } else {
    Serial.println("Lỗi khi gửi dữ liệu nhiệt độ lên ThingSpeak!");
  }
  delay(2000);
  Serial.print("Độ ẩm: ");
  Serial.print(humidity);
  Serial.println("%");

  int y = ThingSpeak.writeField(myChannelNumber, 2, humidity, myWriteAPI);
  if (y == 200) {
    Serial.println("Dữ liệu độ ẩm đã gửi thành công lên ThingSpeak!");
  } else {
    Serial.println("Lỗi khi gửi dữ liệu độ ẩm lên ThingSpeak!");
  }

  delay(2000);
}
