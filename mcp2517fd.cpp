/*
  mcp2517fd.cpp - Library for mcp2517fd Arduino CAN shield
  ver1.0 created by Sherzaad Dinah
*/
#include "mcp2517fd.h"

void mcp2517fd::getcurrentCiFIFOCON(CAN_FIFO_CHANNEL channel)
{
    uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    REG_CiFIFOCON ciFifoCon;

    ciFifoCon.dword = 0;
    ciFifoCon.dword = ReadDWord(a);
    Serial.print("ciFifoCon.dword is ");
    Serial.print(ciFifoCon.dword, BIN);
    Serial.print("  ");
    Serial.println(ciFifoCon.txBF.TxEnable, BIN);
}

void mcp2517fd::getcurrentTXREQandUINC(CAN_FIFO_CHANNEL channel)
{
    REG_CiFIFOCON ciFifoCon;
    
    // Set UINC
    uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
    ciFifoCon.dword = 0;
    uint8_t rxd = ReadByte(a);
    Serial.print("getcurrentTXREQandUINC is ");
    Serial.println(rxd, BIN);
}
void mcp2517fd::getcurrentTxSta()
{
    txFlags = TransmitChannelEventGet(CAN_FIFO_CH1); //CAN_FIFO_CH1 use for TX
    Serial.print("getcurrentTxSta txFlags is ");
    Serial.println(txFlags, BIN);
}

void mcp2517fd::getcurrentFIFOSTA(CAN_FIFO_CHANNEL channel)
{
    uint32_t fifoReg[3];
    REG_CiFIFOSTA ciFifoSta;
    uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
    ReadDWordArray(a, fifoReg, 3);
    
    // Get status
    ciFifoSta.dword = fifoReg[1];
    Serial.print("channel ");
    Serial.print(channel, HEX);
    Serial.print(" ciFifoSta status is ");
    Serial.println(ciFifoSta.dword, HEX);
}

// *****************************************************************************
// *****************************************************************************
// chaoqian modify
void mcp2517fd::DRV_SPI_Initialize()
{
//#ifndef SPI_Init     //chaoqian modify
//#define SPI_Init
    //SPI clock speed:speed, Data Shift:MSB First, Data Clock Idle: SPI_MODE0
    //SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
//#endif
    Serial.println(F("DRV_SPI_Initialize"));
    //pinMode(int0_pin, INPUT);
    //pinMode(intr_pin, INPUT_PULLUP);
    pinMode(cs_pin, OUTPUT);
    pinMode(int0_pin, INPUT);
    pinMode(intr_pin, INPUT_PULLUP);
    digitalWrite(cs_pin,HIGH);

    
    cs_mask = digitalPinToBitMask(cs_pin);
    cs_reg = portOutputRegister(digitalPinToPort(cs_pin));
    
    intr_mask = digitalPinToBitMask(intr_pin);
    intr_reg = portInputRegister(digitalPinToPort(intr_pin));
    
    Serial.println("init reg print-----------");
    Serial.print("cs_pin/cs_mask/cs_reg is ");
    Serial.print(cs_pin, DEC);
    Serial.print("/");
    Serial.print(cs_mask, BIN);
    Serial.print("/");
    Serial.println(*cs_reg, BIN);
    Serial.print("intr_pin/intr_mask/intr_reg is ");
    Serial.print(intr_pin, DEC);
    Serial.print("/");
    Serial.print(intr_mask, BIN);
    Serial.print("/");
    Serial.println(*intr_reg, BIN);
    Serial.println("init reg print end-----------");

//    SPI.setMISO(12);       // SCK on pin 14
//    SPI.setSCK(13);       // SCK on pin 13
//    SPI.setMOSI(11);
     
    SPI.begin();
    Serial.println(F("DRV_SPI_Initialize end"));
    
    pinMode (test_pin, OUTPUT);
    //digitalWrite(test_pin,HIGH);



}

// *****************************************************************************
// *****************************************************************************
// Section: send out message  chaoqian modify

void mcp2517fd::APP_TransmitMessageQueue(void)
{


      Serial.println(F("APP_TransmitMessageQueue start"));
      int8_t result;
      uint8_t tec, rec;
      uint8_t attempts = MAX_TXQUEUE_ATTEMPTS;

      // Check if FIFO is not full
      do{
        txFlags = TransmitChannelEventGet(CAN_FIFO_CH1); //CAN_FIFO_CH1 use for TX
        Serial.print("txFlags and attempts is ");
        Serial.print(txFlags, BIN);
        Serial.print("  ");
        Serial.println(attempts, DEC);
        if(attempts == 0) {
            ErrorCountStateGet(&tec, &rec, &errorFlags);
            return;
        }
        attempts--;

      } while (!(txFlags & CAN_TX_FIFO_NOT_FULL_EVENT));

      // Load message and transmit
      uint8_t n = DLCtoDataLength(txObj.bF.ctrl.DLC);
      Serial.print("DLC and n and sid sid is ");
      Serial.print(txObj.bF.ctrl.DLC, DEC);
      Serial.print("  ");
      Serial.print(txObj.bF.id.SID, HEX);
      Serial.print("  ");
      Serial.print(txObj.bF.id.SID, BIN);
      Serial.print("  ");
      Serial.println(n, DEC);

      result = TransmitChannelLoad(&txObj, txd, n, CAN_FIFO_CH1, true);

    //+++++++++++++++++++++
    getcurrentTxSta();
    getcurrentFIFOSTA(CAN_FIFO_CH1);
    getcurrentFIFOSTA(CAN_FIFO_CH2);
    getcurrentTXREQandUINC(CAN_FIFO_CH1);
}

void mcp2517fd::APP_ReceiveMessage_Tasks()
{
    //Serial.println("APP_ReceiveMessage_Tasks start");
    if (available())
    {
      //Serial.println("CAN Bus is available start**************************");
      //read out the data
      uint8_t dlc, i, rxstep;
      
      //getcurrentFIFOSTA(CAN_FIFO_CH2);
      
      rxFlags = ReceiveChannelEventGet(CAN_FIFO_CH2);
      //Serial.print("rxFlags is ");
      //Serial.println(rxFlags, BIN);
      if (rxFlags & CAN_RX_FIFO_NOT_EMPTY_EVENT) {
        // Read the message
        ReceiveMessageGet(&rxObj, rxd, MAX_DATA_BYTES);
        dlc = DLCtoDataLength(rxObj.bF.ctrl.DLC);
    
        Serial.print(' ');
    
        for (i = 0; i < 8; i++) {
          Serial.print(rxd[i], HEX);
        }
        Serial.println();
        Serial.println(rxd[2], HEX);
        if(rxd[2] == 0x11){
            Serial.println("YES YES !!!!!!!!!!");
            digitalWrite(test_pin,HIGH);
        }
        if(rxd[2] == 0x00){
            digitalWrite(test_pin,LOW);
            Serial.println("NO NO !!!!!!!!!!");
        }

        Serial.println(' ');
      }
      //Serial.println("CAN Bus is available end**************************");
    }
    //Serial.println("APP_ReceiveMessage_Tasks end");
}

int8_t mcp2517fd::DRV_SPI_TransferData(uint8_t *SpiTxData, uint8_t *SpiRxData, uint16_t spiTransferSize)
{
  uint8_t i;
     
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));//chaoqian modify

  RESET_CS();
  
  for(i=0;i<spiTransferSize;i++)
  {
      SpiRxData[i] = SPI.transfer(SpiTxData[i]);
  }
  
  SET_CS();
  SPI.endTransaction();
  
  return 0;
   
}

// *****************************************************************************
// *****************************************************************************
// Section: Reset
void mcp2517fd::Reset()
{
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

  RESET_CS();
  Serial.println("Can bus Reset");

  SPI.transfer((uint8_t) (cINSTRUCTION_RESET << 4));
  SPI.transfer(0x00);
  SET_CS();

  SPI.endTransaction();

}

// *****************************************************************************
// *****************************************************************************
// Section: SPI Access Functions

uint8_t mcp2517fd::ReadByte(uint16_t address)
{
  uint16_t spiTransferSize = 3;
  int8_t spiTransferError = 0;

  uint8_t rxd;

    // chaoqian modify
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  spiTransmitBuffer[2] = 0;

  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

  // Update data
  rxd = spiReceiveBuffer[2];

  return rxd;
}

void mcp2517fd::WriteByte(uint16_t address, uint8_t txd)
{
  uint16_t spiTransferSize = 3;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  spiTransmitBuffer[2] = txd;
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);

  //return spiTransferError;
}

uint16_t mcp2517fd::ReadWord(uint16_t address)
{
  uint8_t i;
  uint32_t x;
  uint16_t spiTransferSize = 4;
  int8_t spiTransferError = 0;
  uint16_t rxd;
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  if (spiTransferError) {
      return spiTransferError;
  }
  
  // Update data
  rxd = 0;
  for (i = 2; i < 4; i++) {
      x = (uint32_t) spiReceiveBuffer[i];
      rxd += x << ((i - 2)*8);
  }
  
  return rxd;
}

void mcp2517fd::WriteWord(uint16_t address, uint16_t txd)
{
  uint8_t i;
  uint16_t spiTransferSize = 4;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  // Split word into 2 bytes and add them to buffer
  for (i = 0; i < 2; i++) {
      spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
  }
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  
  //return spiTransferError;
}

uint32_t mcp2517fd::ReadDWord(uint16_t address)
{
  uint8_t i;
  uint32_t x;
  uint16_t spiTransferSize = 6;
  int8_t spiTransferError = 0;
  uint32_t rxd;
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  if (spiTransferError) {
      return spiTransferError;
  }
  
  // Update data
  rxd = 0;
  for (i = 2; i < 6; i++) {
      x = (uint32_t) spiReceiveBuffer[i];
      rxd += x << ((i - 2)*8);
  }
  
  return rxd;

}

//void mcp2517fd::WriteDWord(uint16_t address, uint32_t txd)
//{
//  union {
//    uint32_t val;
//    uint8_t bytes[4];
//  } tx;
//
//  tx.val = txd;
//
////  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));//chaoqian modify
//  RESET_CS();
//
//  SPI.transfer((uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF)));
//  SPI.transfer((uint8_t) (address & 0xFF));
//
//  Serial.print("txd ");
//  Serial.println(txd, BIN);
//
//  for (uint8_t i = 0; i < sizeof(uint32_t); ++i) {
//    Serial.print("WriteDWord ");
//    Serial.println(tx.bytes[i], BIN);
//    SPI.transfer(tx.bytes[i]);
//  }
//
//  SET_CS();
////  SPI.endTransaction();
//}

void mcp2517fd::WriteDWord(uint16_t address, uint32_t txd)
{
  uint8_t i;
  uint16_t spiTransferSize = 6;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  // Split word into 4 bytes and add them to buffer
  for (i = 0; i < 4; i++) {
      spiTransmitBuffer[i + 2] = (uint8_t) ((txd >> (i * 8)) & 0xFF);
  }
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  
  //return spiTransferError;

}

