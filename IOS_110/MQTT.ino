// MQTT Stuff

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
  if (! errorLog.publish(eLog)) {
    Serial.println(F("IO: errorLog Failed"));
  } else {
//    Serial.println(F("IO: errorLog OK!"));
  }
}

boolean MQTTconnect() {

  if(!checkWiFi()){
    return false;
  }

  // Stop if already connected to MQTT.
  if (mqtt.connected()) {
//    Serial.println("MQTT Already Connected!");
    return true;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while (mqtt.connect() != 0) { // connect will return 0 for connected
//       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("MQTT Connect Fail");
         MQTT_Connected = false;
         logEvent = true;
         return false;
       }
  }
  Serial.println("MQTT Connected!");
  MQTT_Connected = true;
  logEvent = true;
  return true;
}
