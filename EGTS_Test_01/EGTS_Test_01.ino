/*
    Name:       EGTS_Test_01.ino
    Created:	22.06.2018 19:12:38
    Author:     Александр
*/
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "de.h"     // ST_ANSWER
#include "egts.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ST_RECORD records[1];
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define OUT_BUFFER_SIZE 1024
char outBuffer[OUT_BUFFER_SIZE] = {0};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char HEX_CHARS[]  PROGMEM = {"0123456789ABCDEF"};
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define SD_CS_PIN 10 // номер пина CS для SD
#include <SdFat.h>
SdFat SD;
bool sdInitFlag = false;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char* ToHex(int i)
{  

  static char _HEX_HOLDER[3] = {0}; // глобальный холдер для шестнадцатеричного представления байта в строковом виде
  
 // String Out;
  int idx = i & 0xF;
  char char1 = (char) pgm_read_byte_near( HEX_CHARS + idx );
  i>>=4;
  idx = i & 0xF;
  char char2 = (char) pgm_read_byte_near( HEX_CHARS + idx );
  //Out = String(char2); Out += String(char1);
  
  _HEX_HOLDER[0] = char2;
  _HEX_HOLDER[1] = char1;
  
  return _HEX_HOLDER;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void writeOnSD(int encodedBytes, const char* filename)
{
  if(!sdInitFlag)
    return;

  SdFile f;

  f.open(filename,FILE_WRITE);  
  if(!f.isOpen())
  {
    Serial.print("Unable to open file \"");
    Serial.print(filename);
    Serial.println("\"!");
    return;
  }

  f.write(outBuffer,encodedBytes);
  f.flush();  
  f.close();

  Serial.print("Data saved to file \"");
  Serial.print(filename);
  Serial.println("\"!");
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void testOutput(int encodedBytes)
{
  Serial.print("Encoded bytes: ");
  Serial.println(encodedBytes);

  Serial.println();
  Serial.println("Encoded buffer RAW dump =============================>");
  Serial.println();

  for(int i=0;i<encodedBytes;i++)
  {
    Serial.write(outBuffer[i]);
  }

  Serial.println();
  Serial.println();
  Serial.println("<<==================================== RAW dump end");

  Serial.println();
  Serial.println("Encoded buffer HEX dump =============================>");
  Serial.println();

  for(int i=0;i<encodedBytes;i++)
  {
    Serial.print(ToHex(outBuffer[i]));
  }

  Serial.println();
  Serial.println();
  Serial.println("<<==================================== HEX dump end");
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{

  Serial.begin(57600);

  Serial.println("Init SD...");
  sdInitFlag = SD.begin(SD_CS_PIN,SPI_HALF_SPEED);
  if(sdInitFlag)
    Serial.println("SD inited successfully.");
  else
    Serial.println("SD init ERROR!");
  

  memset(&records,0,sizeof(records));
  memset(outBuffer,0,OUT_BUFFER_SIZE);


  
  // идентификатор терминала
//  strcpy(records[0].imei,"TEST TERM");
  
//  strcpy(records[0].tracker,"EGTS");

  // версии терминалов (железо и софт)
//  strcpy(records[0].hard,"0.1a");
//  strcpy(records[0].soft,"0.1a");


  // признак валидности записи
  records[0].valid = 1;

  // пишем дату от балды
  records[0].data = 1234;

  // и время от балды
  records[0].time = 650;

  // идём на север :)
  records[0].clon = 'N';

  // и на восток
  records[0].clat = 'E';

  // номер записи
  records[0].recnum = 12;

  // пробег, с дискретностью 0.1 км
  records[0].probeg = 88.9;

  // широта/долгота
  records[0].lon = 1;//12.24;
  records[0].lat = 1;//13.26;

  // скорость, км/ч
  records[0].speed = 49.36;

  // курс?
  records[0].curs = 99;

  // показания топлива
  records[0].fuel[0] = 4321;


  // тестируем пакет аутентификации
  Serial.println("----------------------------- AUTH TEST ------------------------------------------");
  int encodedBytes = terminal_encode(records,-1,outBuffer,OUT_BUFFER_SIZE);
  testOutput(encodedBytes);
  writeOnSD(encodedBytes, "test.bin");
  memset(outBuffer,0,OUT_BUFFER_SIZE);
  
  Serial.println("----------------------------- AUTH TEST END ------------------------------------------");
  Serial.println();


  Serial.println("----------------------------- MAIN TEST ------------------------------------------");
  encodedBytes = terminal_encode(records,1,outBuffer,OUT_BUFFER_SIZE);
  testOutput(encodedBytes);
  writeOnSD(encodedBytes, "test1.bin");
  Serial.println("----------------------------- MAIN TEST END ------------------------------------------");
  


}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop()
{


}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

