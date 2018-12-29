
// WiFi Stuff

boolean WiFiConnect(){
  if(WiFi.status() != WL_CONNECTED){
    unsigned long startConnect = millis();
    int connectTimeout = 20000; // 20 second time out
    Serial.println(); Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    WiFi_Connected = true;
  } else if(WiFi.status() == WL_CONNECTED){
    WiFi_Connected = true;
  }
    logEvent = true;
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());
    return WiFi_Connected;

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.

boolean checkWiFi(){
    // Check WiFi status
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("checkWiFi: not connected");
    WiFiConnect();
    if(WiFi.status() != WL_CONNECTED){
      Serial.println("checkWiFi: try failed");
      WiFi_Connected = false;
      logEvent = true;
      return false;
    }
  }
  return true;
}