void mcp2517fd::ReadByteArray(uint16_t address, uint8_t *rxd, uint16_t nBytes)
{
  uint16_t i;
  uint16_t spiTransferSize = nBytes + 2;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  // Clear data
  for (i = 2; i < spiTransferSize; i++) {
      spiTransmitBuffer[i] = 0;
  }
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  
  // Update data
  for (i = 0; i < nBytes; i++) {
      rxd[i] = spiReceiveBuffer[i + 2];
  }
  
  //return spiTransferError;

}

void mcp2517fd::WriteByteArray(uint16_t address, uint8_t *txd, uint16_t nBytes)
{
  uint16_t i;
  uint16_t spiTransferSize = nBytes + 2;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  
  // Add data
  for (i = 2; i < spiTransferSize; i++) {
      spiTransmitBuffer[i] = txd[i - 2];
  }
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  
  //return spiTransferError;

}

void mcp2517fd::WriteByteSafe(uint16_t address, uint8_t txd)
{
  union {
    uint16_t result;
    uint8_t bytes[2];
  } crc;

  uint8_t spiTransmitBuffer[3] = {(uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF)), (uint8_t) (address & 0xFF), txd};

  //calc CRC
  crc.result = CalculateCRC16(spiTransmitBuffer, 3);

  RESET_CS();

  SPI.transfer(spiTransmitBuffer[0]);
  SPI.transfer(spiTransmitBuffer[1]);
  SPI.transfer(spiTransmitBuffer[2]);
  SPI.transfer(crc.bytes[1]);
  SPI.transfer(crc.bytes[0]);

  SET_CS();
}

void mcp2517fd::WriteDWordSafe(uint16_t address, uint32_t txd)
{
  union {
    uint16_t result;
    uint8_t bytes[2];
  } crc;

  uint8_t spiTransmitBuffer[6] = {(uint8_t) ((cINSTRUCTION_WRITE_SAFE << 4) + ((address >> 8) & 0xF)), (uint8_t) (address & 0xFF), 0, 0, 0, 0};

  memcpy ( &spiTransmitBuffer[2], &txd, sizeof(txd) );

  //calc CRC
  crc.result = CalculateCRC16(spiTransmitBuffer, 6);

  RESET_CS();

  for (uint8_t i = 0; i < 6; ++i) {
    SPI.transfer(spiTransmitBuffer[i]);
  }

  SPI.transfer(crc.bytes[1]);
  SPI.transfer(crc.bytes[0]);

  SET_CS();
}

uint8_t mcp2517fd::ReadByteArrayWithCRC(uint16_t address, uint8_t *rxd, uint16_t nBytes, bool fromRam)
{
  union {
    uint16_t result;
    uint8_t bytes[2];
  } crc;

  uint8_t spiBuffer[nBytes + 5]; //first two bytes for sending command & address, third for size, last two bytes for CRC

  RESET_CS();

  // Compose command
  spiBuffer[0] = (uint8_t) ((cINSTRUCTION_READ_CRC << 4) + ((address >> 8) & 0xF));
  spiBuffer[1] = (uint8_t) (address & 0xFF);
  if (fromRam) {
    spiBuffer[2] = nBytes >> 2;
  } else {
    spiBuffer[2] = nBytes;
  }

  SPI.transfer(spiBuffer[0]);
  SPI.transfer(spiBuffer[1]);
  SPI.transfer(spiBuffer[2]);

  SPI.transfer(0x00);

  for (uint8_t i = 3; i < nBytes + 5; i++) {
    spiBuffer[i] = SPI.transfer(0x00);
  }

  SET_CS();

  // Get CRC from controller
  uint16_t crcFromSpiSlave = (uint16_t) (spiBuffer[nBytes + 3] << 8) + (uint16_t) (spiBuffer[nBytes + 4]);

  // Use the buffer to calculate CRC
  uint16_t crcAtController = CalculateCRC16(spiBuffer, nBytes + 3);

  // Compare CRC readings
  if (crcFromSpiSlave == crcAtController) {
    return 1;
  }
  else {
    return 0;
  }

  memcpy ( rxd, &spiBuffer[3], nBytes );
}

void mcp2517fd::WriteByteArrayWithCRC(uint16_t address, uint8_t *txd, uint16_t nBytes, bool fromRam)
{
  union {
    uint16_t result;
    uint8_t bytes[2];
  } crc;

  uint8_t spiTransmitBuffer[nBytes + 3];

  spiTransmitBuffer[0] = (uint8_t) ((cINSTRUCTION_WRITE_CRC << 4) + ((address >> 8) & 0xF));
  spiTransmitBuffer[1] = (uint8_t) (address & 0xFF);
  if (fromRam) {
    spiTransmitBuffer[2] = nBytes >> 2;
  } else {
    spiTransmitBuffer[2] = nBytes;
  }

  memcpy ( &spiTransmitBuffer[3], txd, nBytes );

  //calc CRC
  crc.result = CalculateCRC16(spiTransmitBuffer, nBytes + 3);

  RESET_CS();

  for (uint8_t i = 0; i < nBytes + 3; ++i) {
    SPI.transfer(spiTransmitBuffer[i]);
  }

  SPI.transfer(crc.bytes[1]);
  SPI.transfer(crc.bytes[0]);

  SET_CS();
}

void mcp2517fd::ReadDWordArray(uint16_t address, uint32_t *rxd, uint16_t nWords)
{
    uint16_t i, j, n;
    REG_t w;
    uint16_t spiTransferSize = nWords * 4 + 2;
    int8_t spiTransferError = 0;

    // Compose command
    spiTransmitBuffer[0] = (cINSTRUCTION_READ << 4) + ((address >> 8) & 0xF);
    spiTransmitBuffer[1] = address & 0xFF;

    // Clear data
    for (i = 2; i < spiTransferSize; i++) {
        spiTransmitBuffer[i] = 0;
    }

    spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
    if (spiTransferError) {
        //return spiTransferError;
    }

    // Convert Byte array to Word array
    n = 2;
    for (i = 0; i < nWords; i++) {
        w.dword = 0;
        for (j = 0; j < 4; j++, n++) {
            w.bytes[j] = spiReceiveBuffer[n];
        }
        rxd[i] = w.dword;
#if DEBUG_FLAG
        Serial.println(i, DEC);
        Serial.println(w.dword, BIN);
        Serial.println(rxd[i], BIN);
#endif
    }

    //return spiTransferError;
}

void mcp2517fd::WriteDWordArray(uint16_t address, uint32_t *txd, uint16_t nWords)
{
  uint16_t i, j, n;
  REG_t w;
  uint16_t spiTransferSize = nWords * 4 + 2;
  int8_t spiTransferError = 0;
  
  // Compose command
  spiTransmitBuffer[0] = (cINSTRUCTION_WRITE << 4) + ((address >> 8) & 0xF);
  spiTransmitBuffer[1] = address & 0xFF;
  
  // Convert ByteArray to word array
  n = 2;
  for (i = 0; i < nWords; i++) {
      w.dword = txd[i];
      for (j = 0; j < 4; j++, n++) {
          spiTransmitBuffer[n] = w.bytes[j];
      }
  }
  
  spiTransferError = DRV_SPI_TransferData(spiTransmitBuffer, spiReceiveBuffer, spiTransferSize);
  
  //return spiTransferError;

}
// *****************************************************************************
// *****************************************************************************
// Section: Configuration
void mcp2517fd::Configure(CAN_CONFIG* config)
{
  REG_CiCON ciCon;

  ciCon.dword = canControlResetValues[cREGADDR_CiCON / 4];

  ciCon.bF.DNetFilterCount = config->DNetFilterCount;
  ciCon.bF.IsoCrcEnable = config->IsoCrcEnable;
  ciCon.bF.ProtocolExceptionEventDisable = config->ProtocolExpectionEventDisable;
  ciCon.bF.WakeUpFilterEnable = config->WakeUpFilterEnable;
  ciCon.bF.WakeUpFilterTime = config->WakeUpFilterTime;
  ciCon.bF.BitRateSwitchDisable = config->BitRateSwitchDisable;
  ciCon.bF.RestrictReTxAttempts = config->RestrictReTxAttempts;
  ciCon.bF.EsiInGatewayMode = config->EsiInGatewayMode;
  ciCon.bF.SystemErrorToListenOnly = config->SystemErrorToListenOnly;
  ciCon.bF.StoreInTEF = config->StoreInTEF;
  ciCon.bF.TXQEnable = config->TXQEnable;
  ciCon.bF.TxBandWidthSharing = config->TxBandWidthSharing;

  WriteDWord(cREGADDR_CiCON, ciCon.dword);
}

void mcp2517fd::ConfigureObjectReset(CAN_CONFIG* config)
{
  REG_CiCON ciCon;
  ciCon.dword = canControlResetValues[cREGADDR_CiCON / 4];

  config->DNetFilterCount = ciCon.bF.DNetFilterCount;
  config->IsoCrcEnable = ciCon.bF.IsoCrcEnable;
  config->ProtocolExpectionEventDisable = ciCon.bF.ProtocolExceptionEventDisable;
  config->WakeUpFilterEnable = ciCon.bF.WakeUpFilterEnable;
  config->WakeUpFilterTime = ciCon.bF.WakeUpFilterTime;
  config->BitRateSwitchDisable = ciCon.bF.BitRateSwitchDisable;
  config->RestrictReTxAttempts = ciCon.bF.RestrictReTxAttempts;
  config->EsiInGatewayMode = ciCon.bF.EsiInGatewayMode;
  config->SystemErrorToListenOnly = ciCon.bF.SystemErrorToListenOnly;
  config->StoreInTEF = ciCon.bF.StoreInTEF;
  config->TXQEnable = ciCon.bF.TXQEnable;
  config->TxBandWidthSharing = ciCon.bF.TxBandWidthSharing;
}

// *****************************************************************************
// *****************************************************************************
// Section: Operating mode
void mcp2517fd::OperationModeSelect(CAN_OPERATION_MODE opMode)
{
  // Read
  uint8_t d = ReadByte(cREGADDR_CiCON + 3);

  // Modify
  d &= ~0x07;
  d |= opMode;

  // Write
  WriteByte(cREGADDR_CiCON + 3, d);
}

