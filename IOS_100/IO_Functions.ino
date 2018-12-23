



void sendToIO(){
  MQTT_connect();
  if (! insideSauna.publish(latest_t1)) {
    Serial.println(F("IO: t1 Failed"));
  } else {
    Serial.println(F("IO: t1 OK!"));
  }
  if (! outsideSauna.publish(latest_t2)) {
    Serial.println(F("IO: t2 Failed"));
  } else {
    Serial.println(F("IO: t2 OK!"));
  }
  if (! heaterCurrent.publish(Irms)) {
    Serial.println(F("IO: Irms Failed"));
  } else {
    Serial.println(F("IO: Irms OK!"));
  }
}

boolean WiFiConnect(){
  unsigned long startConnect = millis();
  int connectTimeout = 20000; // 20 second time out
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(millis()-startConnect > connectTimeout){
      WiFiConnected = false;
      return WiFiConnected;
    }
  }
  WiFiConnected = true;
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  return WiFiConnected;
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
boolean MQTT_connect() {

  // Check WiFi status before trying to MQTT 
  if(WiFi.status() != WL_CONNECTED){
    if(WiFiConnected){
      WiFiConnected = false;
      WiFiDisconnectTime = millis();
    }
    return false;
  }

  // Stop if already connected to MQTT.
  if (mqtt.connected()) {
    Serial.println("MQTT Already Connected!");
    return true;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("MQTT Connect Fail");
         MQTTconnected = false;
         MQTTdisconnectTime = millis();
         return false;
       }
  }
  Serial.println("MQTT Connected!");
  MQTTconnected = true;
  return true;
}
