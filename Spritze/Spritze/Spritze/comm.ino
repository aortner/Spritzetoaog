void transmit_AOG() {
  //Send to agopenGPS, 5Hz per second

  SC_Serial.print(0); //100 x actual!
  SC_Serial.print(",");
  SC_Serial.print(0); //100 x actual!
  SC_Serial.print(",");
  SC_Serial.print(0);

  SC_Serial.print(",");
  SC_Serial.print(RelayToAOG[1]);
  SC_Serial.print(",");
  SC_Serial.print(RelayToAOG[0]);
  SC_Serial.print(",");
  SC_Serial.print(SectSWOffToAOG[1]);
  SC_Serial.print(",");
  SC_Serial.print(SectSWOffToAOG[0]);
  SC_Serial.print(",");
  SC_Serial.print(SectMainToAOG);
  SC_Serial.println();
  // flush out buffer
  SC_Serial.flush();
  lastAOG_Transmit = millis();
}

//******************************************************************
void receive_AOG() {
  if (SC_Serial.available() > 0 && !isDataFound && !isSettingFound) //find the header,
  {
    byte temp = SC_Serial.read();
    //Serial.print(temp);
    header = tempHeader << 8 | temp;               //high,low bytes to make int
    tempHeader = temp;                             //save for next time
    if (header == 32762) isDataFound = true;     //Do we have a match?
    if (header == 32760) isSettingFound = true;     //Do we have a match?
  }

  //DATA Header has been found, so the next 4 bytes are the data -- 127H + 250L = 32762
  if (SC_Serial.available() > 0 && isDataFound)
  {
    isDataFound = false;
    temp = SC_Serial.read();   // read relay control from AgOpenGPS
    temp = SC_Serial.read();   // read relay control from AgOpenGPS
    temp = SC_Serial.read() >> 2;  //actual speed times 4, single byte
    // sent as 100 times value in liters per minute
    temp = SC_Serial.read();
    temp = SC_Serial.read();
    temp = SC_Serial.read();
    temp = SC_Serial.read();
    //UTurn byte
    temp = SC_Serial.read();
    //reset watchdog as we just heard from AgOpenGPS
    watchdogTimer = 0;
    lastAOG_Receive = millis();
  }

  //SETTINGS Header has been found,  6 bytes are the settings -- 127H + 248L = 32760
  if (SC_Serial.available() > 0 && isSettingFound)
  {
    isSettingFound = false;  //reset the flag

    //accumulated volume, 0 it if 32700 is sent
    temp = SC_Serial.read();
    temp = SC_Serial.read();


    //flow meter cal factor in counts per Liter
    temp = SC_Serial.read();
    temp = SC_Serial.read();
    temp = SC_Serial.read();
    temp = SC_Serial.read();

  }
}


//******************************************************************
void flush_serial() {    //clean out serial buffer
  while (SC_Serial.available() > 0) char t = SC_Serial.read();
  watchdogTimer = 0;
}