CAN_OPERATION_MODE mcp2517fd::OperationModeGet()
{
  CAN_OPERATION_MODE mode = CAN_INVALID_MODE;

  // Read Opmode
  uint8_t d = ReadByte(cREGADDR_CiCON + 3);

  // Get Opmode bits
  d = (d >> 5) & 0x7;

  // Decode Opmode
  switch (d) {
    case CAN_NORMAL_MODE:
      mode = CAN_NORMAL_MODE;
      break;
    case CAN_SLEEP_MODE:
      mode = CAN_SLEEP_MODE;
      break;
    case CAN_INTERNAL_LOOPBACK_MODE:
      mode = CAN_INTERNAL_LOOPBACK_MODE;
      break;
    case CAN_EXTERNAL_LOOPBACK_MODE:
      mode = CAN_EXTERNAL_LOOPBACK_MODE;
      break;
    case CAN_LISTEN_ONLY_MODE:
      mode = CAN_LISTEN_ONLY_MODE;
      break;
    case CAN_CONFIGURATION_MODE:
      mode = CAN_CONFIGURATION_MODE;
      break;
    case CAN_CLASSIC_MODE:
      mode = CAN_CLASSIC_MODE;
      break;
    case CAN_RESTRICTED_MODE:
      mode = CAN_RESTRICTED_MODE;
      break;
    default:
      mode = CAN_INVALID_MODE;
      break;
  }

  return mode;
}

// *****************************************************************************
// *****************************************************************************
// Section: CAN Transmit
void mcp2517fd::TransmitChannelConfigure(CAN_TX_FIFO_CONFIG* config, CAN_FIFO_CHANNEL channel)
{
  // Setup FIFO
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];
  ciFifoCon.txBF.TxEnable = 1;
  ciFifoCon.txBF.FifoSize = config->FifoSize;
  ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
  ciFifoCon.txBF.TxAttempts = config->TxAttempts;
  ciFifoCon.txBF.TxPriority = config->TxPriority;
  ciFifoCon.txBF.RTREnable = config->RTREnable;

  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

  WriteDWord(a, ciFifoCon.dword);

#if DEBUG_FLAG
  //--------------------------------------------------
  ciFifoCon.dword = ReadDWord(a);
  Serial.print("ciFifoCon.dword is ");
  Serial.println(ciFifoCon.dword, BIN);
  Serial.println(ciFifoCon.txBF.TxEnable, BIN);
  //----------------------------------------------
#endif
}

void mcp2517fd::TransmitChannelConfigureObjectReset(CAN_TX_FIFO_CONFIG* config)
{
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  config->RTREnable = ciFifoCon.txBF.RTREnable;
  config->TxPriority = ciFifoCon.txBF.TxPriority;
  config->TxAttempts = ciFifoCon.txBF.TxAttempts;
  config->FifoSize = ciFifoCon.txBF.FifoSize;
  config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;
}

uint8_t mcp2517fd::TransmitQueueConfigure(CAN_TX_QUEUE_CONFIG* config)
{
#ifndef CAN_TXQUEUE_IMPLEMENTED
  return 0;
#else
  // Setup FIFO
  REG_CiTXQCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  ciFifoCon.txBF.TxEnable = 1;
  ciFifoCon.txBF.FifoSize = config->FifoSize;
  ciFifoCon.txBF.PayLoadSize = config->PayLoadSize;
  ciFifoCon.txBF.TxAttempts = config->TxAttempts;
  ciFifoCon.txBF.TxPriority = config->TxPriority;

  uint16_t a = cREGADDR_CiTXQCON;
  WriteDWord(a, ciFifoCon.dword);

  return 1;
#endif
}

void mcp2517fd::TransmitQueueConfigureObjectReset(CAN_TX_QUEUE_CONFIG* config)
{
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  config->TxPriority = ciFifoCon.txBF.TxPriority;
  config->TxAttempts = ciFifoCon.txBF.TxAttempts;
  config->FifoSize = ciFifoCon.txBF.FifoSize;
  config->PayLoadSize = ciFifoCon.txBF.PayLoadSize;
}

void mcp2517fd::TransmitChannelUpdate(CAN_FIFO_CHANNEL channel, bool flush)
{
  REG_CiFIFOCON ciFifoCon;

  // Set UINC
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
  ciFifoCon.dword = 0;
  ciFifoCon.txBF.UINC = 1;

  // Set TXREQ
  if (flush) {
    ciFifoCon.txBF.TxRequest = 1;
  }

  WriteByte(a, ciFifoCon.bytes[1]);

  getcurrentTXREQandUINC(CAN_FIFO_CH1);
}

int8_t mcp2517fd::TransmitChannelLoad(CAN_TX_MSGOBJ* txObj, uint8_t *txd, uint32_t txdNumBytes, CAN_FIFO_CHANNEL channel, bool flush)
{
  uint32_t fifoReg[3];
  REG_CiFIFOCON ciFifoCon;
  REG_CiFIFOSTA ciFifoSta;
  REG_CiFIFOUA ciFifoUa;

  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

  ReadDWordArray(a, fifoReg, 3);

  // Check that it is a transmit buffer
  ciFifoCon.dword = fifoReg[0];
  Serial.print("TransmitChannelLoad a and ciFifoCon.txBF.TxEnable is ");
  Serial.print(a, HEX);
  Serial.print("/");
  Serial.println(ciFifoCon.txBF.TxEnable, BIN);

  if (!ciFifoCon.txBF.TxEnable) {    //chaoqian modify
    Serial.println(" error -2");
    return -2;
  }

  // Check that DLC is big enough for data
  uint32_t dataBytesInObject = DLCtoDataLength(txObj->bF.ctrl.DLC);

  if (dataBytesInObject < txdNumBytes) {
    return -1;
  }

  // Get status
  ciFifoSta.dword = fifoReg[1];
  Serial.print(" ciFifoSta status is ");
  Serial.println(ciFifoSta.dword, HEX);

  // Get address
  ciFifoUa.dword = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
  a = 4 * ciFifoUa.bF.UserAddress;
#else
  a = ciFifoUa.bF.UserAddress;
#endif
  a += cRAMADDR_START;

  uint8_t txBuffer[MAX_MSG_SIZE];

  txBuffer[0] = txObj->byte[0]; //not using 'for' to reduce no of instructions
  txBuffer[1] = txObj->byte[1];
  txBuffer[2] = txObj->byte[2];
  txBuffer[3] = txObj->byte[3];
  txBuffer[4] = txObj->byte[4];
  txBuffer[5] = txObj->byte[5];
  txBuffer[6] = txObj->byte[6];
  txBuffer[7] = txObj->byte[7];


  for (uint8_t i = 0; i < txdNumBytes; i++) {
    txBuffer[i + 8] = txd[i];
  }
    uint8_t aa = 0;
    for (aa;aa < 8;aa++)
    {
      Serial.print(txBuffer[aa], HEX);
    }
    Serial.println();

  // Make sure we write a multiple of 4 bytes to RAM
  uint16_t n = 0;
  uint8_t j = 0;

  if (txdNumBytes % 4) {
    // Need to add bytes
    n = 4 - (txdNumBytes % 4);
    uint8_t i = txdNumBytes + 8;

    for (j = 0; j < n; j++) {
      txBuffer[i + 8 + j] = 0;
    }
  }

  WriteByteArray(a, txBuffer, txdNumBytes + 8 + n);

  // Set UINC and TXREQ
  TransmitChannelUpdate(channel, flush);

  return 1;
}

void mcp2517fd::TransmitChannelFlush(CAN_FIFO_CHANNEL channel)
{
  // Address of TXREQ
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1;

  // Set TXREQ
  uint8_t d = 0x02;

  // Write
  WriteByte(a, d);
}

CAN_TX_FIFO_STATUS mcp2517fd::TransmitChannelStatusGet(CAN_FIFO_CHANNEL channel)
{
  uint32_t sta = 0;
  uint32_t fifoReg[2];
  REG_CiFIFOSTA ciFifoSta;
  REG_CiFIFOCON ciFifoCon;

  // Get FIFO registers
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

  ReadDWordArray(a, fifoReg, 2);

  // Update data
  ciFifoCon.dword = fifoReg[0];
  ciFifoSta.dword = fifoReg[1];

  // Update status
  sta = ciFifoSta.bytes[0];

  if (ciFifoCon.txBF.TxRequest) {
    sta |= CAN_TX_FIFO_TRANSMITTING;
  }

  return (CAN_TX_FIFO_STATUS) (sta & CAN_TX_FIFO_STATUS_MASK);
}

void mcp2517fd::TransmitChannelReset(CAN_FIFO_CHANNEL channel)
{
  ReceiveChannelReset(channel);
}

void mcp2517fd::TransmitRequestSet(CAN_TXREQ_CHANNEL txreq)
{
  // Write TXREQ register
  uint32_t w = txreq;

  WriteDWord(cREGADDR_CiTXREQ, w);
}

uint32_t mcp2517fd::TransmitRequestGet()
{
  return ReadDWord(cREGADDR_CiTXREQ);
}

void mcp2517fd::TransmitChannelAbort(CAN_FIFO_CHANNEL channel)
{
  // Address
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // byte address of TXREQ

  // Clear TXREQ
  uint8_t d = 0x00;

  // Write
  WriteByte(a, d);
}

void mcp2517fd::TransmitAbortAll()
{
  // Read CiCON byte 3
  uint8_t d = ReadByte((cREGADDR_CiCON + 3));

  // Modify
  d |= 0x8;

  // Write
  WriteByte((cREGADDR_CiCON + 3), d);
}

void mcp2517fd::TransmitBandWidthSharingSet(CAN_TX_BANDWITH_SHARING txbws)
{
  // Read CiCON byte 3
  uint8_t d = ReadByte((cREGADDR_CiCON + 3));

  // Modify
  d &= 0x0f;
  d |= (txbws << 4);

  // Write
  WriteByte((cREGADDR_CiCON + 3), d);
}

// *****************************************************************************
// *****************************************************************************
// Section: CAN Receive
void mcp2517fd::FilterObjectConfigure(CAN_FILTER filter, CAN_FILTEROBJ_ID* id)
{
  REG_CiFLTOBJ fObj;

  // Setup
  fObj.dword = 0;
  fObj.bF = *id;

  uint16_t a = cREGADDR_CiFLTOBJ + (filter * CiFILTER_OFFSET);

  WriteDWord(a, fObj.dword);
}

void mcp2517fd::FilterMaskConfigure(CAN_FILTER filter, CAN_MASKOBJ_ID* mask)
{
  REG_CiMASK mObj;

  // Setup
  mObj.dword = 0;
  mObj.bF = *mask;
  uint16_t a = cREGADDR_CiMASK + (filter * CiFILTER_OFFSET);

  WriteDWord(a, mObj.dword);
}

void mcp2517fd::FilterToFifoLink(CAN_FILTER filter, bool enable, CAN_FIFO_CHANNEL channel)
{
  REG_CiFLTCON_BYTE fCtrl;

  // Enable
  if (enable) {
    fCtrl.bF.Enable = 1;
  } else {
    fCtrl.bF.Enable = 0;
  }

  // Link
  fCtrl.bF.BufferPointer = channel;
  uint16_t a = cREGADDR_CiFLTCON + filter;

  WriteByte(a, fCtrl.bytes);
}

