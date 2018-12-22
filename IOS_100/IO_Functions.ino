



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

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    Serial.println("MQTT Already Connected!");
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("MQTT Connect Fail");
         return;
       }
  }
  Serial.println("MQTT Connected!");
}
