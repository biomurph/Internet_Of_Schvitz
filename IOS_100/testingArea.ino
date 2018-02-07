

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

  sampleCount = 0;
  sumI = 0;
  start = millis();
  sample = micros();
  
  while(millis() - start < 1000){
    if(micros() - sample >= 1000){
      sample = micros();
      newSample = analogRead(A0);
      newSample -= OFFSET;  // 524
      float sampleSq = float(newSample * newSample);
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

/*
 *  rms/512 = Irms/30
 *
 */
