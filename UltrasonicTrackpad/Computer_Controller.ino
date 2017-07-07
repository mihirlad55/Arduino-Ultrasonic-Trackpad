
#define PIN_SONAR_ECHO_XU 8
#define PIN_SONAR_TRIG_XU 7

#define PIN_SONAR_ECHO_XD 3
#define PIN_SONAR_TRIG_XD 2

#define PIN_SONAR_ECHO_YL 11
#define PIN_SONAR_TRIG_YL 10

#define PIN_SONAR_ECHO_YR 13
#define PIN_SONAR_TRIG_YR 12

#define PIN_BTN_LEFT_CLICK 5
#define PIN_BTN_RIGHT_CLICK 4


#define SCREEN_WIDTH 3200
#define SCREEN_HEIGHT 1800
#define RANGE_MAX 12
#define RANGE_MIN 0
#define RANGE (RANGE_MAX - RANGE_MIN)
#define SENSITIVITY  5.0
#define OFFSET 3.0


void setup() {  
  pinMode(PIN_SONAR_TRIG_XU, OUTPUT);
  pinMode(PIN_SONAR_ECHO_XU, INPUT);
  pinMode(PIN_SONAR_TRIG_XD, OUTPUT);
  pinMode(PIN_SONAR_ECHO_XD, INPUT);
  pinMode(PIN_SONAR_TRIG_YL, OUTPUT);
  pinMode(PIN_SONAR_ECHO_YL, INPUT);
  pinMode(PIN_SONAR_TRIG_YR, OUTPUT);
  pinMode(PIN_SONAR_ECHO_YR, INPUT);
  pinMode(PIN_BTN_LEFT_CLICK, INPUT);
  pinMode(PIN_BTN_RIGHT_CLICK, INPUT);

  digitalWrite(PIN_BTN_LEFT_CLICK, HIGH);
  digitalWrite(PIN_BTN_RIGHT_CLICK, HIGH);

  Serial.begin(68000);
  //Serial.setTimeout(10000);
}


float axisXU, axisXD, axisYL, axisYR, axisX, axisY, lastAxisX, lastAxisY;
float x, y;
bool printCoords = false;

void loop() {

  updateSensorValues();

  while (axisXU > RANGE_MAX && axisXD > RANGE_MAX && axisYL > RANGE_MAX && axisYR > RANGE_MAX) { updateSensorValues();  printCoordinates(); }

  if (abs(axisXU - axisXD) < 1 && axisXU < RANGE_MAX && axisXD < RANGE_MAX) lastAxisX = (axisXU + axisXD) / 2;
  else if (axisXU > RANGE_MAX && axisXD < RANGE_MAX) lastAxisX = axisXD;
  else if (axisXU < RANGE_MAX && axisXD > RANGE_MAX) lastAxisX = axisXU;
  else if (axisXU > RANGE_MAX && axisXD > RANGE_MAX) lastAxisX = lastAxisX;

  if (abs(axisYL - axisYR) < 1 && axisYL < RANGE_MAX && axisYR < RANGE_MAX) lastAxisY = (axisYL + axisYR) / 2;
  else if (axisYL > RANGE_MAX && axisYR < RANGE_MAX) lastAxisY = axisYR;
  else if (axisYL < RANGE_MAX && axisYR > RANGE_MAX) lastAxisY = axisYL;
  else if (axisYL > RANGE_MAX && axisYR > RANGE_MAX) lastAxisY = lastAxisY;
  

  while ( (axisXU < RANGE_MAX || axisXD < RANGE_MAX) && (axisYL < RANGE_MAX || axisYR < RANGE_MAX) )
  {
    lastAxisX = axisX;
    lastAxisY = axisY;
    
    updateSensorValues();
    if (abs(axisXU - axisXD) < 1 && axisXU < RANGE_MAX && axisXD < RANGE_MAX) axisX = (axisXU + axisXD) / 2;
    else if (axisXU > RANGE_MAX && axisXD < RANGE_MAX) axisX = axisXD;
    else if (axisXU < RANGE_MAX && axisXD > RANGE_MAX) axisX = axisXU;
    else if (axisXU > RANGE_MAX && axisXD > RANGE_MAX) axisX = lastAxisX;
  
    if (abs(axisYL - axisYR) < 1 && axisYL < RANGE_MAX && axisYR < RANGE_MAX) axisY = (axisYL + axisYR) / 2;
    else if (axisYL > RANGE_MAX && axisYR < RANGE_MAX) axisY = axisYR;
    else if (axisYL < RANGE_MAX && axisYR > RANGE_MAX) axisY = axisYL;
    else if (axisYL > RANGE_MAX && axisYR > RANGE_MAX) axisY = lastAxisY;

    x += SENSITIVITY * (axisX - (RANGE_MIN + RANGE / 2.0)) / (RANGE / 2.0);
    y += SENSITIVITY * (axisY - (RANGE_MIN + RANGE / 2.0)) / (RANGE / 2.0);


   x = constrain(x, 0, SCREEN_WIDTH);
   y = constrain(y, 0, SCREEN_HEIGHT);

    printCoords = true;
    printCoordinates();

    delay(0);
  }
  printCoords = false;


}


void updateSensorValues()
{
  //Upper X-Axis
  digitalWrite(PIN_SONAR_TRIG_XU, LOW);
  delayMicroseconds(2);
  
  digitalWrite(PIN_SONAR_TRIG_XU, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_SONAR_TRIG_XU, LOW);
  axisXU = (pulseIn(PIN_SONAR_ECHO_XU, HIGH) * 0.034 / 2);
  axisXU = max(axisXU, 0);

  
  //Left Y-Axis
  digitalWrite(PIN_SONAR_TRIG_YL, LOW);
  delayMicroseconds(2);
  
  digitalWrite(PIN_SONAR_TRIG_YL, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_SONAR_TRIG_YL, LOW);
  axisYL = (pulseIn(PIN_SONAR_ECHO_YL, HIGH) * 0.034 / 2);
  axisYL = max(axisYL, 0);


  //Lower X-Axis
  digitalWrite(PIN_SONAR_TRIG_XD, LOW);
  delayMicroseconds(2);
  
  digitalWrite(PIN_SONAR_TRIG_XD, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_SONAR_TRIG_XD, LOW);
  axisXD = (pulseIn(PIN_SONAR_ECHO_XD, HIGH) * 0.034 / 2);
  axisXD = max(axisXD, 0);


  //Right Y-Axis
  digitalWrite(PIN_SONAR_TRIG_YR, LOW);
  delayMicroseconds(2);
  
  digitalWrite(PIN_SONAR_TRIG_YR, HIGH);
  delayMicroseconds(10);

  digitalWrite(PIN_SONAR_TRIG_YR, LOW);
  axisYR = (pulseIn(PIN_SONAR_ECHO_YR, HIGH) * 0.034 / 2);
  axisYR = max(axisYR, 0);
}

void printCoordinates()
{
  String msg = "|";
  if (printCoords) {
    if (x < 10) msg += "0";
    msg += String(x) + ",";
    
    if (y < 10) msg += "0";
    msg += String(y);
  } else msg += "TTTTTTTTTTT";

  
  if (digitalRead(PIN_BTN_LEFT_CLICK) == LOW) msg += "L";
  else msg += "K";

  if (digitalRead(PIN_BTN_RIGHT_CLICK) == LOW) msg += "R";
  else msg += "E";
  
  msg += ":";
  Serial.println(msg);
  //Serial.flush();
  delay(10);
  //byte rx[3];
  //Serial.readBytesUntil('|', rx, 3);

  //Serial.println(axisXD);

  //Serial.println(axisXD);
}


