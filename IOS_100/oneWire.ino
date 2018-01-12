



void countBirds(){

  // locate devices on the bus
  Serial.print("Locating thermomnometers...");
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" thermizors.");

  // report parasite power requirements
  Serial.print("Parasitic power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(Therm1, 0)){
    Serial.println("Unable to find address for Device 0");
  }else{
    t1 = true;
  }
  if (!sensors.getAddress(Therm2, 1)){
    Serial.println("Unable to find address for Device 1");
  }else{
    t2 = true;
  }
   // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(Therm1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(Therm2);
  Serial.println();
  
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(Therm1), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(Therm2), DEC);
  Serial.println();

// call sensors.requestTemperatures() to issue a global temperature
// request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

// print the device information
  printData(Therm1);
  printData(Therm2);
//

}



// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

float printFahrenheit(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  return (DallasTemperature::toFahrenheit(tempC));
}
float printCelcius(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  return tempC;
}

