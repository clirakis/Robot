/**
 ******************************************************************
 *
 * Module Name : 251128_R1
 *
 * Author/Date : C.B. Lirakis / 28-Nov-25
 *
 * Description : Robot control module for Arduino. Communicate simple commands for 
 *               moving the robot in patterns. This should communicate through the 
 *               serial port and receive commands terminated by CR.  These commands are
 *               then parsed into operations performed by the Aruduino stepper motors.
 *
 *               the orginal code I made appears to be lost. 
 *
 * Restrictions/Limitations : NONE
 *
 * Change Descriptions :
 *
 * Classification : Unclassified
 *
 * References : 
 * https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 * https://docs.arduino.cc/libraries/stepper/
 * https://docs.arduino.cc/learn/electronics/servo-motors/
 * https://docs.arduino.cc/libraries/servo/
 *
 *******************************************************************
 */
#include <Servo.h>
/*
 * Current stepper motor set values
*/
static int Left, Right;
Servo One;
Servo Two;
/*
* these are the offsets that represent zero motion on the specific servo. 
*/
const int Left0  = 96;
const int Right0 = 94;
#define DEBUG_PRINT 0


/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  One.attach(9);
  Two.attach(10);
  Serial.println("Version 1.02");
}
/**
 ******************************************************************
 *
 * Function Name :
 *
 * Description :
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static void BlinkTest(void)
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println("HIGH");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
/**
 ******************************************************************
 *
 * Function Name : Parse
 *
 * Description : Parse Command and do operation 
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static void Help(void)
{
  Serial.println("? - help");
  Serial.println("B - reverse");
  Serial.println("C - center");
  Serial.println("D - decrease speed");
  Serial.println("F - forward");
  Serial.println("G - Get current values.");
  Serial.println("H - halt");
  Serial.println("I - increase speed");
  Serial.println("L  - turn left");
  Serial.println("R  - turn right");
  Serial.println("S value, set speed to value");
  Serial.println("V - two values vector");
}
/**
 ******************************************************************
 *
 * Function Name : Parse
 *
 * Description : Parse Command and do operation 
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
static void Parse(String command)
{
  int d;
  String sub;
  /*
   * when entering the value for the servo motion it will be referenced to their zero point. 
   * eg: 0 means sending the device a halt command. 
  */
  // Debug line
  #if DEBUG_PRINT
  Serial.println(command); 
  #endif

  switch (command[0])
  {
    case '?':
      Help();
      break;
    case 'B':
      Left=-1;
      Right=-1;
      break;
    case 'C':
      if (Left>Right)
        Right = Left;
      else
        Left = Right;
      break;
    case 'D':
      Left--;
      Right--;
      break;
    case 'F':
      Left++;
      Right++;
      break;
    case 'G':
      sub = String(Left) + String(",") + String(Right);
      Serial.println(sub);
      break;
    case 'H':
    case 'h':
      Left = Right = 0;
      break;
    case 'I':
      /* FIXME -- make this more sophisticated, depending on current direction*/
      Left++;
      Right++;
      break;
    case 'L':
      Left++;
      break;
    case 'R':
      Right++;
    case 'S':
      /* Remaing part of string is speed, delimit by : */
      d = command.indexOf(':');
      Left = command.substring(d+1).toInt();
      Right = Left;
      break;
    case 'V':
      /* 2 values passed after : delimited by L,R */
      d = command.indexOf(':');
      sub = command.substring(d+1);
      d = sub.indexOf(',');
      Left = sub.substring(0,d).toInt();
      Right = sub.substring(d+1).toInt();
      break;
    default:
      Serial.println("Command not recognized.");
  }
  One.write(Right + Right0);
  Two.write(Left0 - Left);    // not sure why this is. 
  #if DEBUG_PRINT
  sub = String("New set values are: ") + String(Left) + String(" , ") + String(Right);
  Serial.println(sub);
  #endif
}
/**
 ******************************************************************
 *
 * Function Name : Loop
 *
 * Description : Arudino Main Event loop. 
 *
 * Inputs :
 *
 * Returns :
 *
 * Error Conditions :
 * 
 * Unit Tested on: 
 *
 * Unit Tested by: CBL
 *
 *
 *******************************************************************
 */
void loop() {
  String RxData;
  // put your main code here, to run repeatedly:
  // Test code
  //BlinkTest();
  digitalWrite(LED_BUILTIN, HIGH);
  /*
   * See if there is any data on the serial line and parse it accordingly. 
   * put in LED blinks for debug purposes.
   */
  if(Serial.available()>0)
  {
    // Simple repeat back. 
    RxData = Serial.readStringUntil('\n');
    Parse(RxData);
  }
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  
}
