



void sendToIO(){
  MQTTconnect();
  
  if (! outsideSauna.publish(latest_t2)) {
    Serial.println(F("IO: t2 Failed"));
  } else {
//    Serial.println(F("IO: t2 OK!"));
  }
  if (! heaterCurrent.publish(Irms)) {
    Serial.println(F("IO: Irms Failed"));
  } else {
//    Serial.println(F("IO: Irms OK!"));
  }
  if (! insideSauna.publish(latest_t1)) {
    Serial.println(F("IO: t1 Failed"));
  } else {
//    Serial.println(F("IO: t1 OK!"));
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
      WiFi_Connected = false;
      writeEvent();
      printStateToLCD();
      return WiFi_Connected;
    }
  }
  WiFi_Connected = true;
  writeEvent();
  printStateToLCD();
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  return WiFi_Connected;
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
boolean MQTTconnect() {

  // Check WiFi status before trying to MQTT 
  if(WiFi.status() != WL_CONNECTED){
    if(WiFi_Connected){  // just set this once
      WiFi_Connected = false;
      writeEvent();
      printStateToLCD();
    }
    return false;
  }

  // Stop if already connected to MQTT.
  if (mqtt.connected()) {
//    Serial.println("MQTT Already Connected!");
    return true;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
//  int8_t ret;
  while (mqtt.connect() != 0) { // connect will return 0 for connected
//       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("MQTT Connect Fail");
         MQTT_Connected = false;
         writeEvent();
         printStateToLCD();
         return false;
       }
  }
  Serial.println("MQTT Connected!");
  MQTT_Connected = true;
  writeEvent();
  printStateToLCD();
  return true;
}
