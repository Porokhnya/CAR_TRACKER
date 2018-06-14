#include "CoreTransport.h"
#include "HexUtils.h"
//--------------------------------------------------------------------------------------------------------------------------------------
// CoreTransportClient
//--------------------------------------------------------------------------------------------------------------------------------------
CoreTransportClient::CoreTransportClient()
{
  socket = NO_CLIENT_ID;
  dataBuffer = NULL;
  dataBufferSize = 0;
  parent = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
CoreTransportClient::~CoreTransportClient()
{
  clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransportClient::accept(CoreTransport* _parent)
{
  parent = _parent;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransportClient::clear()
{
    delete [] dataBuffer; 
    dataBuffer = NULL;
    dataBufferSize = 0;
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransportClient::disconnect()
{
  if(!parent)
    return;
  
  if(!connected())
    return;

  parent->doDisconnect(*this);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransportClient::connect(const char* ip, uint16_t port)
{
  if(!parent)
    return;
  
  if(connected()) // уже присоединены, нельзя коннектится до отсоединения!!!
    return;
          
  parent->doConnect(*this,ip,port);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreTransportClient::write(uint8_t* buff, size_t sz)
{
  if(!parent)
    return false;
  
    if(!sz || !buff || !connected() || socket == NO_CLIENT_ID)
    {
      #ifdef _DEBUG
        DBGLN(F("CoreTransportClient - CAN'T WRITE!"));
      #endif
      return false;
    }

  clear();
  dataBufferSize = sz; 
  if(dataBufferSize)
  {
      dataBuffer = new  uint8_t[dataBufferSize];
      memcpy(dataBuffer,buff,dataBufferSize);
  }

    parent->doWrite(*this);
    
   return true;
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreTransportClient::connected() 
{
  if(!parent || socket == NO_CLIENT_ID)
    return false;
    
  return parent->connected(socket);
}
//--------------------------------------------------------------------------------------------------------------------------------------
// CoreTransport
//--------------------------------------------------------------------------------------------------------------------------------------
CoreTransport::CoreTransport(uint8_t clientsPoolSize)
{
  for(uint8_t i=0;i<clientsPoolSize;i++)
  {
    CoreTransportClient* client = new CoreTransportClient();
    client->accept(this);
    client->bind(i);
    
    pool.push_back(client);
    status.push_back(false);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
CoreTransport::~CoreTransport()
{
  for(size_t i=0;i<pool.size();i++)
  {
    delete pool[i];
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::initPool()
{

  #ifdef _DEBUG
    DBGLN(F("INIT CLIENTS POOL..."));
  #endif
  
  Vector<CoreTransportClient*> tmp = externalClients;
  for(size_t i=0;i<tmp.size();i++)
  {
    notifyClientConnected(*(tmp[i]),false,CT_ERROR_NONE);
    tmp[i]->release();
  }
  
  for(size_t i=0;i<status.size();i++)
  {
    status[i] = false;
    pool[i]->clear(); // очищаем внутренних клиентов
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreTransport::connected(uint8_t socket)
{
  return status[socket];
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::doWrite(CoreTransportClient& client)
{
  if(!client.connected())
  {
    client.clear();
    return;
  }

   beginWrite(client); 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::doConnect(CoreTransportClient& client, const char* ip, uint16_t port)
{
  if(client.connected())
    return;

   // запоминаем нашего клиента
   client.accept(this);

  // если внешний клиент - будем следить за его статусом соединения/подсоединения
   if(isExternalClient(client))
    externalClients.push_back(&client);

   beginConnect(client,ip,port); 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::doDisconnect(CoreTransportClient& client)
{
  if(!client.connected())
    return;

    beginDisconnect(client);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::subscribe(IClientEventsSubscriber* subscriber)
{
  for(size_t i=0;i<subscribers.size();i++)
  {
    if(subscribers[i] == subscriber)
      return;
  }

  subscribers.push_back(subscriber);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::unsubscribe(IClientEventsSubscriber* subscriber)
{
  for(size_t i=0;i<subscribers.size();i++)
  {
    if(subscribers[i] == subscriber)
    {
      for(size_t k=i+1;k<subscribers.size();k++)
      {
        subscribers[k-1] = subscribers[k];
      }
      subscribers.pop();
      break;
    }
  }  
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreTransport::isExternalClient(CoreTransportClient& client)
{
  // если клиент не в нашем пуле - это экземпляр внешнего клиента
  for(size_t i=0;i<pool.size();i++)
  {
    if(pool[i] == &client)
      return false;
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::notifyClientConnected(CoreTransportClient& client, bool connected, int16_t errorCode)
{
   // тут надо синхронизировать с пулом клиентов
   if(client.socket != NO_CLIENT_ID)
   {
      status[client.socket] = connected;
   }
  
    for(size_t i=0;i<subscribers.size();i++)
    {
      subscribers[i]->OnClientConnect(client,connected,errorCode);
    }

      // возможно, это внешний клиент, надо проверить - есть ли он в списке слежения
      if(!connected) // пришло что-то типа 1,CLOSED
      {         
        // клиент отсоединился, надо освободить его сокет
        for(size_t i=0;i<externalClients.size();i++)
        {
          if(externalClients[i]->socket == client.socket)
          {
            externalClients[i]->clear();
            
            #ifdef _DEBUG
              DBG(F("RELEASE SOCKET ON OUTGOING CLIENT #"));
              DBGLN(String(client.socket));
            #endif
            
            externalClients[i]->release(); // освобождаем внешнему клиенту сокет
            
            for(size_t k=i+1;k<externalClients.size();k++)
            {
              externalClients[k-1] = externalClients[k];
            }
            externalClients.pop();
            break;
          }
        } // for
      } // if(!connected)
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::notifyDataWritten(CoreTransportClient& client, int16_t errorCode)
{
    for(size_t i=0;i<subscribers.size();i++)
    {
      subscribers[i]->OnClientDataWritten(client,errorCode);
    } 
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreTransport::notifyDataAvailable(CoreTransportClient& client, uint8_t* data, size_t dataSize, bool isDone)
{
    for(size_t i=0;i<subscribers.size();i++)
    {
      subscribers[i]->OnClientDataAvailable(client,data,dataSize,isDone);
    }  
}
//--------------------------------------------------------------------------------------------------------------------------------------
CoreTransportClient* CoreTransport::getClient(uint8_t socket)
{
  if(socket != NO_CLIENT_ID)
    return pool[socket];

  for(size_t i=0;i<pool.size();i++)
  {
    if(!pool[i]->connected())
      return pool[i];
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------
#include "PDUClasses.h"
//--------------------------------------------------------------------------------------------------------------------------------------
extern "C" {
static void __noincomingcall(const String& phoneNumber, bool isKnownNumber, bool& shouldHangUp) {}
static void __nosmsreceived(const String& phoneNumber, const String& message, bool isKnownNumber) {}
static void __nocusdreceived(const String& cusd){}
}
//--------------------------------------------------------------------------------------------------------------------------------------
void ON_INCOMING_CALL(const String& phoneNumber, bool isKnownNumber, bool& shouldHangUp) __attribute__ ((weak, alias("__noincomingcall")));
void ON_SMS_RECEIVED(const String& phoneNumber,const String& message, bool isKnownNumber) __attribute__ ((weak, alias("__nosmsreceived")));
void ON_CUSD_RECEIVED(const String& cusd) __attribute__ ((weak, alias("__nocusdreceived")));
//--------------------------------------------------------------------------------------------------------------------------------------
CoreSIM800Transport SIM800;
//--------------------------------------------------------------------------------------------------------------------------------------
CoreSIM800Transport::CoreSIM800Transport() : CoreTransport(SIM800_MAX_CLIENTS)
{

  recursionGuard = 0;
  flags.waitCipstartConnect = false;
  cipstartConnectClient = NULL;
  workStream = NULL;  

}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::readFromStream()
{
  if(!workStream)
    return;
    
  while(workStream->available())
  {
    receiveBuffer.push_back((uint8_t) workStream->read()); 
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::sendCommand(const String& command, bool addNewLine)
{

  size_t len = command.length();
  for(size_t i=0;i<len;i++)
  {
    // записали байтик
    workStream->write(command[i]);

    // прочитали, что пришло от ESP
    readFromStream();

   #ifdef USE_WIFI_MODULE
     // и модуль ESP тоже тут обновим
     ESP.readFromStream();
   #endif     
  }
    
  if(addNewLine)
  {
    workStream->println();
  }
  
  // прочитали, что пришло от SIM800
  readFromStream();

   #ifdef USE_WIFI_MODULE
   // и модуль ESP тоже тут обновим
   ESP.readFromStream();
   #endif   

  #ifdef GSM_DEBUG_MODE
    DBG(F("SIM800: ==> "));
    DBGLN(command);
  #endif

  machineState = sim800WaitAnswer; // говорим, что надо ждать ответа от SIM800
  // запоминаем время отсылки последней команды
  timer = millis();
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::GetAPNUserPass(String& user, String& pass)
{
    user = APN_USER;
    pass = APN_PASS;
}
//--------------------------------------------------------------------------------------------------------------------------------
String CoreSIM800Transport::GetAPN()
{
    return APN_ADDRESS;
}
//--------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::sendCommand(SIM800Commands command)
{
  currentCommand = command;
  
  // тут посылаем команду в SIM800
  switch(command)
  {
    case smaNone:
    case smaCIPSTART:
    case smaCIPSEND:
    case smaWaitSendDone:
    case smaCIPCLOSE:
    case smaCMGS:
    case smaWaitForSMSClearance:
    case smaWaitSMSSendDone:
    case smaCUSD:
    break;

    case smaCheckReady:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Check for modem READY..."));
      #endif
      sendCommand(F("AT+CPAS"));
    }
    break;

    case smaCIPHEAD:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set IPD setting..."));
      #endif
      sendCommand(F("AT+CIPHEAD=1"));      
    }
    break;

    case smaCIPSHUT:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Deactivate GPRS connection..."));
      #endif
      sendCommand(F("AT+CIPSHUT"));      
    }
    break;

    case smaCIICR:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Activate GPRS connection..."));
      #endif
      sendCommand(F("AT+CIICR"));            
    }
    break;

    case smaCIFSR:
    {
      sendCommand(F("AT+CIFSR"));      
    }
    break;

    case smaCSTT:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Setup GPRS connection..."));
      #endif

      String apnUser, apnPass;
      GetAPNUserPass(apnUser, apnPass);
      
      String comm = F("AT+CSTT=\"");
      comm += GetAPN();
      comm += F("\",\"");
      comm += apnUser;
      comm += F("\",\"");
      comm += apnPass;
      comm += F("\"");

      sendCommand(comm);
    }
    break;

    case smaCIPMODE:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set CIPMODE..."));
      #endif
      sendCommand(F("AT+CIPMODE=0"));            
    }
    break;

    case smaCIPMUX:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set CIPMUX..."));
      #endif
      sendCommand(F("AT+CIPMUX=1"));            
    }
    break;

    case smaEchoOff:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: echo OFF..."));
      #endif
      sendCommand(F("ATE0"));
    }
    break;

    case smaDisableCellBroadcastMessages:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: disable cell broadcast messagess..."));
      #endif
      sendCommand(F("AT+CSCB=1"));
    }
    break;

    case smaAON:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Turn AON ON..."));
      #endif    
        sendCommand(F("AT+CLIP=1"));  
    }
    break;

    case smaPDUEncoding:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set PDU format..."));
      #endif
      sendCommand(F("AT+CMGF=0"));
    }
    break;

    case smaUCS2Encoding:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set UCS2 format..."));
      #endif
      sendCommand(F("AT+CSCS=\"UCS2\""));
    }
    break;

    case smaSMSSettings:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set SMS output mode..."));
      #endif
      sendCommand(F("AT+CNMI=2,2"));
    }
    break;

    case smaGPRSMultislotClass:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Set GPRS multislot class..."));
      #endif
      sendCommand(F("AT+CGMSCLASS=4"));
    }
    break;    

    case smaWaitReg:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Check registration status..."));
      #endif
      sendCommand(F("AT+CREG?"));
    }
    break;

    case smaCheckModemHang:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Check if modem available..."));
      #endif
      sendCommand(F("AT"));
    }
    break;

    #ifdef GSM_PULL_GPRS_BY_PING
    case smaPING:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Start PING..."));
      #endif
      String cmd = F("AT+CIPPING=\"");
      cmd += GSM_PING_HOST;
      cmd += F("\",2,32,2,64");
      sendCommand(cmd);
    }
    break;
    #endif // GSM_PULL_GPRS_BY_PING
    

    case smaHangUp:
    {
      #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: Hang up...")); 
      #endif     
      sendCommand(F("ATH"));      
    }
    break;
    
  } // switch

}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreSIM800Transport::isKnownAnswer(const String& line, SIM800KnownAnswer& result)
{
  result = gsmNone;
  
  if(line == F("OK"))
  {
    result = gsmOK;
    return true;
  }
  if(line == F("ERROR"))
  {
    result = gsmError;
    return true;
  }
  if(line == F("FAIL"))
  {
    result = gsmFail;
    return true;
  }
  if(line.endsWith(F("SEND OK")))
  {
    result = gsmSendOk;
    return true;
  }
  if(line.endsWith(F("SEND FAIL")))
  {
    result = gsmSendFail;
    return true;
  }
  if(line.endsWith(F("CONNECT OK")))
  {
    result = gsmConnectOk;
    return true;
  }
  if(line.endsWith(F("CONNECT FAIL")))
  {
    result = gsmConnectFail;
    return true;
  }
  if(line.endsWith(F("ALREADY CONNECT")))
  {
    result = gsmAlreadyConnect;
    return true;
  }
  if(line.endsWith(F("CLOSE OK")))
  {
    result = gsmCloseOk;
    return true;
  }
   if(line.endsWith(F("SHUT OK")))
  {
    result = gsmShutOk;
    return true;
  }  
  
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::processIncomingCall(const String& line)
{
 // приходит строка вида
  // +CLIP: "79182900063",145,,,"",0
  
   // входящий звонок, проверяем, приняли ли мы конец строки?
    String ring = line.substring(8); // пропускаем команду +CLIP:, пробел и открывающую кавычку "

    int16_t idx = ring.indexOf("\"");
    if(idx != -1)
      ring = ring.substring(0,idx);

    if(ring.length() && ring[0] != '+')
      ring = String(F("+")) + ring;

  // ищем - есть ли у нас этот номер среди известных
  bool isKnownNumber = false;

  String knownNumber = GSM_KNOWN_NUMBER;

  if(knownNumber.startsWith(ring))
  {
    isKnownNumber = true;
  }

  bool shouldHangUp = true;
  
  // вызываем событие
  ON_INCOMING_CALL(ring,isKnownNumber,shouldHangUp);
  
 // добавляем команду "положить трубку" - она выполнится первой, поскольку очередь инициализации у нас имеет приоритет
  if(shouldHangUp)
    initCommandsQueue.push_back(smaHangUp);  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::processCMT(const String& pdu)
{
  if(pdu.length())
  {
    
    PDUIncomingMessage sms = PDU.Decode(pdu);

    if(sms.IsDecodingSucceed)
    {
      // СМС пришло, вызываем событие
      String knownNumber = GSM_KNOWN_NUMBER;
      
      bool anyKnownNumbersFound = false;
      if(knownNumber.startsWith(sms.SenderNumber))
      {
        anyKnownNumbersFound = true;
      }
      ON_SMS_RECEIVED(sms.SenderNumber,sms.Message, anyKnownNumbersFound);
    }

    
  } // if(pdu.length())
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::sendQueuedCUSD()
{
  if(!cusdList.size())
    return;

  String* cusdToSend = cusdList[0];

  for(size_t i=1;i<cusdList.size();i++)
  {
    cusdList[i-1] = cusdList[i];
  }

  cusdList.pop();

  sendCommand(*cusdToSend);
  delete cusdToSend;
  currentCommand = smaCUSD;  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::sendQueuedSMS()
{
  if(!outgoingSMSList.size())
    return;

  delete smsToSend;
  smsToSend = new String();
  
  int16_t messageLength = 0;

  SIM800OutgoingSMS* sms = &(outgoingSMSList[0]);  
 
  PDUOutgoingMessage encodedMessage = PDU.Encode(*(sms->phone),*(sms->message),sms->isFlash, smsToSend);
  messageLength = encodedMessage.MessageLength;
    
  delete sms->phone;
  delete sms->message;

  if(outgoingSMSList.size() < 2)
  {
    outgoingSMSList.clear();
  }
  else
  {
    for(size_t i=1;i<outgoingSMSList.size();i++)
    {
      outgoingSMSList[i-1] = outgoingSMSList[i];
    }
    outgoingSMSList.pop();
  }
    
  // тут отсылаем СМС
  String command = F("AT+CMGS=");
  command += String(messageLength);

  flags.waitForDataWelcome = true;
  sendCommand(command);
  currentCommand = smaCMGS;
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::processKnownStatusFromSIM800(const String& line)
{
  if(flags.pduInNextLine) // в прошлой строке пришло +CMT, поэтому в текущей - содержится PDU
  {
      flags.pduInNextLine = false;

      // разбираем, чего там пришло
      processCMT(line);

      timer = millis();
      return; 
  }

  // смотрим, подсоединился ли клиент?
   int16_t idx = line.indexOf(F(", CONNECT OK"));
   if(idx != -1)
   {
      // клиент подсоединился
      String s = line.substring(0,idx);
      int16_t clientID = s.toInt();
      if(clientID >=0 && clientID < SIM800_MAX_CLIENTS)
      {          
        #ifdef GSM_DEBUG_MODE
          DBG(F("SIM800: client connected - #"));
          DBGLN(String(clientID));
        #endif

        // тут смотрим - посылали ли мы запрос на коннект?
        if(flags.waitCipstartConnect && cipstartConnectClient != NULL && clientID == cipstartConnectClientID)
        {                
          // есть клиент, для которого надо установить ID
          cipstartConnectClient->bind(clientID);
          flags.waitCipstartConnect = false;
          cipstartConnectClient = NULL;
          cipstartConnectClientID = NO_CLIENT_ID;              
        } // if                 

        // выставляем клиенту флаг, что он подсоединён
        CoreTransportClient* client = getClient(clientID);              
        notifyClientConnected(*client,true,CT_ERROR_NONE);
      }
   } // if

   idx = line.indexOf(F(", CLOSE OK"));
   if(idx == -1)
    idx = line.indexOf(F(", CLOSED"));
   if(idx == -1)
    idx = line.indexOf(F("CONNECT FAIL"));
    
   if(idx != -1)
   {
    // клиент отсоединился
      String s = line.substring(0,idx);
      int16_t clientID = s.toInt();
      if(clientID >=0 && clientID < SIM800_MAX_CLIENTS)
      {
        
        if(line.indexOf(F("CONNECT FAIL")) != -1) // CONNECT FAIL приходит без ID клиента!!!
        {
          clientID = cipstartConnectClientID;
        }
        
        #ifdef GSM_DEBUG_MODE
          DBG(F("SIM800: client disconnected - #"));
          DBGLN(String(clientID));
        #endif

          // выставляем клиенту флаг, что он отсоединён
          CoreTransportClient* client = getClient(clientID);
          notifyClientConnected(*client,false,CT_ERROR_NONE);

        if(flags.waitCipstartConnect && cipstartConnectClient != NULL && clientID == cipstartConnectClientID)
        {            
          // есть клиент, для которого надо установить ID
          cipstartConnectClient->bind(clientID);
          notifyClientConnected(*cipstartConnectClient,false,CT_ERROR_NONE);
          cipstartConnectClient->bind(NO_CLIENT_ID);
          flags.waitCipstartConnect = false;
          cipstartConnectClient = NULL;
          cipstartConnectClientID = NO_CLIENT_ID;
                  
        } // if                           
      }        
    
   } // if(idx != -1)

  if(line.startsWith(F("+CLIP:")))
  {
    #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: +CLIP detected, parse!"));
    #endif
   
    processIncomingCall(line);
    
    timer = millis();        
    return; // поскольку мы сами отработали входящий звонок - выходим
  }
  else
  if(line.startsWith(F("+CMT:")))
  {
      flags.pduInNextLine = true;
      return;
  }
  else
  if(line.startsWith(F("+CUSD:")))
  {
    // пришёл ответ на запрос CUSD
    #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: +CUSD detected, parse!"));
    #endif
      
    // дождались ответа, парсим
    int quotePos = line.indexOf('"');
    int lastQuotePos = line.lastIndexOf('"');
    
    if(quotePos != -1 && lastQuotePos != -1 && quotePos != lastQuotePos) 
    {
   
      String cusdSMS = line.substring(quotePos+1,lastQuotePos);
      
      #ifdef GSM_DEBUG_MODE
        DBG(F("ENCODED CUSD IS: ")); 
        DBGLN(cusdSMS);
      #endif

      // тут декодируем CUSD
      String decodedCUSD = PDU.getUTF8From16BitEncoding(cusdSMS);

      #ifdef GSM_DEBUG_MODE
        DBG(F("DECODED CUSD IS: ")); 
        DBGLN(decodedCUSD);
      #endif

      ON_CUSD_RECEIVED(decodedCUSD);
    
    }
    return;   
  } // if(line.startsWith(F("+CUSD:")))
  else
  if(line.startsWith(F("+PDP: DEACT")))
  {
      #ifdef GSM_DEBUG_MODE
        DBGLN(F("SIM800: GPRS connection broken, restart!")); 
      #endif

      rebootModem();
      
  } // if(line.startsWith(F("+PDP:DEACT")))
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreSIM800Transport::checkIPD(const TransportReceiveBuffer& buff)
{
  if(buff.size() < 13) // минимальная длина для RECEIVE, на примере +RECEIVE,1,1:
    return false;

  if(buff[0] == '+' && buff[1] == 'R' && buff[2] == 'E' && buff[3] == 'C'  && buff[4] == 'E'
   && buff[5] == 'I'  && buff[6] == 'V'  && buff[7] == 'E')
  {
    size_t to = min(buff.size(),30); // заглядываем вперёд на 30 символов, не больше
    for(size_t i=8;i<to;i++)
    {
      if(buff[i] == ':') // буфер начинается на +RECEIVE и содержит дальше ':', т.е. за ним уже идут данные
        return true;
    }
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
#ifdef GSM_DEBUG_MODE
void CoreSIM800Transport::dumpReceiveBuffer()
{
  Serial.print(F("SIM800 RECEIVE BUFFER, SIZE="));
  Serial.println(receiveBuffer.size());
  
  for(size_t i=0;i<receiveBuffer.size();i++)
  {
    Serial.print(Hex::ToHex(receiveBuffer[i]));
    Serial.print(" ");
  }

 Serial.println();
  
}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::rebootModem()
{
    #ifdef USE_GSM_REBOOT_PIN
      // есть пин, который надо использовать при зависании
      digitalWrite(GSM_REBOOT_PIN,GSM_POWER_OFF);
    #endif

    machineState = sim800Reboot;
    timer = millis();
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::update()
{

  if(!workStream) // нет рабочего потока
    return;

  if(flags.onIdleTimer) // попросили подождать определённое время, в течение которого просто ничего не надо делать
  {
      if(millis() - idleTimer > idleTime)
      {
        flags.onIdleTimer = false;
      }
  }

  // читаем из потока всё, что там есть
  readFromStream();

   #ifdef USE_WIFI_MODULE
   // и модуль ESP тоже тут обновим
   ESP.readFromStream();
   #endif   

  RecursionCounter recGuard(&recursionGuard);

  if(recursionGuard > 1) // рекурсивный вызов - просто вычитываем из потока - и всё.
  {
    #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: RECURSION!"));
    #endif    
    return;
  }
  
  bool hasAnswer = receiveBuffer.size();

  if(!hasAnswer)
  {
      // нет ответа от SIM800, проверяем, завис ли он?

      if(millis() - timer > GSM_MAX_ANSWER_TIME)
      {
        #ifdef GSM_DEBUG_MODE
          DBGLN(F("SIM800: modem not answering, reboot!"));
        #endif

        rebootModem();        
      } // if   
  }

  // выставляем флаг, что мы хотя бы раз получили хоть чего-то от ESP
  flags.isAnyAnswerReceived = flags.isAnyAnswerReceived || hasAnswer;

  bool hasAnswerLine = false; // флаг, что мы получили строку ответа от модема    

  String thisCommandLine;

  // тут проверяем, есть ли чего интересующего в буфере?
  if(checkIPD(receiveBuffer))
  {
      
    // в буфере лежит +RECEIVE,ID,DATA_LEN:
      int16_t idx = receiveBuffer.indexOf(','); // ищем первую запятую после +RECEIVE
      uint8_t* ptr = receiveBuffer.pData();
      ptr += idx+1;
      // перешли за запятую, парсим ID клиента
      String connectedClientID;
      while(*ptr != ',')
      {
        connectedClientID += (char) *ptr;
        ptr++;
      }
      ptr++; // за запятую
      String dataLen;
      while(*ptr != ':')
      {
        dataLen += (char) *ptr;
        ptr++;
      }
  
      // получили ID клиента и длину его данных, которые - пока в потоке, и надо их быстро попакетно вычитать
      int ipdClientID = connectedClientID.toInt();
      size_t ipdClientDataLength = dataLen.toInt();

      #ifdef GSM_DEBUG_MODE
        DBG(F("+RECEIVE DETECTED, CLIENT #"));
        DBG(String(ipdClientID));
        DBG(F(", LENGTH="));
        DBGLN(String(ipdClientDataLength));
      #endif

      // удаляем +RECEIVE,ID,DATA_LEN:
      receiveBuffer.remove(0,receiveBuffer.indexOf(':')+1);

      // у нас есть длина данных к вычитке, плюс сколько-то их лежит в буфере уже.
      // читать всё - мы не можем, т.к. данные могут быть гигантскими.
      // следовательно, надо читать по пакетам.
      CoreTransportClient* cl = getClient(ipdClientID);

      if(receiveBuffer.size() >= ipdClientDataLength)
      {
        // на время события мы должны обеспечить неизменность буфера, т.к.
        // в обработчике события может быть вызван yield, у указатель на память станет невалидным!
        
        uint8_t* thisBuffer = new uint8_t[ipdClientDataLength];
        memcpy(thisBuffer,receiveBuffer.pData(),ipdClientDataLength);

        receiveBuffer.remove(0,ipdClientDataLength);

        if(!receiveBuffer.size())
          receiveBuffer.clear();
          
        // весь пакет - уже в буфере
        notifyDataAvailable(*cl, thisBuffer, ipdClientDataLength, true);
        delete [] thisBuffer;
                
      }
      else
      {
        // не хватает части пакета в буфере.
        
        // теперь смотрим, сколько у нас данных ещё не послано клиентам
        size_t remainingDataLength = ipdClientDataLength;

        // нам осталось послать remainingDataLength данных клиентам,
        // побив их на пакеты длиной максимум TRANSPORT_MAX_PACKET_LENGTH

        // вычисляем длину одного пакета
        size_t packetLength = min(TRANSPORT_MAX_PACKET_LENGTH,remainingDataLength);

        while(remainingDataLength > 0)
        {
            // читаем, пока не хватает данных для одного пакета
            while(receiveBuffer.size() < packetLength)
            {
                #ifdef USE_WIFI_MODULE
                  ESP.readFromStream();
                #endif
              
                // данных не хватает, дочитываем
                if(!workStream->available())
                  continue;
    
                receiveBuffer.push_back((uint8_t) workStream->read());
                
            } // while

            // вычитали один пакет, уведомляем клиентов, при этом может пополниться буфер,
            // поэтому сохраняем пакет так, чтобы указатель на него был всегда валидным.
            uint8_t* thisBuffer = new uint8_t[packetLength];
            memcpy(thisBuffer,receiveBuffer.pData(),packetLength);

            receiveBuffer.remove(0,packetLength);
            if(!receiveBuffer.size())
              receiveBuffer.clear();

            notifyDataAvailable(*cl, thisBuffer, packetLength, (remainingDataLength - packetLength) == 0);
            delete [] thisBuffer;
            
            remainingDataLength -= packetLength;
            packetLength = min(TRANSPORT_MAX_PACKET_LENGTH,remainingDataLength);
        } // while

          
      } // else
    
  } // if(checkIPD(receiveBuffer))
  else if(flags.waitForDataWelcome && receiveBuffer[0] == '>')
  {
    flags.waitForDataWelcome = false;
    thisCommandLine = '>';
    hasAnswerLine = true;

    receiveBuffer.remove(0,1);
  }
  else // любые другие ответы от SIM800
  {
    // ищем до первого перевода строки
    size_t cntr = 0;
    for(;cntr<receiveBuffer.size();cntr++)
    {
      if(receiveBuffer[cntr] == '\n')
      {          
        hasAnswerLine = true;
        cntr++;
        break;
      }
    } // for

    if(hasAnswerLine) // нашли перевод строки в потоке
    {
      for(size_t i=0;i<cntr;i++)
      {
        if(receiveBuffer[i] != '\r' && receiveBuffer[i] != '\n')
          thisCommandLine += (char) receiveBuffer[i];
      } // for

      receiveBuffer.remove(0,cntr);
      
    } // if(hasAnswerLine)
  } // else

  // если в приёмном буфере ничего нету - просто почистим память
  if(!receiveBuffer.size())
    receiveBuffer.clear();

  if(hasAnswerLine && thisCommandLine.startsWith(F("AT+")))
   {
    #ifdef GSM_DEBUG_MODE
      DBG(F("Ignored echo: "));
      DBGLN(thisCommandLine);    
    #endif    
    // это эхо, игнорируем
      thisCommandLine = "";
      hasAnswerLine = false;
   }

   if(hasAnswerLine && flags.ignoreNextEmptyLine)
   {
      flags.ignoreNextEmptyLine = false;
      if(thisCommandLine == " ")
      {
        hasAnswerLine = false;
        thisCommandLine = "";
      }   
   }


  if(hasAnswerLine && !thisCommandLine.length()) // пустая строка, не надо обрабатывать
    hasAnswerLine = false;

   #ifdef GSM_DEBUG_MODE
    if(hasAnswerLine)
    {
      DBG(F("<== SIM800: "));
      DBGLN(thisCommandLine);
    }
   #endif

    // тут анализируем ответ от ESP, если он есть, на предмет того - соединён ли клиент, отсоединён ли клиент и т.п.
    // это нужно делать именно здесь, поскольку в этот момент в SIM800 может придти внешний коннект.
    if(hasAnswerLine)
    {
      processKnownStatusFromSIM800(thisCommandLine);
    } 

    // при разборе ответа тут будет лежать тип ответа, чтобы часто не сравнивать со строкой
    SIM800KnownAnswer knownAnswer = gsmNone;

    if(!flags.onIdleTimer)
    {
    // анализируем состояние конечного автомата, чтобы понять, что делать
    switch(machineState)
    {
        case sim800Idle: // ничего не делаем, можем работать с очередью команд и клиентами
        {            
            // смотрим - если есть хоть одна команда в очереди инициализации - значит, мы в процессе инициализации, иначе - можно работать с очередью клиентов
            if(initCommandsQueue.size())
            {
                #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: process next init command..."));
                #endif
                currentCommand = initCommandsQueue[initCommandsQueue.size()-1];
                initCommandsQueue.pop();
                sendCommand(currentCommand);
            } // if
            else
            {
              // в очереди команд инициализации ничего нет, значит, можем выставить флаг, что мы готовы к работе с клиентами
              flags.ready = true;

              if(outgoingSMSList.size())
              {
                sendQueuedSMS();
              }
              else
              if(cusdList.size())
              {
                sendQueuedCUSD();
              }
              else
              if(clientsQueue.size())
              {
                  // получаем первого клиента в очереди
                  TransportClientQueueData dt = clientsQueue[0];
                  int16_t clientID = dt.client->socket;
                  
                  // смотрим, чего он хочет от нас
                  switch(dt.action)
                  {
                    case actionDisconnect:
                    {
                      // хочет отсоединиться

                      currentCommand = smaCIPCLOSE;
                      String cmd = F("AT+CIPCLOSE=");
                      cmd += clientID;
                      sendCommand(cmd);                      
                      
                    }
                    break; // actionDisconnect

                    case actionConnect:
                    {
                        // мы разрешаем коннектиться только тогда, когда предыдущий коннект клиента уже обработан
                        if(!cipstartConnectClient)
                        {
                          // хочет подсоединиться
                          if(flags.gprsAvailable)
                          {
                            // здесь надо искать первый свободный слот для клиента
                            CoreTransportClient* freeSlot = getClient(NO_CLIENT_ID);
                            clientID = freeSlot ? freeSlot->socket : NO_CLIENT_ID;
      
                            flags.waitCipstartConnect = true;
                            cipstartConnectClient = dt.client;
                            cipstartConnectClientID = clientID;
                            
                            currentCommand = smaCIPSTART;
                            String comm = F("AT+CIPSTART=");
                            comm += clientID;
                            comm += F(",\"TCP\",\"");
                            comm += dt.ip;
                            comm += F("\",");
                            comm += dt.port;
      
                            delete [] clientsQueue[0].ip;
                            clientsQueue[0].ip = NULL;
                                            
                            // и отсылаем её
                            sendCommand(comm);
                            
                          } // gprsAvailable
                          else
                          {
                            // нет GPRS, не можем устанавливать внешние соединения!!!
                            removeClientFromQueue(dt.client);
                            dt.client->bind(clientID);
                            notifyClientConnected(*(dt.client),false,CT_ERROR_CANT_CONNECT);
                            dt.client->release();                        
                          }
                          
                       } // if(!cipstartConnectClient)
                    }
                    break; // actionConnect

                    case actionWrite:
                    {
                      // хочет отослать данные

                      currentCommand = smaCIPSEND;

                      size_t dataSize;
                      uint8_t* buffer = dt.client->getBuffer(dataSize);
                      clientsQueue[0].data = buffer;
                      clientsQueue[0].dataLength = dataSize;
                      dt.client->releaseBuffer();
                      
                      String command = F("AT+CIPSEND=");
                      command += clientID;
                      command += F(",");
                      command += dataSize;
                      flags.waitForDataWelcome = true; // выставляем флаг, что мы ждём >
                      
                      sendCommand(command);                
                    }
                    break; // actionWrite
                  } // switch
              }
              else
              {
                timer = millis(); // обновляем таймер в режиме ожидания, поскольку мы не ждём ответа на команды

                static uint32_t hangTimer = 0;
                if(millis() - hangTimer > GSM_AVAILABLE_CHECK_TIME)
                {
                  #ifdef GSM_DEBUG_MODE
                    DBGLN(F("SIM800: want to check modem availability..."));
                  #endif
                  hangTimer = millis();
                  sendCommand(smaCheckModemHang);
                  
                } // if
                else
                {
                  #ifdef GSM_PULL_GPRS_BY_PING
                    static uint32_t pingTimer = 0;
                    if(millis() - pingTimer > GSM_PING_INTERVAL)
                    {
                        #ifdef GSM_DEBUG_MODE
                          DBGLN(F("SIM800: PING GPRS!"));
                        #endif
                        pingTimer = millis();
                        sendCommand(smaPING);                      
                    }
                  #endif // GSM_PULL_GPRS_BY_PING
                } // else
                
              } // else
            } // else inited
        }
        break; // sim800Idle

        case sim800WaitAnswer: // ждём ответа от модема на посланную ранее команду (функция sendCommand переводит конечный автомат в эту ветку)
        {
          // команда, которую послали - лежит в currentCommand, время, когда её послали - лежит в timer.
              if(hasAnswerLine)
              {                
                // есть строка ответа от модема, можем её анализировать, в зависимости от посланной команды (лежит в currentCommand)
                switch(currentCommand)
                {
                  case smaNone:
                  {
                    // ничего не делаем
                  }
                  break; // cmdNone

                  case smaCUSD:
                  {
                    // отсылали запрос CUSD
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                        DBGLN(F("SIM800: CUSD WAS SENT."));
                      #endif
                      machineState = sim800Idle;
                    }                     
                  }
                  break; // smaCUSD

                  case smaWaitSMSSendDone:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                        DBGLN(F("SIM800: SMS was sent."));
                      #endif
                      sendCommand(F("AT+CMGD=1,4"));
                      currentCommand = smaWaitForSMSClearance;
                    }                       
                  }
                  break; // smaWaitSMSSendDone

                  case smaWaitForSMSClearance:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                        DBGLN(F("SIM800: SMS cleared."));
                      #endif
                      machineState = sim800Idle;
                    }                       
                    
                  }
                  break; // smaWaitForSMSClearance

                  case smaCMGS:
                  {
                    // отсылаем SMS
                    
                          if(thisCommandLine == F(">")) 
                          {
                            
                            // дождались приглашения, можно посылать    
                            #ifdef GSM_DEBUG_MODE
                                DBGLN(F("SIM800: Welcome received, continue sending..."));
                            #endif

                            sendCommand(*smsToSend,false);
                            workStream->write(0x1A); // посылаем символ окончания посыла

                            delete smsToSend;
                            smsToSend = new String();

                            currentCommand = smaWaitSMSSendDone;
                          } 
                          else 
                          {
                  
                            #ifdef GSM_DEBUG_MODE
                              DBG(F("SIM800: BAD ANWER TO SMS COMMAND: WANT '>', RECEIVED: "));
                              DBGLN(thisCommandLine);
                           #endif

                            delete smsToSend;
                            smsToSend = new String();
                            
                            // пришло не то, что ждали - просто игнорируем отсыл СМС
                             machineState = sim800Idle;
                              
                          }
                  }
                  break; // smaCMGS

                  case smaCIPCLOSE:
                  {
                    // отсоединялись. Ответа не ждём, т.к. может вклиниться всё, что угодно, пока мы ждём ответа

                    
                      if(clientsQueue.size())
                      {
                        #ifdef GSM_DEBUG_MODE
                            DBGLN(F("SIM800: Client disconnected."));
                        #endif

                        TransportClientQueueData dt = clientsQueue[0];                        
                        CoreTransportClient* thisClient = dt.client;
                        removeClientFromQueue(thisClient);
                                               
                      } // if(clientsQueue.size()) 
                      
                        machineState = sim800Idle; // переходим к следующей команде

                  }
                  break; // smaCIPCLOSE

                  case smaCIPSTART:
                  {
                    // соединялись
                    
                        if(isKnownAnswer(thisCommandLine,knownAnswer))
                        {                                                                                                        
                          if(knownAnswer == gsmConnectOk)
                          {
                            // законнектились удачно
                            if(clientsQueue.size())
                            {
                               #ifdef GSM_DEBUG_MODE
                                DBGLN(F("SIM800: Client connected."));
                               #endif
                               
                               TransportClientQueueData dt = clientsQueue[0];
                               removeClientFromQueue(dt.client);       
                            }
                            machineState = sim800Idle; // переходим к следующей команде
                          } // gsmConnectOk
                          else if(knownAnswer == gsmConnectFail)
                          {
                            // ошибка соединения
                            if(clientsQueue.size())
                            {
                               #ifdef GSM_DEBUG_MODE
                                DBGLN(F("SIM800: Client connect ERROR!"));
                               #endif

                              flags.waitCipstartConnect = false;
                              cipstartConnectClient = NULL;                               
                               
                              TransportClientQueueData dt = clientsQueue[0];

                              CoreTransportClient* thisClient = dt.client;
                              removeClientFromQueue(thisClient);
                               
                              notifyClientConnected(*thisClient,false,CT_ERROR_CANT_CONNECT);
                            }
                            machineState = sim800Idle; // переходим к следующей команде
                          } // gsmConnectFail
                        } // isKnownAnswer                   
                    
                  }
                  break; // cmdCIPSTART


                  case smaWaitSendDone:
                  {
                    // дожидаемся конца отсыла данных от клиента в SIM800
                      
                      if(isKnownAnswer(thisCommandLine,knownAnswer))
                      {                                                
                        if(knownAnswer == gsmSendOk)
                        {
                          // send ok
                          if(clientsQueue.size())
                          {
                             #ifdef GSM_DEBUG_MODE
                                DBGLN(F("SIM800: data was sent."));
                             #endif
                             
                             TransportClientQueueData dt = clientsQueue[0];
                             
                             CoreTransportClient* thisClient = dt.client;
                             removeClientFromQueue(thisClient);

                             // очищаем данные у клиента
                             thisClient->clear();

                             notifyDataWritten(*thisClient,CT_ERROR_NONE);
                          }                     
                        } // send ok
                        else
                        {
                          // send fail
                          if(clientsQueue.size())
                          {
                             #ifdef GSM_DEBUG_MODE
                                DBGLN(F("SIM800: send data fail!"));
                             #endif
                             
                             TransportClientQueueData dt = clientsQueue[0];

                             CoreTransportClient* thisClient = dt.client;
                             removeClientFromQueue(thisClient);
                                                          
                             // очищаем данные у клиента
                             thisClient->clear();
                             
                             notifyDataWritten(*thisClient,CT_ERROR_CANT_WRITE);
                             
                          }                     
                        } // else send fail
  
                        machineState = sim800Idle; // переходим к следующей команде
                        
                      } // if(isKnownAnswer(thisCommandLine,knownAnswer))
                       

                  }
                  break; // smaWaitSendDone

                  case smaCIPSEND:
                  {
                    // тут отсылали запрос на запись данных с клиента
                    if(thisCommandLine == F(">"))
                    {
                       // дождались приглашения, можем писать в SIM800
                       // тут пишем напрямую
                       if(clientsQueue.size())
                       {
                          // говорим, что ждём окончания отсыла данных
                          currentCommand = smaWaitSendDone;                          
                          TransportClientQueueData dt = clientsQueue[0];                     

                          #ifdef GSM_DEBUG_MODE
                              DBG(F("SIM800: > received, start write from client to SIM800, DATA LENGTH: "));
                              DBGLN(String(dt.dataLength));
                          #endif

                          flags.ignoreNextEmptyLine = true;
                          
                          for(size_t kk=0;kk<dt.dataLength;kk++)
                          {
                            workStream->write(dt.data[kk]);
                            readFromStream();

                             #ifdef USE_WIFI_MODULE
                             // и модуль ESP тоже тут обновим
                             ESP.readFromStream();
                             #endif                             
                          }
                          
                          delete [] clientsQueue[0].data;
                          delete [] clientsQueue[0].ip;
                          clientsQueue[0].data = NULL;
                          clientsQueue[0].ip = NULL;
                          clientsQueue[0].dataLength = 0;

                          // очищаем данные у клиента сразу после отсыла
                           dt.client->clear();
                       }
                    } // if
                    else
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(knownAnswer == gsmError || knownAnswer == gsmSendFail)
                      {
                           // всё плохо, не получилось ничего записать
                          if(clientsQueue.size())
                          {
                             #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Client write ERROR!"));
                             #endif
                             
                             TransportClientQueueData dt = clientsQueue[0];
    
                             CoreTransportClient* thisClient = dt.client;
                             removeClientFromQueue(thisClient);
    
                             // очищаем данные у клиента
                             thisClient->clear();
    
                             notifyDataWritten(*thisClient,CT_ERROR_CANT_WRITE);
                            
                          }
                          
                        machineState = sim800Idle; // переходим к следующей команде
                      }                                           
              
                    } // else can't write
                    
                  }
                  break; // smaCIPSEND
                 
                  case smaCheckReady: // ждём готовности модема, ответ на команду AT+CPAS?
                  {
                      if( thisCommandLine.startsWith( F("+CPAS:") ) ) 
                      {
                          // это ответ на команду AT+CPAS, можем его разбирать
                          if(thisCommandLine == F("+CPAS: 0")) 
                          {
                            // модем готов, можем убирать команду из очереди и переходить к следующей
                            #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Modem ready."));
                            #endif
                            
                            machineState = sim800Idle; // и переходим на следующую
                        }
                        else 
                        {
                           #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Modem NOT ready, try again later..."));
                           #endif
                           
                           idleTime = 2000; // повторим через 2 секунды
                           flags.onIdleTimer = true;
                           idleTimer = millis();
                           // добавляем ещё раз эту команду
                           initCommandsQueue.push_back(smaCheckReady);
                           machineState = sim800Idle; // и пошлём ещё раз команду проверки готовности           
                        }
                      }                    
                  }
                  break; // cmdWantReady


                  case smaHangUp:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                        DBGLN(F("SIM800: Call dropped."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                    }                    
                  }
                  break; // smaHangUp

                  case smaCIPHEAD:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                        DBGLN(F("SIM800: CIPHEAD command processed."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaCIPHEAD

                  case smaCIPSHUT:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(knownAnswer == gsmShutOk || knownAnswer == gsmError)
                      {
                        #ifdef GSM_DEBUG_MODE
                          DBGLN(F("SIM800: CIPSHUT command processed."));
                        #endif
                        machineState = sim800Idle; // переходим к следующей команде
                      }
                    }
                  }
                  break; // smaCIPSHUT                  

                  case smaCIICR:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: CIICR command processed."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде

                      if(knownAnswer == gsmOK)
                      {
                        // тут можем добавлять новые команды для GPRS
                        idleTime = 1000; // обработаем ответ через 1 секунду
                        flags.onIdleTimer = true;
                        idleTimer = millis();
                                                   
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: start checking GPRS connection..."));
                        #endif
                        initCommandsQueue.push_back(smaCIFSR);
                        gprsCheckingAttempts = 0;
                      }
                    }
                  }
                  break; // smaCIICR

                  case smaCIFSR:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      // если мы здесь - мы не получили IP-адреса, т.к. ответ - один из известных
                      
                      if(knownAnswer == gsmOK)
                      {

                        // Тут пробуем чуть позже ещё раз эту команду
                        if(++gprsCheckingAttempts <=5)
                        {
                          #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: try to get GPRS IP address a little bit later..."));
                          #endif
                          
                          idleTime = 5000; // обработаем ответ через 5 секунд
                          flags.onIdleTimer = true;
                          idleTimer = millis();
                          initCommandsQueue.push_back(smaCIFSR);
                          machineState = sim800Idle; // переходим к следующей команде  
                        }
                        else
                        {
                          #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Unable to get GPRS IP address!"));
                          #endif
                          // всё, исчерпали лимит на попытки получить IP-адрес
                          machineState = sim800Idle; // переходим к следующей команде
                          flags.gprsAvailable = false;
                        }
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: GPRS connection fail!"));
                        #endif
                        flags.gprsAvailable = false;
                        machineState = sim800Idle; // переходим к следующей команде
                      }
                    } // isKnownAnswer
                    else
                    if(thisCommandLine.length() && thisCommandLine.indexOf(".") != -1)
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBG(F("SIM800: GPRS IP address found - "));                      
                              DBGLN(thisCommandLine);
                      #endif
                      
                      flags.gprsAvailable = true;
                      machineState = sim800Idle; // переходим к следующей команде          
                    }
                  }
                  break; // smaCIFSR

                  case smaCSTT:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: CSTT command processed."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде

                      if(knownAnswer == gsmOK)
                      {
                        // тут можем добавлять новые команды для GPRS
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: start GPRS connection..."));
                        #endif
                        initCommandsQueue.push_back(smaCIICR);
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Can't start GPRS connection!"));
                        #endif
                      }
                    }                    
                  }
                  break; // smaCSTT

                  case smaCIPMODE:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: CIPMODE command processed."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaCIPMODE
                  
                  case smaCIPMUX:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: CIPMUX command processed."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaCIPMUX


                  case smaEchoOff:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Echo OFF command processed."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Echo OFF command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaEchoOff

                  case smaDisableCellBroadcastMessages:
                  {                    
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Broadcast SMS disabled."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Broadcast SMS command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaDisableCellBroadcastMessages

                  case smaAON:
                  {                    
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: AON is ON."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: AON command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }
                  }
                  break; // smaAON

                  case smaPDUEncoding:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: PDU format is set."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: PDU format command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }                    
                  }
                  break; // smaPDUEncoding

                  case smaUCS2Encoding:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: UCS2 encoding is set."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: UCS2 encoding command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }                                        
                  }
                  break; // smaUCS2Encoding

                  case smaSMSSettings:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: SMS settings is set."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: SMS settings command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }                                        
                  }
                  break; // smaSMSSettings

                  case smaGPRSMultislotClass:
                  {
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      if(gsmOK == knownAnswer)
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: GPRS multislot class is set."));
                        #endif
                      }
                      else
                      {
                        #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: GPRS multislot class command FAIL!"));
                        #endif
                      }
                      machineState = sim800Idle; // переходим к следующей команде
                    }                                        
                  }
                  break; // smaGPRSMultislotClass                  

                  case smaWaitReg:
                  {
                     if(thisCommandLine.indexOf(F("+CREG: 0,1")) != -1)
                        {
                          // зарегистрированы в GSM-сети
                             flags.isModuleRegistered = true;
                             #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: Modem registered in GSM!"));
                             #endif
                             machineState = sim800Idle;
                        } // if
                        else
                        {
                          // ещё не зарегистрированы
                            flags.isModuleRegistered = false;
                            idleTime = GSM_CHECK_REGISTRATION_INTERVAL; // повторим через 5 секунд
                            flags.onIdleTimer = true;
                            idleTimer = millis();
                            // добавляем ещё раз эту команду
                            initCommandsQueue.push_back(smaWaitReg);
                            machineState = sim800Idle;
                        } // else                    
                  }
                  break; // smaWaitReg
                  
                  case smaCheckModemHang:
                  {                    
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: modem answered and available."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                      
                    } // if(isKnownAnswer

                  }
                  break; // smaCheckModemHang

                  #ifdef GSM_PULL_GPRS_BY_PING
                  case smaPING:
                  {                    
                    if(isKnownAnswer(thisCommandLine,knownAnswer))
                    {
                      #ifdef GSM_DEBUG_MODE
                              DBGLN(F("SIM800: PING done."));
                      #endif
                      machineState = sim800Idle; // переходим к следующей команде
                      
                    } // if(isKnownAnswer

                  }
                  break; // smaPING
                  #endif // GSM_PULL_GPRS_BY_PING
                                    
                } // switch

                
              } // if(hasAnswerLine)
              
         
        }
        break; // sim800WaitAnswer

        case sim800Reboot:
        {
          // ждём перезагрузки модема
          uint32_t powerOffTime = 
          #ifdef USE_GSM_REBOOT_PIN
            GSM_REBOOT_TIME
          #else
          0
          #endif
          ;
          
          if(millis() - timer > powerOffTime)
          {
            #ifdef USE_GSM_REBOOT_PIN
              #ifdef GSM_DEBUG_MODE
                DBGLN(F("SIM800: turn power ON!"));
              #endif
              digitalWrite(GSM_REBOOT_PIN,GSM_POWER_ON);              
            #endif

            
            #ifdef USE_SIM800_POWERKEY
                digitalWrite(SIM800_POWERKEY_PIN,SIM800_POWERKEY_OFF_LEVEL);
            #endif
            
            machineState = sim800WaitInit;
            timer = millis();
            
          } // if
        }
        break; // smaReboot

        case sim800WaitInit:
        {
          uint32_t waitTime = 
          #ifdef USE_SIM800_POWERKEY
          SIM800_WAIT_POWERKEY_AFTER_POWER_ON
          #else
          0
          #endif
          ;
            
          if(millis() - timer > waitTime)
          { 
            #ifdef GSM_DEBUG_MODE
              DBGLN(F("SIM800: Power ON completed!"));
              DBGLN(F("SIM800: Turn gate OFF!"));
            #endif

              digitalWrite(SIM800_GATE_KEY,SIM800_GATE_OFF);
              
               #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: check STATUS pin..."));
               #endif

                while (digitalRead(SIM800_STATUS_PIN) != LOW)
                {
                  delay(100);
                }
                               
               #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: modem is OFF, turn gate ON..."));
               #endif

               digitalWrite(SIM800_GATE_KEY,SIM800_GATE_ON);
               delay(1000);

               #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: gate is ON!"));
               #endif


              #ifdef USE_SIM800_POWERKEY
              
               #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: use POWERKEY!"));
               #endif
                                
                digitalWrite(SIM800_POWERKEY_PIN,SIM800_POWERKEY_ON_LEVEL);
                delay(SIM800_POWERKEY_PULSE_DURATION);        
                digitalWrite(SIM800_POWERKEY_PIN,SIM800_POWERKEY_OFF_LEVEL);

              #endif            

            // теперь ждём загрузки модема
            idleTime = GSM_WAIT_AFTER_REBOOT_TIME; // подождём чуть-чуть...
            flags.onIdleTimer = true;
           
            machineState = sim800WaitBootBegin;
            idleTimer = millis();
                

          } // 
        }
        break; // sim800WaitInit

        case sim800WaitBootBegin:
        {
          #ifdef GSM_DEBUG_MODE
                DBGLN(F("SIM800: inited after reboot, check STATUS pin..."));
          #endif

              int count_status = 0;
              while (digitalRead(SIM800_STATUS_PIN) == LOW)
              {
                count_status++;
                if (count_status > 100)
                {
                #ifdef GSM_DEBUG_MODE
                      DBGLN(F("SIM800: STATUS pin FAIL, restart!!!"));
                #endif                  
                  restart();
                  machineState = sim800Reboot;
                  return;
                }
                delay(100);
              }

                #ifdef GSM_DEBUG_MODE
                      DBGLN(F("SIM800: power ON completed."));
                #endif                 
          
          sendCommand(F("AT"));
          machineState = sim800WaitBoot;          
        }
        break; // sim800WaitBootBegin

        case sim800WaitBoot:
        {
          if(hasAnswerLine)
          {
            #ifdef USE_GSM_REBOOT_PIN
              // используем управление питанием, ждём загрузки модема
              if(thisCommandLine == F("Call Ready") || thisCommandLine == F("SMS Ready"))
              {
                #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: BOOT FOUND, INIT!"));
                #endif
                restart();
              }
            #else
              // управление питанием не используем, здесь не надо ждать загрузки модема - достаточно дождаться ответа на команду
              if(isKnownAnswer(thisCommandLine,knownAnswer))
              {
                #ifdef GSM_DEBUG_MODE
                  DBGLN(F("SIM800: ANSWERED, INIT!"));
                #endif
                restart();
              }
               
            #endif
            
          } // if(hasAnswerLine)
             
        }
        break; // sim800WaitBoot
      
      } // switch

    } // if(!flags.onIdleTimer)


}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::begin()
{

  #ifdef GSM_DEBUG_MODE
   DBGLN(F("SIM800: begin."));
  #endif
    
  workStream = &GSM_SERIAL;
  GSM_SERIAL.begin(GSM_BAUD_RATE, SERIAL_8N1);
  restart();

  pinMode(SIM800_STATUS_PIN,INPUT);
  pinMode(SIM800_GATE_KEY,OUTPUT);
  digitalWrite(SIM800_GATE_KEY,SIM800_GATE_OFF);

  #ifdef USE_GSM_REBOOT_PIN
  
    #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: power OFF!"));
    #endif
    // есть пин, который надо использовать при зависании
    pinMode(GSM_REBOOT_PIN,OUTPUT);
    digitalWrite(GSM_REBOOT_PIN,GSM_POWER_OFF);
  #endif

  #ifdef USE_SIM800_POWERKEY
      pinMode(SIM800_POWERKEY_PIN,OUTPUT);
      digitalWrite(SIM800_POWERKEY_PIN, SIM800_POWERKEY_OFF_LEVEL);
  #endif
  
  machineState = sim800Reboot;

}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::sendCUSD(const String& cusd)
{
  if(!ready())
    return;

  #ifdef GSM_DEBUG_MODE
    DBG(F("SIM800: CUSD REQUESTED: "));
    DBGLN(cusd);
  #endif


  unsigned int bp = 0;
  String out;
  
  PDU.UTF8ToUCS2(cusd, bp, &out);

  String* completeCommand = new String(F("AT+CUSD=1,\""));
  *completeCommand += out.c_str();
  *completeCommand += F("\"");

   cusdList.push_back(completeCommand);
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreSIM800Transport::sendSMS(const String& phoneNumber, const String& message, bool isFlash)
{
  if(!ready())
    return false;
    
    SIM800OutgoingSMS queuedSMS;
    
    queuedSMS.isFlash = isFlash;   
    queuedSMS.phone = new String(phoneNumber.c_str());    
    queuedSMS.message = new String(message.c_str());    

    outgoingSMSList.push_back(queuedSMS);

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::restart()
{
  // очищаем входной буфер
  receiveBuffer.clear();

  delete smsToSend;
  smsToSend = new String();

  // очищаем очередь клиентов, заодно им рассылаем события
  clearClientsQueue(true);  

  // т.к. мы ничего не инициализировали - говорим, что мы не готовы предоставлять клиентов
  flags.ready = false;
  flags.isAnyAnswerReceived = false;
  flags.waitForDataWelcome = false;
  flags.onIdleTimer = false;
  flags.isModuleRegistered = false;
  flags.gprsAvailable = false;
  flags.ignoreNextEmptyLine = false;
  
  timer = millis();

  flags.waitCipstartConnect = false; // не ждёт соединения внешнего клиента
  cipstartConnectClient = NULL;  

  currentCommand = smaNone;
  machineState = sim800Idle;

  // инициализируем очередь командами по умолчанию
 createInitCommands(true);
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::createInitCommands(bool addResetCommand)
{  
  UNUSED(addResetCommand);
  
  // очищаем очередь команд
  clearInitCommands();

  // если указаны параметры APN - при старте поднимаем GPRS
  String apn = GetAPN();
  if(apn.length())
  {
    initCommandsQueue.push_back(smaCSTT);
  }

  initCommandsQueue.push_back(smaCIPSHUT);

  initCommandsQueue.push_back(smaCIPMUX);
  initCommandsQueue.push_back(smaCIPMODE);
  initCommandsQueue.push_back(smaWaitReg); // ждём регистрации
  
  initCommandsQueue.push_back(smaCIPHEAD);
  
  initCommandsQueue.push_back(smaGPRSMultislotClass); // настройки вывода SMS
  initCommandsQueue.push_back(smaSMSSettings); // настройки вывода SMS
  initCommandsQueue.push_back(smaUCS2Encoding); // кодировка сообщений
  initCommandsQueue.push_back(smaPDUEncoding); // формат сообщений
  initCommandsQueue.push_back(smaAON); // включение АОН
  initCommandsQueue.push_back(smaDisableCellBroadcastMessages); // выключение броадкастовых SMS
  initCommandsQueue.push_back(smaCheckReady); // проверка готовности    
  initCommandsQueue.push_back(smaEchoOff); // выключение эха
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::clearInitCommands()
{
  initCommandsQueue.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::clearClientsQueue(bool raiseEvents)
{
 // тут попросили освободить очередь клиентов.
  // для этого нам надо выставить каждому клиенту флаг того, что он свободен,
  // плюс - сообщить, что текущее действие над ним не удалось.  

    for(size_t i=0;i<clientsQueue.size();i++)
    {
        TransportClientQueueData dt = clientsQueue[i];
        delete [] dt.data;
        delete [] dt.ip;

        // если здесь в очереди есть хоть один клиент с неназначенным ID (ждёт подсоединения) - то в события он не придёт,
        // т.к. там сравнивается по назначенному ID. Поэтому мы назначаем ID клиенту в первый свободный слот.
        if(dt.client->socket == NO_CLIENT_ID)
        {
          CoreTransportClient* cl = getClient(NO_CLIENT_ID);
          if(cl)
            dt.client->bind(cl->socket);
        }
        
        if(raiseEvents)
        {
          switch(dt.action)
          {
            case actionDisconnect:
                // при дисконнекте всегда считаем, что ошибок нет
                notifyClientConnected(*(dt.client),false,CT_ERROR_NONE);
            break;
  
            case actionConnect:
                // если было запрошено соединение клиента с адресом - говорим, что соединиться не можем
                notifyClientConnected(*(dt.client),false,CT_ERROR_CANT_CONNECT);
            break;
  
            case actionWrite:
              // если попросили записать данные - надо сообщить подписчикам, что не можем записать данные
              notifyDataWritten(*(dt.client),CT_ERROR_CANT_WRITE);
              notifyClientConnected(*(dt.client),false,CT_ERROR_NONE);
            break;
          } // switch
          

        } // if(raiseEvents)
        
    } // for

  clientsQueue.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreSIM800Transport::isClientInQueue(CoreTransportClient* client, TransportClientAction action)
{
  for(size_t i=0;i<clientsQueue.size();i++)
  {
    if(clientsQueue[i].client == client && clientsQueue[i].action == action)
      return true;
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::addClientToQueue(CoreTransportClient* client, TransportClientAction action, const char* ip, uint16_t port)
{
  while(isClientInQueue(client, action))
  {
    removeClientFromQueue(client,action);
  }

    TransportClientQueueData dt;
    dt.client = client;
    dt.action = action;
    
    dt.ip = NULL;
    if(ip)
    {
      dt.ip = new char[strlen(ip)+1];
      strcpy(dt.ip,ip);
    }
    dt.port = port;

    clientsQueue.push_back(dt);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::removeClientFromQueue(CoreTransportClient* client, TransportClientAction action)
{
  
  for(size_t i=0;i<clientsQueue.size();i++)
  {
    if(clientsQueue[i].client == client && clientsQueue[i].action == action)
    {
      delete [] clientsQueue[i].ip;
      delete [] clientsQueue[i].data;
      
        for(size_t j=i+1;j<clientsQueue.size();j++)
        {
          clientsQueue[j-1] = clientsQueue[j];
        }
        
        clientsQueue.pop();
        break;
    } // if
    
  } // for  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::removeClientFromQueue(CoreTransportClient* client)
{
  for(size_t i=0;i<clientsQueue.size();i++)
  {
    if(clientsQueue[i].client == client)
    {
      delete [] clientsQueue[i].ip;
      delete [] clientsQueue[i].data;
      
        for(size_t j=i+1;j<clientsQueue.size();j++)
        {
          clientsQueue[j-1] = clientsQueue[j];
        }
        
        clientsQueue.pop();
        break;
    } // if
    
  } // for
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::beginWrite(CoreTransportClient& client)
{

  // добавляем клиента в очередь на запись
  addClientToQueue(&client, actionWrite);

  // клиент добавлен, теперь при обновлении транспорта мы начнём работать с записью в поток с этого клиента
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::beginConnect(CoreTransportClient& client, const char* ip, uint16_t port)
{
  if(client.connected())
  {
    
    #ifdef GSM_DEBUG_MODE
      DBGLN(F("SIM800: client already connected!"));
    #endif
    return;
    
  }
  
  // добавляем клиента в очередь на соединение
  addClientToQueue(&client, actionConnect, ip, port);

  // клиент добавлен, теперь при обновлении транспорта мы начнём работать с соединением клиента
  
}
//--------------------------------------------------------------------------------------------------------------------------------------
void CoreSIM800Transport::beginDisconnect(CoreTransportClient& client)
{
  if(!client.connected())
  {
    return;
  }

  // добавляем клиента в очередь на соединение
  addClientToQueue(&client, actionDisconnect);

  // клиент добавлен, теперь при обновлении транспорта мы начнём работать с отсоединением клиента
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool CoreSIM800Transport::ready()
{
  return flags.ready && flags.isAnyAnswerReceived && flags.isModuleRegistered; // если мы полностью инициализировали SIM800 - значит, можем работать
}
//--------------------------------------------------------------------------------------------------------------------------------------