void mcp2517fd::FilterEnable(CAN_FILTER filter)
{
  REG_CiFLTCON_BYTE fCtrl;

  // Read
  uint16_t a = cREGADDR_CiFLTCON + filter;

  fCtrl.bytes = ReadByte(a);

  // Modify
  fCtrl.bF.Enable = 1;

  // Write
  WriteByte(a, fCtrl.bytes);
}

void mcp2517fd::FilterDisable(CAN_FILTER filter)
{
  REG_CiFLTCON_BYTE fCtrl;

  // Read
  uint16_t a = cREGADDR_CiFLTCON + filter;

  fCtrl.bytes = ReadByte(a);

  // Modify
  fCtrl.bF.Enable = 0;

  // Write
  WriteByte(a, fCtrl.bytes);
}

void mcp2517fd::DeviceNetFilterCountSet(CAN_DNET_FILTER_SIZE dnfc)
{
  uint8_t d = 0;

  // Read CiCON byte 0
  d = ReadByte(cREGADDR_CiCON);

  // Modify
  d &= 0x1f;
  d |= dnfc;

  // Write
  WriteByte(cREGADDR_CiCON, d);
}

uint8_t mcp2517fd::ReceiveChannelConfigure(CAN_RX_FIFO_CONFIG* config, CAN_FIFO_CHANNEL channel)
{
#ifdef CAN_TXQUEUE_IMPLEMENTED
  if (channel == CAN_TXQUEUE_CH0) {
    return 0;
  }
#endif

  // Setup FIFO
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  ciFifoCon.rxBF.TxEnable = 0;
  ciFifoCon.rxBF.FifoSize = config->FifoSize;
  ciFifoCon.rxBF.PayLoadSize = config->PayLoadSize;
  ciFifoCon.rxBF.RxTimeStampEnable = config->RxTimeStampEnable;

  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

  WriteDWord(a, ciFifoCon.dword);

  return 1;
}

void mcp2517fd::ReceiveChannelConfigureObjectReset(CAN_RX_FIFO_CONFIG* config)
{
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  config->FifoSize = ciFifoCon.rxBF.FifoSize;
  config->PayLoadSize = ciFifoCon.rxBF.PayLoadSize;
  config->RxTimeStampEnable = ciFifoCon.rxBF.RxTimeStampEnable;
}

CAN_RX_FIFO_STATUS mcp2517fd::ReceiveChannelStatusGet(CAN_FIFO_CHANNEL channel)
{
  REG_CiFIFOSTA ciFifoSta;

  // Read
  ciFifoSta.dword = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

  ciFifoSta.bytes[0] = ReadByte(a);

  // Update data
  return (CAN_RX_FIFO_STATUS) (ciFifoSta.bytes[0] & 0x0F);
}

uint8_t mcp2517fd::ReceiveMessageGet(CAN_RX_MSGOBJ* rxObj, uint8_t *rxd, uint8_t nBytes, CAN_FIFO_CHANNEL channel)
{
  uint8_t n = 0;
  uint8_t i = 0;
  uint16_t a;
  uint32_t fifoReg[3];
  REG_CiFIFOCON ciFifoCon;
  REG_CiFIFOSTA ciFifoSta;
  REG_CiFIFOUA ciFifoUa;
  int8_t spiTransferError = 0;

  // Get FIFO registers
  a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);

  ReadDWordArray(a, fifoReg, 3);

  // Check that it is a receive buffer
  ciFifoCon.dword = fifoReg[0];
  if (ciFifoCon.txBF.TxEnable) {
    return 0;
  }

  // Get Status
  ciFifoSta.dword = fifoReg[1];

  // Get address
  ciFifoUa.dword = fifoReg[2];
#ifdef USERADDRESS_TIMES_FOUR
  a = 4 * ciFifoUa.bF.UserAddress;
#else
  a = ciFifoUa.bF.UserAddress;
#endif
  a += cRAMADDR_START;

  // Number of bytes to read
  n = nBytes + 8; // Add 8 header bytes

  if (ciFifoCon.rxBF.RxTimeStampEnable) {
    n += 4; // Add 4 time stamp bytes
  }

  // Make sure we read a multiple of 4 bytes from RAM
  if (n % 4) {
    n = n + 4 - (n % 4);
  }

  // Read rxObj using one access
  uint8_t ba[MAX_MSG_SIZE];

  if (n > MAX_MSG_SIZE) {
    n = MAX_MSG_SIZE;
  }

  ReadByteArray(a, ba, n);

  // Assign message header
  REG_t myReg;

  myReg.bytes[0] = ba[0];
  myReg.bytes[1] = ba[1];
  myReg.bytes[2] = ba[2];
  myReg.bytes[3] = ba[3];
  rxObj->word[0] = myReg.dword;

  myReg.bytes[0] = ba[4];
  myReg.bytes[1] = ba[5];
  myReg.bytes[2] = ba[6];
  myReg.bytes[3] = ba[7];
  rxObj->word[1] = myReg.dword;

  if (ciFifoCon.rxBF.RxTimeStampEnable) {
    myReg.bytes[0] = ba[8];
    myReg.bytes[1] = ba[9];
    myReg.bytes[2] = ba[10];
    myReg.bytes[3] = ba[11];
    rxObj->word[2] = myReg.dword;

    // Assign message data
    for (i = 0; i < nBytes; i++) {
      rxd[i] = ba[i + 12];
    }
  } else {
    rxObj->word[2] = 0;

    // Assign message data
    for (i = 0; i < nBytes; i++) {
      rxd[i] = ba[i + 8];
    }
  }

  // UINC channel
  ReceiveChannelUpdate(channel);

  return 1;
}

void mcp2517fd::ReceiveChannelReset(CAN_FIFO_CHANNEL channel)
{
  REG_CiFIFOCON ciFifoCon;

  // Address and data
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
  ciFifoCon.dword = 0;
  ciFifoCon.rxBF.FRESET = 1;

  WriteByte(a, ciFifoCon.bytes[1]);
}

void mcp2517fd::ReceiveChannelUpdate(CAN_FIFO_CHANNEL channel)
{
  REG_CiFIFOCON ciFifoCon;

  ciFifoCon.dword = 0;

  // Set UINC
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET) + 1; // Byte that contains FRESET
  ciFifoCon.rxBF.UINC = 1;

  // Write byte
  WriteByte(a, ciFifoCon.bytes[1]);
}

// *****************************************************************************
// *****************************************************************************
// Section: Transmit Event FIFO
CAN_TEF_FIFO_STATUS mcp2517fd::TefStatusGet()
{
  // Read
  REG_CiTEFSTA ciTefSta;

  ciTefSta.dword = 0;

  uint16_t a = cREGADDR_CiTEFSTA;

  ciTefSta.bytes[0] = ReadByte(a);

  return (CAN_TEF_FIFO_STATUS) (ciTefSta.bytes[0] & CAN_TEF_FIFO_STATUS_MASK);
}

CAN_TEF_MSGOBJ mcp2517fd::TefMessageGet()
{
  uint16_t a = 0;
  uint32_t fifoReg[3];

  CAN_TEF_MSGOBJ tefObj;

  // Get FIFO registers
  a = cREGADDR_CiTEFCON;

  ReadDWordArray(a, fifoReg, 3);

  // Get control
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = fifoReg[0];

  // Get status
  REG_CiTEFSTA ciTefSta;
  ciTefSta.dword = fifoReg[1];

  // Get address
  REG_CiFIFOUA ciTefUa;
  ciTefUa.dword = fifoReg[2];

#ifdef USERADDRESS_TIMES_FOUR
  a = 4 * ciTefUa.bF.UserAddress;
#else
  a = ciTefUa.bF.UserAddress;
#endif
  a += cRAMADDR_START;

  // Number of bytes to read
  uint8_t n = 8; // 8 header bytes

  if (ciTefCon.bF.TimeStampEnable) {
    n += 4; // Add 4 time stamp bytes
  }

  // Read rxObj using one access
  uint8_t ba[12];

  ReadByteArray(a, ba, n);

  // Assign message header
  REG_t myReg;

  myReg.bytes[0] = ba[0];
  myReg.bytes[1] = ba[1];
  myReg.bytes[2] = ba[2];
  myReg.bytes[3] = ba[3];
  tefObj.dword[0] = myReg.dword;

  myReg.bytes[0] = ba[4];
  myReg.bytes[1] = ba[5];
  myReg.bytes[2] = ba[6];
  myReg.bytes[3] = ba[7];
  tefObj.dword[1] = myReg.dword;

  if (ciTefCon.bF.TimeStampEnable) {
    myReg.bytes[0] = ba[8];
    myReg.bytes[1] = ba[9];
    myReg.bytes[2] = ba[10];
    myReg.bytes[3] = ba[11];
    tefObj.dword[2] = myReg.dword;
  } else {
    tefObj.dword[2] = 0;
  }

  // Set UINC
  TefUpdate();

  return tefObj;
}

void mcp2517fd::TefReset()
{
  // Set FRESET
  uint16_t a = cREGADDR_CiTEFCON + 1;
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = 0;
  ciTefCon.bF.FRESET = 1;

  // Write byte
  WriteByte(a, ciTefCon.bytes[1]);
}

void mcp2517fd::TefUpdate()
{
  // Set UINC
  uint16_t a = cREGADDR_CiTEFCON + 1;
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = 0;
  ciTefCon.bF.UINC = 1;

  // Write byte
  WriteByte(a, ciTefCon.bytes[1]);
}

void mcp2517fd::TefConfigure(CAN_TEF_CONFIG* config)
{
  // Setup FIFO
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = canControlResetValues[cREGADDR_CiTEFCON / 4];

  ciTefCon.bF.FifoSize = config->FifoSize;
  ciTefCon.bF.TimeStampEnable = config->TimeStampEnable;

  WriteDWord(cREGADDR_CiTEFCON, ciTefCon.dword);
}

void mcp2517fd::TefConfigureObjectReset(CAN_TEF_CONFIG* config)
{
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = canControlResetValues[cREGADDR_CiFIFOCON / 4];

  config->FifoSize = ciTefCon.bF.FifoSize;
  config->TimeStampEnable = ciTefCon.bF.TimeStampEnable;
}

// *****************************************************************************
// *****************************************************************************
// Section: Module Events
CAN_MODULE_EVENT mcp2517fd::ModuleEventGet()
{
  // Read Interrupt flags
  REG_CiINTFLAG intFlags;
  intFlags.word = 0;

  intFlags.word = ReadWord(cREGADDR_CiINTFLAG);

  // Update data
  return (CAN_MODULE_EVENT) (intFlags.word & CAN_ALL_EVENTS);
}

void mcp2517fd::ModuleEventEnable(unsigned int flags)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiINTENABLE;
  REG_CiINTENABLE intEnables;
  intEnables.word = 0;

  intEnables.word = ReadWord(a);

  // Modify
  intEnables.word |= (flags & CAN_ALL_EVENTS);

  // Write
  WriteWord(a, intEnables.word);
}

