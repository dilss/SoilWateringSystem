
#include <SoftwareSerial.h>



const byte HC12RxdPin = 4;                      // "RXD" Pin on HC12
const byte HC12TxdPin = 5;                      // "TXD" Pin on HC12
const byte HC12SetPin = 6;                      // "SET" Pin on HC12

unsigned long timer = millis();                 // Delay Timer

char serialByteIn;                              // Temporary variable
char HC12ByteIn;                                // Temporary variable
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String serialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
boolean serialEnd = false;                      // Flag to indicate End of Serial String
boolean HC12Start = false;
boolean HC12End = false;                        // Flag to indiacte End of HC12 String
boolean commandMode = false;                    // Send AT commands
String clientIDVect[] = {"1A","2A"};
String initialPackageMarker = "S";              // Inicial marker of package
String finalPackageMarker = "E";                // Final marker of a package
String auxiliarString = "";
String returnOk = "OK";
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
    }                                     // Set HC12End flag to true
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
void setup() {
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
   fillHC12Buffer();
   //readDataFromRemote();
   if (HC12End){
      if((HC12ReadBuffer == clientIDVect[0]) or (HC12ReadBuffer == clientIDVect[1]))
       {
          auxiliarString = initialPackageMarker+returnOk;
          auxiliarString += HC12ReadBuffer;
          
          HC12.println(auxiliarString+finalPackageMarker);
          delay(1000);
          Serial.print("Receiving from client: ");
          Serial.println(HC12ReadBuffer);
          HC12ReadBuffer = "";
        }
      HC12End = false;
      fillHC12Buffer();
      if(HC12End){
          Serial.println(HC12ReadBuffer);
          HC12End = false;
          HC12ReadBuffer = "";
      }
   }
   
   
}

