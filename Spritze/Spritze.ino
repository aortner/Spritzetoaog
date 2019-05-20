
//Teilbreitenabfrage über Spannungsteiler 12 v zu 5 v an Agopen
// by Andreas Ortner
// Danke an Brian, MTZ and Coffetrac für die Vorlage

#define Sw1       2  // Teilbreiten 1-7
#define Sw2       3  // 
#define Sw3       4  // 
#define Sw4       5
#define Sw5       6
//#define Sw6       7
//#define Sw7       8

#define Input_Mode INPUT // oder INPUT (5 V am Eingang) oder INPUT_PULLUP (GND gegen Port)

#define Baudrate 38400
#define SC_Serial Serial

int tbon  = 0;
int tboff = 0;
int temp;

byte SectMainToAOG = 0;  // output the Switches to AOG
byte SectSWOffToAOG[] = {0, 0};
byte RelayToAOG[] = {0, 0};

// Program flow
unsigned long lastAOG_Receive;
unsigned long lastAOG_Transmit;

const unsigned int LOOP_TIME = 1000; //1hz
unsigned int currentTime = LOOP_TIME;
unsigned int lastTime = LOOP_TIME;
byte watchdogTimer = 0;
unsigned int dT = 50000;

bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0;

// Setup procedure ------------------------
void setup() {
  SC_Serial.begin(Baudrate);

  pinMode(Sw1, Input_Mode);
  pinMode(Sw2, Input_Mode);
  pinMode(Sw3, Input_Mode);
  pinMode(Sw4, Input_Mode);
  pinMode(Sw5, Input_Mode);
  //  pinMode(Sw6, Input_Mode);
  //  pinMode(Sw7, Input_Mode);
}



void loop() {

  tbon = 0;
  tboff = 0;

  if (digitalRead(Sw1) == HIGH)tboff += 1;
  else tbon += 1;
  if (digitalRead(Sw2) == HIGH)tboff += 2;
  else tbon += 2;
  if (digitalRead(Sw3) == HIGH)tboff += 4;
  else tbon += 4;
  if (digitalRead(Sw4) == HIGH)tboff += 8;
  else tbon += 8;
  if (digitalRead(Sw5) == HIGH)tboff += 16;
  else tbon += 16;
  // if (digitalRead(Sw6) == HIGH)tboff += 32;
  // else tbon += 32;
  // if (digitalRead(Sw7) == HIGH)tboff += 64;
  // else tbon += 64;



  if (tbon !=  SectSWOffToAOG[0] || tboff != RelayToAOG[0] ) {
    SectSWOffToAOG[0] = tbon;
    // RelayToAOG[0] =  tboff;  // wenn das hier ausgeschaltet ist wir in agopen angezeigt ob der Schalter aktiv ist (unten). die teilbreiten werden von agopen geschaltet

    transmit_AOG();
  }


  currentTime = millis();
  unsigned int time = currentTime;

  if (currentTime - lastTime >= LOOP_TIME)
  {
    dT = currentTime - lastTime;
    lastTime = currentTime;

    //increase the watchdog - reset in data recv.
    watchdogTimer++;

    transmit_AOG();

  } //end of timed loop


  receive_AOG();
  delay(10);
}
