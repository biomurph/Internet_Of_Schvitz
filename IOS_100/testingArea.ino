

int offset = 524;

void readAndPrintIsensor(){
  unsigned long sample, start;
  int sumSamples = 0;
  int meanSamples;
  int S;
  int numSamples = 0;
  start = millis();
  sample = micros();
  while(millis() - start < 1000){
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



float calcIrms(){
  unsigned long sample, start;
  int newSample;
  int sampleCount = 0;
  long sumI = 0;
  
  start = millis();
  sample = micros();
  while(millis() - start < 1000){
    if(micros() - sample >= 2000){
      sample = micros();
      newSample = analogRead(A0);
      newSample -= offset;
      float sampleSq = float(newSample * newSample);
      sumI += sampleSq;
      sampleCount++;
    }
  }
  Irms = sqrt(sumI/sampleCount);
  return Irms;
}