void mcp2517fd::ModuleEventDisable(CAN_MODULE_EVENT flags)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiINTENABLE;
  REG_CiINTENABLE intEnables;
  intEnables.word = 0;

  intEnables.word = ReadWord(a);

  // Modify
  intEnables.word &= ~(flags & CAN_ALL_EVENTS);

  // Write
  WriteWord(a, intEnables.word);
}

void mcp2517fd::ModuleEventClear(CAN_MODULE_EVENT flags)
{
  // Read Interrupt flags
  uint16_t a = cREGADDR_CiINTFLAG;
  REG_CiINTFLAG intFlags;
  intFlags.word = 0;

  // Write 1 to all flags except the ones that we want to clear
  // Writing a 1 will not set the flag
  // Only writing a 0 will clear it
  // The flags are HS/C
  intFlags.word = CAN_ALL_EVENTS;
  intFlags.word &= ~flags;

  // Write
  WriteWord(a, intFlags.word);
}

CAN_RXCODE mcp2517fd::ModuleEventRxCodeGet()
{
  // Read
  uint16_t a = cREGADDR_CiVEC + 3;

  uint8_t rxCodeByte = ReadByte(a);

  // Decode data
  // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
  if ((rxCodeByte < CAN_RXCODE_TOTAL_CHANNELS) || (rxCodeByte == CAN_RXCODE_NO_INT)) {
    return (CAN_RXCODE) rxCodeByte;
  }
  else {
    return CAN_RXCODE_RESERVED; // shouldn't get here
  }
}

CAN_TXCODE mcp2517fd::ModuleEventTxCodeGet()
{
  // Read
  uint8_t a = cREGADDR_CiVEC + 2;

  uint8_t txCodeByte = ReadByte(a);

  // Decode data
  // 0x40 = "no interrupt" (CAN_FIFO_CIVEC_NOINTERRUPT)
  if ((txCodeByte < CAN_TXCODE_TOTAL_CHANNELS) || (txCodeByte == CAN_TXCODE_NO_INT)) {
    return (CAN_TXCODE) txCodeByte;
  }
  else {
    return CAN_TXCODE_RESERVED; // shouldn't get here
  }
}

CAN_FILTER mcp2517fd::ModuleEventFilterHitGet()
{
  // Read
  uint16_t a = cREGADDR_CiVEC + 1;

  uint8_t filterHitByte = ReadByte(a);

  // Update data
  return (CAN_FILTER) filterHitByte;
}

CAN_ICODE mcp2517fd::ModuleEventIcodeGet()
{
  // Read
  uint16_t a = cREGADDR_CiVEC;

  uint8_t icodeByte = ReadByte(a);

  // Decode
  if ((icodeByte < CAN_ICODE_RESERVED) && ((icodeByte < CAN_ICODE_TOTAL_CHANNELS) || (icodeByte >= CAN_ICODE_NO_INT))) {
    return (CAN_ICODE) icodeByte;
  }
  else {
    return CAN_ICODE_RESERVED; // shouldn't get here
  }
}

// *****************************************************************************
// *****************************************************************************
// Section: Transmit FIFO Events
CAN_TX_FIFO_EVENT mcp2517fd::TransmitChannelEventGet(CAN_FIFO_CHANNEL channel)
{
  // Read Interrupt flags
  REG_CiFIFOSTA ciFifoSta;
  ciFifoSta.dword = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);



  ciFifoSta.bytes[0] = ReadByte(a);

  Serial.print("TransmitChannelEventGet a and ciFifoSta.bytes[0] is ");
  Serial.print(a, HEX);
  Serial.println(ciFifoSta.bytes[0], BIN);

  return (CAN_TX_FIFO_EVENT) (ciFifoSta.bytes[0] & CAN_TX_FIFO_ALL_EVENTS);
}

uint8_t mcp2517fd::TransmitChannelIndexGet(CAN_FIFO_CHANNEL channel)
{
  // Read index
  REG_CiFIFOSTA ciFifoSta;
  ciFifoSta.dword = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

  ciFifoSta.dword = ReadDWord(a);

  return ciFifoSta.txBF.FifoIndex;
}

void mcp2517fd::TransmitChannelEventEnable(CAN_TX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = 0;

  ciFifoCon.bytes[0] = ReadByte(a);
  Serial.print("ciFifoCon.dword is ");
  Serial.println(ciFifoCon.dword, BIN);
  Serial.println(ciFifoCon.bytes[0], BIN);
  Serial.println(ciFifoCon.txBF.TxEnable, BIN);

  // Modify
  ciFifoCon.bytes[0] |= (flags & CAN_TX_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciFifoCon.bytes[0]);

  //chaoqian modify debug
  ciFifoCon.bytes[0] = ReadByte(a);
  Serial.print("TransmitChannelEventEnable  is ");
  Serial.println(ciFifoCon.bytes[0], BIN);

  REG_CiFIFOCON ciFifoCon2;
  ciFifoCon2.dword = 0;
  ciFifoCon2.bytes[0] = ReadByte(a);

  Serial.print("ciFifoCon2.dword is ");
  Serial.println(ciFifoCon2.dword, BIN);
  Serial.println(ciFifoCon2.bytes[0], BIN);
  Serial.println(ciFifoCon2.txBF.TxEnable, BIN);
}

void mcp2517fd::TransmitChannelEventDisable(CAN_TX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = 0;

  ciFifoCon.bytes[0] = ReadByte(a);

  // Modify
  ciFifoCon.bytes[0] &= ~(flags & CAN_TX_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciFifoCon.bytes[0]);
}

void mcp2517fd::TransmitChannelEventAttemptClear(CAN_FIFO_CHANNEL channel)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);
  REG_CiFIFOSTA ciFifoSta;
  ciFifoSta.dword = 0;

  ciFifoSta.bytes[0] = ReadByte(a);

  // Modify
  ciFifoSta.bytes[0] &= ~CAN_TX_FIFO_ATTEMPTS_EXHAUSTED_EVENT;

  // Write
  WriteByte(a, ciFifoSta.bytes[0]);
}

// *****************************************************************************
// *****************************************************************************
// Section: Receive FIFO Events
CAN_RX_FIFO_EVENT mcp2517fd::ReceiveChannelEventGet(CAN_FIFO_CHANNEL channel)
{

#ifdef CAN_TXQUEUE_IMPLEMENTED
  if (channel == CAN_TXQUEUE_CH0) return CAN_RX_FIFO_INVALID;
#endif

  // Read Interrupt flags
  REG_CiFIFOSTA ciFifoSta;
  ciFifoSta.dword = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

  ciFifoSta.bytes[0] = ReadByte(a);
#ifdef DEBUG_FLAG
  Serial.print("ReceiveChannelEventGet a and ciFifoSta is ");
  Serial.print(a, HEX);
  Serial.print("  ");
  Serial.println(ciFifoSta.bytes[0], BIN);
#endif
  getcurrentCiFIFOCON(CAN_FIFO_CH2);

  // Update data
  return (CAN_RX_FIFO_EVENT) (ciFifoSta.bytes[0] & CAN_RX_FIFO_ALL_EVENTS);
}

uint8_t mcp2517fd::ReceiveChannelEventEnable(CAN_RX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel)
{
#ifdef CAN_TXQUEUE_IMPLEMENTED
  if (channel == CAN_TXQUEUE_CH0) return 0;
#endif

  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = 0;

  ciFifoCon.bytes[0] = ReadByte(a);

  // Modify
  ciFifoCon.bytes[0] |= (flags & CAN_RX_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciFifoCon.bytes[0]);

  //chaoqian modify debug
  ciFifoCon.bytes[0] = ReadByte(a);
  Serial.print("ReceiveChannelEventEnable and a is ");
  Serial.print(ciFifoCon.bytes[0], BIN);
  Serial.print("  ");
  Serial.println(a, HEX);

  return 1;
}

uint8_t mcp2517fd::ReceiveChannelEventDisable(CAN_RX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel)
{

#ifdef CAN_TXQUEUE_IMPLEMENTED
  if (channel == CAN_TXQUEUE_CH0) return 0;
#endif

  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiFIFOCON + (channel * CiFIFO_OFFSET);
  REG_CiFIFOCON ciFifoCon;
  ciFifoCon.dword = 0;

  ciFifoCon.bytes[0] = ReadByte(a);

  // Modify
  ciFifoCon.bytes[0] &= ~(flags & CAN_RX_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciFifoCon.bytes[0]);

  return 1;
}

uint8_t mcp2517fd::ReceiveChannelEventOverflowClear(CAN_FIFO_CHANNEL channel)
{
#ifdef CAN_TXQUEUE_IMPLEMENTED
  if (channel == CAN_TXQUEUE_CH0) return 0;
#endif

  // Read Interrupt Flags
  REG_CiFIFOSTA ciFifoSta;
  ciFifoSta.dword = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);

  ciFifoSta.bytes[0] = ReadByte(a);

  // Modify
  ciFifoSta.bytes[0] &= ~(CAN_RX_FIFO_OVERFLOW_EVENT);

  // Write
  WriteByte(a, ciFifoSta.bytes[0]);

  return 1;
}

// *****************************************************************************
// *****************************************************************************
// Section: Transmit Event FIFO Events
CAN_TEF_FIFO_EVENT mcp2517fd::TefEventGet()
{
  // Read Interrupt flags
  REG_CiTEFSTA ciTefSta;
  ciTefSta.dword = 0;
  uint16_t a = cREGADDR_CiTEFSTA;

  ciTefSta.bytes[0] = ReadByte(a);

  // Update data
  return (CAN_TEF_FIFO_EVENT) (ciTefSta.bytes[0] & CAN_TEF_FIFO_ALL_EVENTS);
}

void mcp2517fd::TefEventEnable(CAN_TEF_FIFO_EVENT flags)
{

  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiTEFCON;
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = 0;

  ciTefCon.bytes[0] = ReadByte(a);

  // Modify
  ciTefCon.bytes[0] |= (flags & CAN_TEF_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciTefCon.bytes[0]);
}

void mcp2517fd::TefEventDisable(CAN_TEF_FIFO_EVENT flags)
{
  // Read Interrupt Enables
  uint16_t a = cREGADDR_CiTEFCON;
  REG_CiTEFCON ciTefCon;
  ciTefCon.dword = 0;

  ciTefCon.bytes[0] = ReadByte(a);

  // Modify
  ciTefCon.bytes[0] &= ~(flags & CAN_TEF_FIFO_ALL_EVENTS);

  // Write
  WriteByte(a, ciTefCon.bytes[0]);
}

