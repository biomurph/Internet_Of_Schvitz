

byte LCD_status;
boolean LCD_initialised = false;


void printStateToLCD(){  // float inTemp, float outTemp, float amps){
  LCD_Command(CLEAR_DISP_CMD);    // CLEAR DISPLAY
  LCD_Command(HOME_CMD);    // MOVE HOME
  if(schvitzing){
    LCD_printStr("Schvitzing! :)");
    LCD_printFloat(thisI);
    LCD_printStr("A");
  } else {
    LCD_printStr("Not Schvitzing, :(");
  }
  LCD_format(1,0x01);
  LCD_printStr("t1");
  if(t1){
    LCD_printFloat(latest_t1);
  } else {
    LCD_printStr("XXxX");
  }
  LCD_format(1,0x0B);
  LCD_printStr("t2");
  if(t2){
    LCD_printFloat(latest_t2);
  } else {
    LCD_printStr("XXxX");
  }
}


//byte LCD_Read(int n){  
//  byte DATA;  
//  
//  Wire.beginTransmission(LCD_ADDRESS);
//  Wire.write(DISP_CMD);
//  Wire.endTransmission();
//  Wire.requestFrom(LCD_ADDRESS,n);
////  Wire.write(0x00);
//  while(Wire.available())
//  {
//   DATA = Wire.read();
//  // DATA = Wire.receive();
//  }
//  return(DATA);
//}



// send a command to the LCD screen
void LCD_Command(byte data){  

 Wire.beginTransmission(LCD_ADDRESS);
 Wire.write(DISP_CMD);
 Wire.write(data);
 checkWireStatus(Wire.endTransmission());
 delay(1);
 
}

// print a string to the LCD screen
void LCD_printStr(String x){
  Wire.beginTransmission(LCD_ADDRESS);  //0x78
  Wire.write(RAM_WRITE_CMD);               //  ?
  for (int s=0; s<=x.length(); s++){
    Wire.write(x.charAt(s));                  // write ascii
  }
  checkWireStatus(Wire.endTransmission());
  delay(1);
}

// print an int to the LCD screen
void LCD_printInt(int x){
//  Serial.println("in LCD_printInt()");
  boolean digits = true;
  int digitCounter = 0;
  byte thisDigit;    
  char intBuffer[5];
  while(digits){  
    intBuffer[digitCounter] = x%10;
    intBuffer[digitCounter] += '0';
    digitCounter++;
    x /= 10;
    if(x <= 0){
//      Serial.println("!digits");
      digits = false;
    }
  }
  Wire.beginTransmission(LCD_ADDRESS);  //0x78
  Wire.write(RAM_WRITE_CMD);            // Write to the display
  for(int i=digitCounter-1; i>=0; i--){
  Wire.write(intBuffer[i]);
//  Serial.println(intBuffer[i] - '0');
  }
  checkWireStatus(Wire.endTransmission());
  delay(1);
}

// print a float (1/10 resolution) to the LCD screen
void LCD_printFloat(float f){
//  Serial.println("in printFloat()");
  boolean digits = true;
  int digitCounter = 0;
  byte thisDigit;    
  char Buffer[5];
  int convertedF = int(f);
  float tenths = (f - convertedF)*10;
  
  Buffer[digitCounter] = char(tenths)%10;
  Buffer[digitCounter] += '0';
  digitCounter++;
  Buffer[digitCounter] = '.';
  digitCounter++;
  while(digits){  
    Buffer[digitCounter] = convertedF%10;
    Buffer[digitCounter] += '0';
    digitCounter++;
    convertedF /= 10;
    if(convertedF <= 0){
//      Serial.println("!digits");
      digits = false;
    }
  }
  Wire.beginTransmission(LCD_ADDRESS);  //0x78
  Wire.write(RAM_WRITE_CMD);            // Write to the display
  for(int i=digitCounter-1; i>=0; i--){
  Wire.write(Buffer[i]);
//  Serial.println(Buffer[i] - '0');
  }
  checkWireStatus(Wire.endTransmission());
  delay(1);
}


// initialize LCD screen 
void ST7036_init(){  
  Wire.beginTransmission(LCD_ADDRESS);  // 0x78
  Wire.write(DISP_CMD);    
  Wire.write(0x38);    // Function Set (8-bit,2-line,height0,selectTable 00)
  delay(10);
  Wire.write(0x39);    // Function Set (8-bit,2-line,height0,selectTable 01)
  delay(10);
  Wire.write(0x14);    // 1/5 Bias, FX=0
  Wire.write(0x5E);    // ICON on,Booster on, high contrast bits C5,C4
  Wire.write(0x6D);    // follower on, Rab ratio 0-7
  Wire.write(0x73);    // Set contrast bits C3,C2,C1,C0
  Wire.write(0x0C);    // Display on, cursor on, position off
  Wire.write(CLEAR_DISP_CMD);    // CLEAR DISPLAY
  Wire.write(HOME_CMD);    // MOVE HOME
  Wire.write(0x06);    //Cursor move on, Display shift off
  checkWireStatus(Wire.endTransmission());

   if(LCD_initialised){
     LCD_Command(HOME_CMD);
     LCD_printStr("Internet Of Schvitz");
     LCD_Command(LINE_2);
     LCD_printStr("v 1.0.0");
     delay(3000);
   } else {
     Serial.println("not initialized");
   }
}

// position cursor on line l and position pos
// line can be 0 or 1
// pos can be 0x00 to 0x13 for displayed segments
// each line is 0x27 segments long for scrolling... if you like that kind of thing
// NOTE needs error checking for out of bounds values
void LCD_format(int l, byte pos){
  if(l > 0){
    pos += 0x40;
  }
  pos |= 0x80;
  LCD_Command(pos);
}

// look for LCD I2C errors
void checkWireStatus(int stat){

  if ( stat == 0 )
   {
      LCD_initialised = true;
   }else{
    Serial.print(" ! LCD Status "); Serial.println(LCD_status,DEC);
    LCD_initialised = false;
   }
}

