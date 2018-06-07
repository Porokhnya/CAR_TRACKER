#include "HexUtils.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
byte Hex::FromHex(const char* buff)
{
  byte tens = Hex::MakeNum(*buff++);
  byte ones = Hex::MakeNum(*buff);

   if(ones == 'X') 
    return  0;
    
  return  (tens * 16) + ones; 
}
//--------------------------------------------------------------------------------------------------------------------------------
const char HEX_CHARS[]  PROGMEM = {"0123456789ABCDEF"};
//--------------------------------------------------------------------------------------------------------------------------------
const char* Hex::ToHex(int i)
{  

  static char WORK_STATUS_HEX_HOLDER[3] = {0}; // глобальный холдер для шестнадцатеричного представления байта в строковом виде
  
 // String Out;
  int idx = i & 0xF;
  char char1 = (char) pgm_read_byte_near( (HEX_CHARS + idx) );
  i>>=4;
  idx = i & 0xF;
  char char2 = (char) pgm_read_byte_near( (HEX_CHARS + idx) );
  //Out = String(char2); Out += String(char1);
  
  WORK_STATUS_HEX_HOLDER[0] = char2;
  WORK_STATUS_HEX_HOLDER[1] = char1;
  
  //return Out; 
  return WORK_STATUS_HEX_HOLDER;
}
//--------------------------------------------------------------------------------------------------------------------------------
byte Hex::MakeNum(char ch) 
{
  if((ch >= '0') && (ch <= '9'))
    return ((byte) ch) - '0';
  
  switch(ch) 
  {
    case 'A':
    case 'a': return 10;
    
    case 'B':
    case 'b': return 11;
    
    case 'C':
    case 'c': return 12;
    
    case 'D':
    case 'd': return 13;

    case 'E':
    case 'e': return 14;
    
    case 'F':
    case 'f': return 15;
    
    default: return 16;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------