void mcp2517fd::TefEventOverflowClear()
{
  // Read Interrupt Flags
  REG_CiTEFSTA ciTefSta;
  ciTefSta.dword = 0;
  uint16_t a = cREGADDR_CiTEFSTA;

  ciTefSta.bytes[0] = ReadByte(a);

  // Modify
  ciTefSta.bytes[0] &= ~(CAN_TEF_FIFO_OVERFLOW_EVENT);

  // Write
  WriteByte(a, ciTefSta.bytes[0]);
}

// *****************************************************************************
// *****************************************************************************
// Section: Error Handling
uint8_t mcp2517fd::ErrorCountTransmitGet()
{
  // Read Error count
  uint16_t a = cREGADDR_CiTREC + 1;

  return ReadByte(a);
}

uint8_t mcp2517fd::ErrorCountReceiveGet()
{
  // Read Error count
  uint16_t a = cREGADDR_CiTREC;

  return ReadByte(a);
}

CAN_ERROR_STATE mcp2517fd::ErrorStateGet()
{
  // Read Error state
  uint16_t a = cREGADDR_CiTREC + 2;
  uint8_t f = 0;

  f = ReadByte(a);

  // Update data
  return (CAN_ERROR_STATE) (f & CAN_ERROR_ALL);
}

void mcp2517fd::ErrorCountStateGet(uint8_t* tx_err_cnt, uint8_t* rx_err_cnt, CAN_ERROR_STATE* flags)
{
  // Read Error
  uint16_t a = cREGADDR_CiTREC;
  REG_CiTREC ciTrec;
  ciTrec.dword = 0;

  ciTrec.dword = ReadDWord(a);

  // Update data
  *tx_err_cnt = ciTrec.bytes[1];
  *rx_err_cnt = ciTrec.bytes[0];
  *flags = (CAN_ERROR_STATE) (ciTrec.bytes[2] & CAN_ERROR_ALL);
}

CAN_BUS_DIAGNOSTIC mcp2517fd::BusDiagnosticsGet()
{
  // Read diagnostic registers all in one shot
  uint16_t a = cREGADDR_CiBDIAG0;
  uint32_t w[2];

  ReadDWordArray(a, w, 2);

  // Update data
  CAN_BUS_DIAGNOSTIC b;
  b.dword[0] = w[0];
  b.dword[1] = w[1] & 0x0000ffff;
  b.dword[2] = (w[1] >> 16) & 0x0000ffff;

  return b;
}

void mcp2517fd::BusDiagnosticsClear()
{
  // Clear diagnostic registers all in one shot
  uint16_t a = cREGADDR_CiBDIAG0;
  uint32_t w[2];
  w[0] = 0;
  w[1] = 0;

  WriteDWordArray(a, w, 2);
}

// *****************************************************************************
// *****************************************************************************
// Section: ECC
void mcp2517fd::EccEnable()
{
  // Read
  uint8_t d = ReadByte(cREGADDR_ECCCON);

  // Modify
  d |= 0x01;

  // Write
  WriteByte(cREGADDR_ECCCON, d);

#if DEBUG_FLAG
  //chaoqian debug
  d = ReadByte(cREGADDR_ECCCON);
  Serial.println(d, HEX);
#endif
}

void mcp2517fd::EccDisable()
{
  // Read
  uint8_t d = ReadByte(cREGADDR_ECCCON);

  // Modify
  d &= ~0x01;

  // Write
  WriteByte(cREGADDR_ECCCON, d);
}

CAN_ECC_EVENT mcp2517fd::EccEventGet()
{
  // Read Interrupt flags
  uint8_t eccStatus = 0;
  uint16_t a = cREGADDR_ECCSTA;

  eccStatus = ReadByte(a);

  // Update data
  return (CAN_ECC_EVENT) (eccStatus & CAN_ECC_ALL_EVENTS);
}

uint16_t mcp2517fd::EccErrorAddressGet()
{
  REG_ECCSTA reg;

  // Read
  reg.dword = ReadDWord(cREGADDR_ECCSTA);

  // Update data
  return reg.bF.ErrorAddress;
}

void mcp2517fd::EccEventEnable(CAN_ECC_EVENT flags)
{
  // Read
  uint16_t a = cREGADDR_ECCCON;
  uint8_t eccInterrupts = 0;

  eccInterrupts = ReadByte(a);

  // Modify
  eccInterrupts |= (flags & CAN_ECC_ALL_EVENTS);

  // Write
  WriteByte(a, eccInterrupts);
}

void mcp2517fd::EccEventDisable(CAN_ECC_EVENT flags)
{
  // Read
  uint16_t a = cREGADDR_ECCCON;
  uint8_t eccInterrupts = 0;

  eccInterrupts = ReadByte(a);

  // Modify
  eccInterrupts &= ~(flags & CAN_ECC_ALL_EVENTS);

  // Write
  WriteByte(a, eccInterrupts);
}

void mcp2517fd::EccEventClear(CAN_ECC_EVENT flags)
{
  // Read
  uint16_t a = cREGADDR_ECCSTA;
  uint8_t eccStat = 0;

  eccStat = ReadByte(a);

  // Modify
  eccStat &= ~(flags & CAN_ECC_ALL_EVENTS);

  // Write
  WriteByte(a, eccStat);
}

void mcp2517fd::RamInit(uint8_t d)
{
  uint8_t txd[SPI_DEFAULT_BUFFER_LENGTH];
  uint32_t k;

  // Prepare data
  for (k = 0; k < SPI_DEFAULT_BUFFER_LENGTH; k++) {
    txd[k] = d;
  }

  uint16_t a = cRAMADDR_START;

  for (k = 0; k < (cRAM_SIZE / SPI_DEFAULT_BUFFER_LENGTH); k++) {
    WriteByteArray(a, txd, SPI_DEFAULT_BUFFER_LENGTH);
    a += SPI_DEFAULT_BUFFER_LENGTH;
  }
}

// *****************************************************************************
// *****************************************************************************
// Section: CRC
void mcp2517fd::CrcEventEnable(CAN_CRC_EVENT flags)
{
  // Read interrupt control bits of CRC Register
  uint16_t a = cREGADDR_CRC + 3;
  uint8_t crc;

  crc = ReadByte(a);

  // Modify
  crc |= (flags & CAN_CRC_ALL_EVENTS);

  // Write
  WriteByte(a, crc);
}

void mcp2517fd::CrcEventDisable(CAN_CRC_EVENT flags)
{
  // Read interrupt control bits of CRC Register
  uint16_t a = cREGADDR_CRC + 3;
  uint8_t crc;

  crc = ReadByte(a);

  // Modify
  crc &= ~(flags & CAN_CRC_ALL_EVENTS);

  // Write
  WriteByte(a, crc);
}

void mcp2517fd::CrcEventClear(CAN_CRC_EVENT flags)
{
  // Read interrupt flags of CRC Register
  uint16_t a = cREGADDR_CRC + 2;
  uint8_t crc;

  crc = ReadByte(a);

  // Modify
  crc &= ~(flags & CAN_CRC_ALL_EVENTS);

  // Write
  WriteByte(a, crc);
}

CAN_CRC_EVENT mcp2517fd::CrcEventGet()
{
  // Read interrupt flags of CRC Register
  uint16_t a = cREGADDR_CRC + 2;
  uint8_t crc;

  crc = ReadByte(a);

  // Update data
  return (CAN_CRC_EVENT) (crc & CAN_CRC_ALL_EVENTS);
}

// *****************************************************************************
// *****************************************************************************
// Section: Time Stamp
void mcp2517fd::TimeStampEnable()
{
  // Read
  uint8_t d = ReadByte(cREGADDR_CiTSCON + 2);

  // Modify
  d |= 0x01;

  // Write
  WriteByte(cREGADDR_CiTSCON + 2, d);
}

void mcp2517fd::TimeStampDisable()
{
  // Read
  uint8_t d = ReadByte(cREGADDR_CiTSCON + 2);

  // Modify
  d &= 0x06;

  // Write
  WriteByte(cREGADDR_CiTSCON + 2, d);
}

void mcp2517fd::TimeStampModeConfigure(CAN_TS_MODE mode)
{
  // Read
  uint8_t d = ReadByte(cREGADDR_CiTSCON + 2);


  // Modify
  d &= 0x01;
  d |= mode << 1;

  // Write
  WriteByte(cREGADDR_CiTSCON + 2, d);
}

// *****************************************************************************
// *****************************************************************************
// Section: Oscillator and Bit Time
void mcp2517fd::OscillatorEnable()
{
  // Read
  uint8_t d = ReadByte(cREGADDR_OSC);

  // Modify
  d &= ~0x4;

  // Write
  WriteByte(cREGADDR_OSC, d);
}

void mcp2517fd::OscillatorControlSet(CAN_OSC_CTRL ctrl)
{
  REG_OSC osc;
  osc.dword = 0;

  osc.bF.PllEnable = ctrl.PllEnable;
  osc.bF.OscDisable = ctrl.OscDisable;
  osc.bF.SCLKDIV = ctrl.SclkDivide;
  osc.bF.CLKODIV = ctrl.ClkOutDivide;

  // Write
  WriteByte(cREGADDR_OSC, osc.bytes[0]);
}

void mcp2517fd::OscillatorControlObjectReset(CAN_OSC_CTRL* ctrl)
{
  REG_OSC osc;
  osc.dword = mcp2517ControlResetValues[0];

  ctrl->PllEnable = osc.bF.PllEnable;
  ctrl->OscDisable = osc.bF.OscDisable;
  ctrl->SclkDivide = osc.bF.SCLKDIV;
  ctrl->ClkOutDivide = osc.bF.CLKODIV;

}

CAN_OSC_STATUS mcp2517fd::OscillatorStatusGet()
{

  REG_OSC osc;
  osc.dword = 0;
  CAN_OSC_STATUS stat;

  // Read
  osc.bytes[1] = ReadByte(cREGADDR_OSC + 1);

  stat.PllReady = osc.bF.PllReady;
  stat.OscReady = osc.bF.OscReady;
  stat.SclkReady = osc.bF.SclkReady;

  return stat;
}

uint8_t mcp2517fd::BitTimeConfigure(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode, CAN_SYSCLK_SPEED clk)
{
  // Decode clk
  switch (clk) {
    case CAN_SYSCLK_40M:
      if (BitTimeConfigureNominal40MHz(bitTime) == 0) return 0;
      return BitTimeConfigureData40MHz(bitTime, sspMode);
      break;
    case CAN_SYSCLK_20M:
      Serial.println("BitTimeConfigure in 20M");
      if (BitTimeConfigureNominal20MHz(bitTime) == 0) return 0;
      return BitTimeConfigureData20MHz(bitTime, sspMode);
      break;
    case CAN_SYSCLK_10M:
      if (BitTimeConfigureNominal10MHz(bitTime) == 0) return 0;
      return BitTimeConfigureData10MHz(bitTime, sspMode);
      break;
    default:
      return 0;
  }
}

