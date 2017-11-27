#include <avr/io.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <SoftwareSerial.h>



const float counterReference = 12.00; //(43200.00)             //(12 hours * 3600 seconds)
const byte HC12RxdPin = 4;                                     // "RXD" Pin on HC12
const byte HC12TxdPin = 5;                      // "TXD" Pin on HC12
const byte HC12SetPin = 6;                      // "SET" Pin on HC12

unsigned long timer = millis();                 // Delay Timer

int index = 0;
volatile float runningCounter = 0;              //Variable that receive an update of 4 seconds every time an interrupt occurs
volatile int clockFlag = 0;
char serialByteIn;                              // Temporary variable
char HC12ByteIn;                                // Temporary variable
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String serialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
boolean serialEnd = false;                      // Flag to indicate End of Serial String
boolean HC12Start = false;
boolean HC12End = false;                        // Flag to indiacte End of HC12 String
boolean commandMode = false;                    // Send AT commands
String clientID = "1A";
String initialPackageMarker = "S";              // Inicial marker of package
String finalPackageMarker = "E";                // Final marker of a package
int HC12IndexKeeper = 0;
int serialIndexKeeper = 0;


// Software Serial ports Rx and Tx are opposite the HC12 Rx and Tx
// Create Software Serial Port for HC12
SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);

/*=================================================================================================================================*/
String encapsulateData(String data = "")
{
  data = initialPackageMarker + data;
  data = data + finalPackageMarker;
  return data;
}
/*=================================================================================================================================*/
void fillHC12Buffer()                           //fills the HC12 buffer only with valid data
{
  while (HC12.available()) 
  {                                             // While Arduino's HC12 soft serial rx buffer has data
    HC12ByteIn = HC12.read();                   // Store each character from rx buffer in byteIn
    if (HC12Start == true)
    {
      if(HC12ByteIn != 'E')
      {
        HC12ReadBuffer += char(HC12ByteIn);     // Write each character of byteIn to HC12ReadBuffer
      }
      else 
      {
        HC12Start = false;
        HC12End = true;
      }
    }
    else if (HC12ByteIn == 'S')
    {
      HC12Start = true;
    }        
  }
}
/*=================================================================================================================================*/
void fillSerialBuffer()
{
   while (Serial.available())
   {                                            // If Arduino's computer rx buffer has data
    serialByteIn = Serial.read();               // Store each character in byteIn
    if(serialByteIn != '\n')
    {
      serialReadBuffer += char(serialByteIn);   // Write each character of byteIn to SerialReadBuffer
      serialIndexKeeper++;
    }
    if(serialByteIn =='\n')
    {
     serialEnd = true;                          // Set SerialEnd flag to indicate end of line
    }
  }
  serialIndexKeeper = 0;
}
/*=================================================================================================================================*/

void sendDataToRemote()
{
   if (serialEnd){                              // Check to see if SerialEnd flag is true
      serialReadBuffer = encapsulateData(serialReadBuffer);
      Serial.println(serialReadBuffer);
      HC12.println(serialReadBuffer);             // Transmit non-command message
    }
    serialReadBuffer = "";                        // Clear SerialReadBuffer
    serialEnd = false;                            // Reset serial end of line flag

}
/*=================================================================================================================================*/
boolean requestCommunicationWithServer()
{
  HC12.print("S" + clientID + "E");
  delay(1000);
  fillHC12Buffer();
  if(HC12ReadBuffer == "OK1A")
  {
    Serial.println(HC12ReadBuffer);
    HC12ReadBuffer = "";
    return true;
  }
  HC12ReadBuffer = "";
  return false; 
}
/*=================================================================================================================================*/
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  
  sleep_enable();


  /* Disable all of the unused peripherals. This will reduce power
   * consumption further and, more importantly, some of these
   * peripherals may generate interrupts that will wake our Arduino from
   * sleep!
   */
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  

  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}
/*=================================================================================================================================*/
ISR(TIMER1_OVF_vect)                            // Interrrup subroutine that increases the counter for the main execution program
{
  runningCounter += 4.09;
  clockFlag = 1;
}
/*=================================================================================================================================*/
void setup() {
    /* Normal timer operation.*/
  TCCR1A = 0x00; 
  
  /* Clear the timer counter register.
   * You can pre-load this register with a value in order to 
   * reduce the timeout period, say if you wanted to wake up
   * ever 4.0 seconds exactly.
   */
  TCNT1=0x0000; 
  
  /* Configure the prescaler for 1:1024, giving us a 
   * timeout of 4.09 seconds.
   */
  TCCR1B = 0x05;
  
  /* Enable the timer overlow interrupt. */
  TIMSK1=0x01;

  HC12ReadBuffer.reserve(64);                   // Reserve 24 bytes for Serial message input
  serialReadBuffer.reserve(64);                 // Reserve 24 bytes for HC12 message input

  pinMode(HC12SetPin, OUTPUT);                  // Output High for Transparent / Low for Command
  digitalWrite(HC12SetPin, HIGH);               // Enter Transparent mode
  delay(80);                                    // 80 ms delay before operation per datasheet
  Serial.begin(9600);                           // Open serial port to computer
  HC12.begin(9600);                             // Open software serial port to HC12
 }
/*=================================================================================================================================*/
void loop()
{  
  if (runningCounter >= counterReference)
  {
    Serial.println("Boraaaaaa! Ce quer ver essa porra bixo? BIRLLLLL!!!");
    if(requestCommunicationWithServer())
    {
      fillHC12Buffer();
      //sendDataToRemote();
      HC12.print(encapsulateData("AnalogRead:"));
      delay(1000);
      HC12ReadBuffer = "";
     }
     runningCounter = 0.0;
   }
   if(clockFlag) Serial.println(runningCounter);
   clockFlag = 0;
   enterSleep();
}

