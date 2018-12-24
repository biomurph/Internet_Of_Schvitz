

void printRules(){
        Serial.println("\nInternet Of Schvitz  v1.1.0");
        Serial.println("Press 'a' to get raw ADC reading");
        Serial.println("Press 't' to start test");
        Serial.println("Press 'z' to stop test");
        Serial.println("Press 'x' to end schvitz");
        Serial.println("Press 'i' to run currentOnly()");
        Serial.println("Press 's' to schvitz!");
        Serial.println("Press 'b' to find 1 wire temp sensors");
        Serial.println("Press 'd' to measure degrees");
        Serial.println("Press 'm' to check or connect to MQTT");
        Serial.println("Press 'p' to publish data to MQTT");
        Serial.println("Press 'e' to erase EEPROM");
        Serial.println("Press 'r' to read EEPROM");
        Serial.println("Press 'g' to get EEdress");
        Serial.println("Press 'n' to get time info");
        
        Serial.println("Press '?' to print this again");
}

void printToSerial(){
//  csv: temp1 | temp2 | AC_current |
  Serial.print(latest_t1); Serial.print("\t");
  Serial.print(latest_t2); Serial.print("\t");
  Serial.print(Irms); Serial.print("\t");
  printTimeData();
//  Serial.println();
}

void checkSerial(){
  while(Serial.available()){
    inChar = Serial.read();

    switch(inChar){

      case'?':
        printRules();
        break;
      case 'a':
        rawADC = analogRead(A0);
        Serial.print("rawADC = "); Serial.println(rawADC);
        break;
      case 't':
        testing = true;
        testTime = millis();
        Serial.println("starting test");
        break;
      case 'z':
        testing = false;
        Serial.println("end test");
        break;
      case 'x':
        endSchvitz();
        break;
      case 'i':
        Serial.print("Irms = "); Serial.println(Irms);
        break;
      case 's':
        startSchvitz();
        break;
      case 'b':
        countBirds();
        break;  // beak;
      case 'd':  // for degrees...
        Serial.println("getting temps... ");
        sensors.requestTemperatures();
        printData(Therm1);
        printData(Therm2);
        break;
      case 'm':
        MQTTconnect();
        break;
      case 'p':
        sendToIO();
        break;
      case 'e':
        Serial.print("Erasing EEPROM ...");
        EEerace();
        Serial.println("Done!");
        break;
      case 'r':
        readEvents();
        break;
      case 'g':
        Serial.print("You have "); Serial.print((getEEdress()-1)/4,DEC); Serial.println(" errors");
        break;
      case 'n':
        printTimeData();
        break;
      default: Serial.print("echo "); Serial.println(inChar);
        break;
    }
  }
}