uint8_t mcp2517fd::BitTimeConfigureNominal40MHz(CAN_BITTIME_SETUP bitTime)
{
  REG_CiNBTCFG ciNbtcfg;

  ciNbtcfg.dword = canControlResetValues[cREGADDR_CiNBTCFG / 4];

  // Arbitration Bit rate
  switch (bitTime) {
    // All 500K
    case CAN_500K_1M:
    case CAN_500K_2M:
    case CAN_500K_3M:
    case CAN_500K_4M:
    case CAN_500K_5M:
    case CAN_500K_6M7:
    case CAN_500K_8M:
    case CAN_500K_10M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 62;
      ciNbtcfg.bF.TSEG2 = 15;
      ciNbtcfg.bF.SJW = 15;
      break;

    // All 250K
    case CAN_250K_500K:
    case CAN_250K_833K:
    case CAN_250K_1M:
    case CAN_250K_1M5:
    case CAN_250K_2M:
    case CAN_250K_3M:
    case CAN_250K_4M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 126;
      ciNbtcfg.bF.TSEG2 = 31;
      ciNbtcfg.bF.SJW = 31;
      break;

    case CAN_1000K_4M:
    case CAN_1000K_8M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 30;
      ciNbtcfg.bF.TSEG2 = 7;
      ciNbtcfg.bF.SJW = 7;
      break;

    case CAN_125K_500K:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 254;
      ciNbtcfg.bF.TSEG2 = 63;
      ciNbtcfg.bF.SJW = 63;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiNBTCFG, ciNbtcfg.dword);

  return 1;
}

uint8_t mcp2517fd::BitTimeConfigureData40MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
  REG_CiDBTCFG ciDbtcfg;
  REG_CiTDC ciTdc;
  //    sspMode;

  ciDbtcfg.dword = canControlResetValues[cREGADDR_CiDBTCFG / 4];
  ciTdc.dword = 0;

  // Configure Bit time and sample point
  ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
  uint32_t tdcValue = 0;

  // Data Bit rate and SSP
  switch (bitTime) {
    case CAN_500K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 30;
      ciDbtcfg.bF.TSEG2 = 7;
      ciDbtcfg.bF.SJW = 7;
      // SSP
      ciTdc.bF.TDCOffset = 31;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_2M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 14;
      ciDbtcfg.bF.TSEG2 = 3;
      ciDbtcfg.bF.SJW = 3;
      // SSP
      ciTdc.bF.TDCOffset = 15;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_3M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 8;
      ciDbtcfg.bF.TSEG2 = 2;
      ciDbtcfg.bF.SJW = 2;
      // SSP
      ciTdc.bF.TDCOffset = 9;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_4M:
    case CAN_1000K_4M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_5M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 4;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 5;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_6M7:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 3;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 4;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_8M:
    case CAN_1000K_8M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 2;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 3;
      ciTdc.bF.TDCValue = 1;
      break;
    case CAN_500K_10M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 1;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 2;
      ciTdc.bF.TDCValue = 0;
      break;

    case CAN_250K_500K:
    case CAN_125K_500K:
      ciDbtcfg.bF.BRP = 1;
      ciDbtcfg.bF.TSEG1 = 30;
      ciDbtcfg.bF.TSEG2 = 7;
      ciDbtcfg.bF.SJW = 7;
      // SSP
      ciTdc.bF.TDCOffset = 31;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_833K:
      ciDbtcfg.bF.BRP = 1;
      ciDbtcfg.bF.TSEG1 = 17;
      ciDbtcfg.bF.TSEG2 = 4;
      ciDbtcfg.bF.SJW = 4;
      // SSP
      ciTdc.bF.TDCOffset = 18;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 30;
      ciDbtcfg.bF.TSEG2 = 7;
      ciDbtcfg.bF.SJW = 7;
      // SSP
      ciTdc.bF.TDCOffset = 31;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_1M5:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 18;
      ciDbtcfg.bF.TSEG2 = 5;
      ciDbtcfg.bF.SJW = 5;
      // SSP
      ciTdc.bF.TDCOffset = 19;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_2M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 14;
      ciDbtcfg.bF.TSEG2 = 3;
      ciDbtcfg.bF.SJW = 3;
      // SSP
      ciTdc.bF.TDCOffset = 15;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_3M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 8;
      ciDbtcfg.bF.TSEG2 = 2;
      ciDbtcfg.bF.SJW = 2;
      // SSP
      ciTdc.bF.TDCOffset = 9;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_4M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiDBTCFG, ciDbtcfg.dword);

  // Write Transmitter Delay Compensation
#ifdef REV_A
  ciTdc.bF.TDCOffset = 0;
  ciTdc.bF.TDCValue = 0;
#endif

  WriteDWord(cREGADDR_CiTDC, ciTdc.dword);

  return 1;
}

uint8_t mcp2517fd::BitTimeConfigureNominal20MHz(CAN_BITTIME_SETUP bitTime)
{
  REG_CiNBTCFG ciNbtcfg;

  ciNbtcfg.dword = canControlResetValues[cREGADDR_CiNBTCFG / 4];

  // Arbitration Bit rate
  switch (bitTime) {
    // All 500K
    case CAN_500K_1M:
    case CAN_500K_2M:
    case CAN_500K_4M:
    case CAN_500K_5M:
    case CAN_500K_6M7:
    case CAN_500K_8M:
    case CAN_500K_10M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 30;
      ciNbtcfg.bF.TSEG2 = 7;
      ciNbtcfg.bF.SJW = 7;
      break;

    // All 250K
    case CAN_250K_500K:
    case CAN_250K_833K:
    case CAN_250K_1M:
    case CAN_250K_1M5:
    case CAN_250K_2M:
    case CAN_250K_3M:
    case CAN_250K_4M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 62;
      ciNbtcfg.bF.TSEG2 = 15;
      ciNbtcfg.bF.SJW = 15;
      break;

    case CAN_1000K_4M:
    case CAN_1000K_8M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 14;
      ciNbtcfg.bF.TSEG2 = 3;
      ciNbtcfg.bF.SJW = 3;
      break;

    case CAN_125K_500K:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 126;
      ciNbtcfg.bF.TSEG2 = 31;
      ciNbtcfg.bF.SJW = 31;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiNBTCFG, ciNbtcfg.dword);

  return 1;
}

uint8_t mcp2517fd::BitTimeConfigureData20MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
  REG_CiDBTCFG ciDbtcfg;
  REG_CiTDC ciTdc;
  //    sspMode;

  ciDbtcfg.dword = canControlResetValues[cREGADDR_CiDBTCFG / 4];
  ciTdc.dword = 0;

  // Configure Bit time and sample point
  ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
  uint32_t tdcValue = 0;

  // Data Bit rate and SSP
  switch (bitTime) {
    case CAN_500K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 14;
      ciDbtcfg.bF.TSEG2 = 3;
      ciDbtcfg.bF.SJW = 3;
      // SSP
      ciTdc.bF.TDCOffset = 15;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_2M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_4M:
    case CAN_1000K_4M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 2;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 3;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_5M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 1;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 2;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_6M7:
    case CAN_500K_8M:
    case CAN_500K_10M:
    case CAN_1000K_8M:
      //qDebug("Data Bitrate not feasible with this clock!");
      return 0;
      break;

    case CAN_250K_500K:
    case CAN_125K_500K:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 30;
      ciDbtcfg.bF.TSEG2 = 7;
      ciDbtcfg.bF.SJW = 7;
      // SSP
      ciTdc.bF.TDCOffset = 31;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_833K:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 17;
      ciDbtcfg.bF.TSEG2 = 4;
      ciDbtcfg.bF.SJW = 4;
      // SSP
      ciTdc.bF.TDCOffset = 18;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 14;
      ciDbtcfg.bF.TSEG2 = 3;
      ciDbtcfg.bF.SJW = 3;
      // SSP
      ciTdc.bF.TDCOffset = 15;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_1M5:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 8;
      ciDbtcfg.bF.TSEG2 = 2;
      ciDbtcfg.bF.SJW = 2;
      // SSP
      ciTdc.bF.TDCOffset = 9;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_2M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_3M:
      //qDebug("Data Bitrate not feasible with this clock!");
      return 0;
      break;
    case CAN_250K_4M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 2;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 3;
      ciTdc.bF.TDCValue = tdcValue;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiDBTCFG, ciDbtcfg.dword);

  // Write Transmitter Delay Compensation
#ifdef REV_A
  ciTdc.bF.TDCOffset = 0;
  ciTdc.bF.TDCValue = 0;
#endif

  WriteDWord(cREGADDR_CiTDC, ciTdc.dword);

  return 1;
}

uint8_t mcp2517fd::BitTimeConfigureNominal10MHz(CAN_BITTIME_SETUP bitTime)
{
  REG_CiNBTCFG ciNbtcfg;

  ciNbtcfg.dword = canControlResetValues[cREGADDR_CiNBTCFG / 4];

  // Arbitration Bit rate
  switch (bitTime) {
    // All 500K
    case CAN_500K_1M:
    case CAN_500K_2M:
    case CAN_500K_4M:
    case CAN_500K_5M:
    case CAN_500K_6M7:
    case CAN_500K_8M:
    case CAN_500K_10M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 14;
      ciNbtcfg.bF.TSEG2 = 3;
      ciNbtcfg.bF.SJW = 3;
      break;

    // All 250K
    case CAN_250K_500K:
    case CAN_250K_833K:
    case CAN_250K_1M:
    case CAN_250K_1M5:
    case CAN_250K_2M:
    case CAN_250K_3M:
    case CAN_250K_4M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 30;
      ciNbtcfg.bF.TSEG2 = 7;
      ciNbtcfg.bF.SJW = 7;
      break;

    case CAN_1000K_4M:
    case CAN_1000K_8M:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 7;
      ciNbtcfg.bF.TSEG2 = 2;
      ciNbtcfg.bF.SJW = 2;
      break;

    case CAN_125K_500K:
      ciNbtcfg.bF.BRP = 0;
      ciNbtcfg.bF.TSEG1 = 62;
      ciNbtcfg.bF.TSEG2 = 15;
      ciNbtcfg.bF.SJW = 15;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiNBTCFG, ciNbtcfg.dword);

  return 1;
}

