// Notes at the bottom of this page

float calcIrms(){
  float sampleSq;
  sampleCount = 0;
  sumI = 0;
  startTime = millis();
  sample = micros();
  
  while(millis() - startTime < 1000){
    if(micros() - sample >= 1000){
      sample = micros();
      newSample = analogRead(A0);
      newSample -= OFFSET;  // 524
      sampleSq = float(newSample * newSample);
      sumI += sampleSq;
      sampleCount++;
    }
  }
  Irms = sqrt(sumI/sampleCount);
//  Serial.print("\t"); Serial.print(Irms); Serial.print("\t");
  Irms *= 1.414;
  Irms /= 512.0;
  Irms *= 30.0;
//  Serial.print(Irms); Serial.print("\t");
//  Serial.print(Irms); Serial.print("\t");
//  Serial.println();
  Irms10x = Irms*10.0;
  return Irms;
}


void readAndPrintIsensor(){
  unsigned long sample, startTime;
  int sumSamples = 0;
  int meanSamples;
  int S;
  int numSamples = 0;
  startTime = millis();
  sample = micros();
  while(millis() - startTime < 1000){
    if(micros() - sample >= 1000){
      sample = micros();
      S = analogRead(A0);
      numSamples++;
      sumSamples += S;
      Serial.println(S);
    }
  }
  meanSamples = sumSamples / numSamples;
  Serial.print("average of "); Serial.print(numSamples);
  Serial.print(" = "); Serial.println(meanSamples);
}




/*
 * Convert maximum RMS current to peak-current by multiplying by √2.
     Primary peak current = √2 x Primary RMS_Peak_current
     1.414 x 30 A = 42.42A

  Divide the Primary_peak-current by the number of turns in the CT to give the peak-current in the secondary coil
    The YHDC SCT-013-000 CT has 2000 turns, so the secondary peak current will be:
    Secondary peak current = Primary peak current / no. of turns
    42.42A / 2000 = 0.02121A

  To maximise measurement resolution, the voltage across the burden resistor at peak-current should = 0.5 of AREF ~1V
    Ideal load resistance = (AREF/2) / Secondary peak-current
    0.500V / 0.02121A = 23.57Ω
    0.482V / 0.02121A = 22.72Ω
    Settle on 22Ω on the short side


  Calibration = CT Ratio / Burden resistance 
  (30A / 0.02121A) / 22 Ohms = 64.29

    More cray here https://learn.openenergymonitor.org/electricity-monitoring/ctac/calibration

 * 
 * 
 * 
 * 
 * 
 * 
 */
