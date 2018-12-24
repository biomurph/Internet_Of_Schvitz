

byte getEEdress(){
  byte b = EEPROM.read(0);
  if(b == 0xFF){        // if EEPROM is all 0xFF, then it is fresh
    EEdress = 0x01;     // EEdress is always the next available spot in EEPROM
    EEPROM.write(0,EEdress);
    EEPROM.commit();
  }else{
    EEdress = b;
  }
  return EEdress;
}

void writeEvent(){
//  time_t t = now();
  byte e = 0x00;
  EEPROM.write(EEdress,byte(hour())); EEdress++;
  EEPROM.write(EEdress,byte(minute())); EEdress++;
  EEPROM.write(EEdress,byte(second())); EEdress++;
  bitWrite(e, 0, WiFi_Connected);
  bitWrite(e, 3, MQTT_Connected);
  bitWrite(e, 4, NTP_Set); 
  bitWrite(e, 7, NTP_Sync);
  EEPROM.write(EEdress,e); EEdress++; // +4
  if(EEdress > 251){ EEdress = 0x01; }
  EEPROM.write(0,EEdress);
  EEPROM.commit();
}

void readEvents(){
  byte b[4];
  Serial.print("Reading EEPROM to EEdress "); Serial.print(EEdress,DEC);
  Serial.print(" 0x"); Serial.println(EEPROM.read(0),HEX);
  if(EEdress < 5){
    Serial.println("No Errors!");
    return;
  }
  for(int i=1; i<EEdress; i+=4){
    for(int j=0; j<4; j++){
      b[j] = EEPROM.read(i+j);
    }
    Serial.print(b[0],DEC); Serial.print(":");
    Serial.print(b[1],DEC); Serial.print(":");
    Serial.print(b[2],DEC); Serial.print(" 0x");
    Serial.print(b[3],HEX); Serial.println();
  }
}

void EEerace(){
  for(int i=0; i<EEPROM.length(); i++){
    EEPROM.write(i,0xFF);
  }
  EEPROM.commit();
  getEEdress();
}


