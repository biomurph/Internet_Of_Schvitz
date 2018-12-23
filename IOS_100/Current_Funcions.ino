

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
 *  rms/512 = Irms/30
 *
 */