uint8_t mcp2517fd::BitTimeConfigureData10MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode)
{
  REG_CiDBTCFG ciDbtcfg;
  REG_CiTDC ciTdc;
  //    sspMode;

  ciDbtcfg.dword = canControlResetValues[cREGADDR_CiDBTCFG / 4];
  ciTdc.dword = 0;

  // Configure Bit time and sample point
  ciTdc.bF.TDCMode = CAN_SSP_MODE_AUTO;
  uint32_t tdcValue = 0;

  // Data Bit rate and SSP
  switch (bitTime) {
    case CAN_500K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_2M:
      // Data BR
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 2;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 3;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_500K_4M:
    case CAN_500K_5M:
    case CAN_500K_6M7:
    case CAN_500K_8M:
    case CAN_500K_10M:
    case CAN_1000K_4M:
    case CAN_1000K_8M:
      //qDebug("Data Bitrate not feasible with this clock!");
      return 0;
      break;

    case CAN_250K_500K:
    case CAN_125K_500K:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 14;
      ciDbtcfg.bF.TSEG2 = 3;
      ciDbtcfg.bF.SJW = 3;
      // SSP
      ciTdc.bF.TDCOffset = 15;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_833K:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 7;
      ciDbtcfg.bF.TSEG2 = 2;
      ciDbtcfg.bF.SJW = 2;
      // SSP
      ciTdc.bF.TDCOffset = 8;
      ciTdc.bF.TDCValue = tdcValue;
      ciTdc.bF.TDCMode = CAN_SSP_MODE_OFF;
      break;
    case CAN_250K_1M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 6;
      ciDbtcfg.bF.TSEG2 = 1;
      ciDbtcfg.bF.SJW = 1;
      // SSP
      ciTdc.bF.TDCOffset = 7;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_1M5:
      //qDebug("Data Bitrate not feasible with this clock!");
      return 0;
      break;
    case CAN_250K_2M:
      ciDbtcfg.bF.BRP = 0;
      ciDbtcfg.bF.TSEG1 = 2;
      ciDbtcfg.bF.TSEG2 = 0;
      ciDbtcfg.bF.SJW = 0;
      // SSP
      ciTdc.bF.TDCOffset = 3;
      ciTdc.bF.TDCValue = tdcValue;
      break;
    case CAN_250K_3M:
    case CAN_250K_4M:
      //qDebug("Data Bitrate not feasible with this clock!");
      return 0;
      break;

    default:
      return 0;
      break;
  }

  // Write Bit time registers
  WriteDWord(cREGADDR_CiDBTCFG, ciDbtcfg.dword);

  // Write Transmitter Delay Compensation
#ifdef REV_A
  ciTdc.bF.TDCOffset = 0;
  ciTdc.bF.TDCValue = 0;
#endif

  WriteDWord(cREGADDR_CiTDC, ciTdc.dword);

  return 1;
}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO
void mcp2517fd::GpioModeConfigure(GPIO_PIN_MODE gpio0, GPIO_PIN_MODE gpio1)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 3;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[3] = ReadByte(a);

  // Modify
  iocon.bF.PinMode0 = gpio0;
  iocon.bF.PinMode1 = gpio1;

  // Write
  WriteByte(a, iocon.bytes[3]);
}

void mcp2517fd::GpioDirectionConfigure(GPIO_PIN_DIRECTION gpio0, GPIO_PIN_DIRECTION gpio1)
{
  // Read
  uint16_t a = cREGADDR_IOCON;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[0] = ReadByte(a);

  // Modify
  iocon.bF.TRIS0 = gpio0;
  iocon.bF.TRIS1 = gpio1;

  // Write
  WriteByte(a, iocon.bytes[0]);
}

void mcp2517fd::GpioStandbyControlEnable()
{
  // Read
  uint16_t a = cREGADDR_IOCON;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[0] = ReadByte(a);

  // Modify
  iocon.bF.XcrSTBYEnable = 1;

  // Write
  WriteByte( a, iocon.bytes[0]);
}

void mcp2517fd::GpioStandbyControlDisable()
{
  // Read
  uint16_t a = cREGADDR_IOCON;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[0] = ReadByte(a);

  // Modify
  iocon.bF.XcrSTBYEnable = 0;

  // Write
  WriteByte(a, iocon.bytes[0]);
}

void mcp2517fd::GpioInterruptPinsOpenDrainConfigure(GPIO_OPEN_DRAIN_MODE mode)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 3;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[3] = ReadByte(a);

  // Modify
  iocon.bF.INTPinOpenDrain = mode;

  // Write
  WriteByte(a, iocon.bytes[3]);
}

void mcp2517fd::GpioTransmitPinOpenDrainConfigure(GPIO_OPEN_DRAIN_MODE mode)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 3;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[3] = ReadByte(a);

  // Modify
  iocon.bF.TXCANOpenDrain = mode;

  // Write
  WriteByte(a, iocon.bytes[3]);
}

uint8_t mcp2517fd::GpioPinSet(GPIO_PIN_POS pos, GPIO_PIN_STATE latch)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 1;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[1] = ReadByte(a);

  // Modify
  switch (pos) {
    case GPIO_PIN_0:
      iocon.bF.LAT0 = latch;
      break;
    case GPIO_PIN_1:
      iocon.bF.LAT1 = latch;
      break;
    default:
      return 0;
      break;
  }

  // Write
  WriteByte(a, iocon.bytes[1]);

  return 1;
}

GPIO_PIN_STATE mcp2517fd::GpioPinRead(GPIO_PIN_POS pos)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 2;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[2] = ReadByte(a);

  // Update data
  switch (pos) {
    case GPIO_PIN_0:
      return (GPIO_PIN_STATE) iocon.bF.GPIO0;
      break;
    case GPIO_PIN_1:
      return (GPIO_PIN_STATE) iocon.bF.GPIO1;
      break;
    default:
      return (GPIO_PIN_STATE) 2;
      break;
  }
}

void mcp2517fd::GpioClockOutputConfigure(GPIO_CLKO_MODE mode)
{
  // Read
  uint16_t a = cREGADDR_IOCON + 3;
  REG_IOCON iocon;
  iocon.dword = 0;

  iocon.bytes[3] = ReadByte(a);

  // Modify
  iocon.bF.SOFOutputEnable = mode;

  // Write
  WriteByte(a, iocon.bytes[3]);
}

// *****************************************************************************
// *****************************************************************************
// Section: Miscellaneous

int8_t mcp2517fd::DataLengthtoDLC(uint8_t datalength, uint8_t l, uint8_t r) 
{
  if (r >= l) {
    int dlc = l + (r - l) / 2;

    // If the element is present at the middle itself
    if (DLC_DataLength[dlc] == datalength)
      return dlc;

    // If element is smaller than mid, then it can only be present in left subarray
    if (DLC_DataLength[dlc] > datalength) return DataLengthtoDLC(datalength, l, dlc - 1);

    // Else the element can only be present in right subarray
    return DataLengthtoDLC(datalength, dlc + 1, r);
  }

  // We reach here when element is not present in array
  return -1;
}

uint8_t mcp2517fd::FifoIndexGet(CAN_FIFO_CHANNEL channel)
{
  // Read Status register
  uint8_t b = 0;
  uint16_t a = cREGADDR_CiFIFOSTA + (channel * CiFIFO_OFFSET);
  a += 1; // byte[1]

  b = ReadByte(a);

  // Update data
  return b & 0x1f;
}

uint16_t mcp2517fd::CalculateCRC16(uint8_t* data, uint16_t size)
{
  uint16_t init = CRCBASE;
  uint8_t index;

  while (size-- != 0) {
    index = ((uint8_t*) & init)[CRCUPPER] ^ *data++;
    init = (init << 8) ^ crc16_table[index];
  }

  return init;
}

void mcp2517fd::Init(CAN_BITTIME_SETUP selectedBitTime, CAN_FIFO_CHANNEL tx_fifo_ch, CAN_FIFO_CHANNEL rx_fifo_ch )
{
  Serial.println("init start");

  CAN_CONFIG config;
  CAN_TX_FIFO_CONFIG txConfig;
  CAN_RX_FIFO_CONFIG rxConfig;
  REG_CiFLTOBJ fObj;
  REG_CiMASK mObj;

  DRV_SPI_Initialize();
  ReceiveChannelEventGet(); //chaoqian tmp

  // Reset device
  Reset();
  Serial.println("init reset ok");

  // Enable ECC and initialize RAM
  EccEnable();
  RamInit(0xff);
  Serial.println("init RamInit ok");

  // Configure device
  ConfigureObjectReset(&config);
  config.IsoCrcEnable = ISO_CRC;
  config.StoreInTEF = 0;

  Configure(&config);
  Serial.println("init Configure device ok");

  // Setup TX FIFO
  TransmitChannelConfigureObjectReset(&txConfig);
  txConfig.FifoSize = 7;
  txConfig.PayLoadSize = CAN_PLSIZE_64;
  txConfig.TxPriority = 1;

  TransmitChannelConfigure(&txConfig, tx_fifo_ch);
  Serial.println("init Setup TX FIFO ok");

  // Setup RX FIFO
  ReceiveChannelConfigureObjectReset(&rxConfig);
  rxConfig.FifoSize = 15;
  rxConfig.PayLoadSize = CAN_PLSIZE_64;

  ReceiveChannelConfigure(&rxConfig, CAN_FIFO_CH2);
  Serial.println("init Setup RX FIFO ok");
  ReceiveChannelEventGet(); //chaoqian tmp

  //!!!enable ALL FILTERS!!!//
  // Setup RX Filter
  fObj.dword = 0;
  fObj.bF.SID = 0x336;
  fObj.bF.EXIDE = 0;
  fObj.bF.EID = 0x00;
  FilterObjectConfigure(CAN_FILTER0, &fObj.bF);

  // Setup RX Mask
  mObj.dword = 0;
  mObj.bF.MSID = 0x0;
  mObj.bF.MIDE = 0; // Only allow standard IDs
  mObj.bF.MEID = 0x0;
  FilterMaskConfigure(CAN_FILTER0, &mObj.bF);
  
  // Link FIFO and Filter
  FilterToFifoLink(CAN_FILTER0, true, CAN_FIFO_CH2);

  // Setup Bit Time
  //BitTimeConfigure((CAN_BITTIME_SETUP) selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_40M);// chaoqian modify
  BitTimeConfigure((CAN_BITTIME_SETUP) selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_20M);
  //   DRV_CANFDSPI_BitTimeConfigure(DRV_CANFDSPI_INDEX_0, selectedBitTime, CAN_SSP_MODE_AUTO, CAN_SYSCLK_20M);
  Serial.println("init Setup Bit Time ok");

  // Setup Transmit and Receive Interrupts
  GpioModeConfigure(GPIO_MODE_INT, GPIO_MODE_INT);
  TransmitChannelEventEnable(CAN_TX_FIFO_NOT_FULL_EVENT, tx_fifo_ch);
  ReceiveChannelEventEnable(CAN_RX_FIFO_NOT_EMPTY_EVENT, rx_fifo_ch);
  ReceiveChannelEventGet(); //chaoqian tmp

  ModuleEventEnable(CAN_TX_EVENT | CAN_RX_EVENT);
  Serial.println("init Setup Transmit and Receive Interrupts ok");

  // Select Normal Mode  LOOPBACKMODE
  OperationModeSelect(CAN_NORMAL_MODE);
  Serial.println("init Select Normal Mode ok");
  ReceiveChannelEventGet(); //chaoqian tmp

  Serial.println("init end");
}
