

void getEEpointer(){
  byte b = EEPROM.read(0);
  if(b == 0xFF){        // if EEPROM is all 0xFF, then it is fresh
    EEdress = 0x01;     // EEdress is always the next available spot in EEPROM
    EEPROM.write(1,EEdress);
  }else{
    EEdress = b;
  }
  
}

void writeEEPROM(byte error){
  time_t t = now();
  EEPROM.write
}

void readEEPROM(){
  for(int i=0; i<EEdress; i++){
    
  }
}

void EEerace(){
  for(int i=0; i<EEPROM.length; i++){
    EEPROM.write(i,0xFF);
  }
  EEPROM.commit();
}


