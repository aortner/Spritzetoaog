
//Teilbreitenabfrage über Spannungsteiler 12 v zu 5 v an Agopen
// by Andreas Ortner
// Danke an Brian, MTZ and Coffetrac für die Vorlage

#define Sw1       2  // Teilbreiten 1-5
#define Sw2       3  // 
#define Sw3       4  // 
#define Sw4       5
#define Sw5       6

#define Baudrate 38400
#define SC_Serial Serial

int tb  = 0;
int temp;

byte SectMainToAOG = 0;  // output the Switches to AOG
byte SectSWOffToAOG[] = {0, 0};
byte RelayToAOG[] = {0, 0};

// Program flow
unsigned long lastAOG_Receive;
unsigned long lastAOG_Transmit;

const unsigned int LOOP_TIME = 200; //5hz
unsigned int currentTime = LOOP_TIME;
unsigned int lastTime = LOOP_TIME;
byte watchdogTimer = 0;
unsigned int dT = 50000;

bool isDataFound = false, isSettingFound = false;
int header = 0, tempHeader = 0;

// Setup procedure ------------------------
void setup() {
  SC_Serial.begin(Baudrate);

  pinMode(Sw1, INPUT);
  pinMode(Sw2, INPUT);
  pinMode(Sw3, INPUT);
  pinMode(Sw4, INPUT);
  pinMode(Sw5, INPUT);
}



void loop() {


  currentTime = millis();
  unsigned int time = currentTime;

  if (currentTime - lastTime >= LOOP_TIME)
  {
    dT = currentTime - lastTime;
    lastTime = currentTime;

    //increase the watchdog - reset in data recv.
    watchdogTimer++;
    //section read

    tb = 0;
    if (digitalRead(Sw1) == HIGH)tb += 1;
    if (digitalRead(Sw2) == HIGH)tb += 2;
    if (digitalRead(Sw3) == HIGH)tb += 4;
    if (digitalRead(Sw4) == HIGH)tb += 8;
    if (digitalRead(Sw5) == HIGH)tb += 16;

    SectSWOffToAOG[0] = tb;

    transmit_AOG();
  } //end of timed loop
  receive_AOG();
  delay(10);
}
