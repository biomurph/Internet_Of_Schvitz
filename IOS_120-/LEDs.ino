
void fadeLEDs(){
    float f = map(latest_t1,-10.0,250.0,1023.0,0.0);
    setPWMfade(f);
}

void setPWMfade(float _t){
  int t = _t; // expects _t to be 0<>1023
  
  for(int i=0; i<3; i++){
    targetPWM[i] = int(map(t, 0,1023, seed0[i],seed1[i]));
    analogWrite(targetLED[i],targetPWM[i]);
//    Serial.print(targetLED[i]); Serial.print(", "); 
//    Serial.print(targetPWM[i]); Serial.print("\t");
  }
//  Serial.println();

}

void LEDwhite(){
  analogWrite(LCD_RED,MAX_LED);
  analogWrite(LCD_GREEN,MAX_LED);
  analogWrite(LCD_BLUE,MAX_LED);  
}

void testLCD_LEDs(){
  rgb++;
  unsigned int token = rgb%3;
  
  switch(token){
    case 0:
      analogWrite(LCD_RED,MAX_LED);
      analogWrite(LCD_BLUE,MIN_LED);
      analogWrite(LCD_GREEN,MIN_LED);
    break;
    case 1:
      analogWrite(LCD_RED,MIN_LED);
      analogWrite(LCD_BLUE,MAX_LED);
      analogWrite(LCD_GREEN,MIN_LED);
    break;
    case 2:
      analogWrite(LCD_RED,MIN_LED);
      analogWrite(LCD_BLUE,MIN_LED);
      analogWrite(LCD_GREEN,MAX_LED);
    break;
    default: break;
  }
}

void LEDoff(){
  analogWrite(LCD_RED,MIN_LED);
  analogWrite(LCD_GREEN,MIN_LED);
  analogWrite(LCD_BLUE,MIN_LED);  
}
