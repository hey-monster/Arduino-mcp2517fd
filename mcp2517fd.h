/*
  mcp2517FD.h - Library for mcp2517FD Arduino shield
  created by Sherzaad Dinah

  Revision History
  ver1.0 - Newly created
*/
#ifndef	MCP2517FD_H
#define	MCP2517FD

#include "Arduino.h"
#include "drv_canfdspi_defines.h"
#include "drv_canfdspi_register.h"
#include "SPI.h"

//SPISettings settingsA(20000000, MSBFIRST, SPI_MODE0);

#define MAX_TXQUEUE_ATTEMPTS 50

#define DEBUG_FLAG 0


#ifdef ARDUINO_ARCH_AVR
  #define REGTYPE uint8_t   // AVR uses 8-bit registers
#else
  #define REGTYPE uint32_t
#endif

class mcp2517fd {
  public:
    // *****************************************************************************
    //! Reset DUT
    void DRV_SPI_Initialize(); //chaoqian modify

    void getcurrentFIFOSTA(CAN_FIFO_CHANNEL channel);

    void getcurrentCiFIFOCON(CAN_FIFO_CHANNEL channel);

    void getcurrentTXREQandUINC(CAN_FIFO_CHANNEL channel);
        
    void getcurrentTxSta();
    
    int8_t DRV_CANFDSPI_ReadWordArray(uint16_t address, uint32_t *rxd, uint16_t nWords);

    int8_t DRV_SPI_TransferData(uint8_t *SpiTxData, uint8_t *SpiRxData, uint16_t spiTransferSize);

    void Reset();

    // *****************************************************************************
    // *****************************************************************************
    // Section: chaoqian modify
    void APP_TransmitMessageQueue();
    void APP_ReceiveMessage_Tasks();

    // *****************************************************************************
    // *****************************************************************************
    // Section: SPI Access Functions

    // *****************************************************************************
    //! SPI Read Byte

    uint8_t ReadByte(uint16_t address);

    // *****************************************************************************
    //! SPI Write Byte

    void WriteByte(uint16_t address, uint8_t txd);

    // *****************************************************************************
    //! SPI Read Word

    uint16_t ReadWord(uint16_t address);

    // *****************************************************************************
    //! SPI Write Word

    void WriteWord(uint16_t address, uint16_t txd);

    /// *****************************************************************************
    //! SPI Read Word

    uint32_t ReadDWord(uint16_t address);

    // *****************************************************************************
    //! SPI Write DWord

    void WriteDWord(uint16_t address, uint32_t txd);

    // *****************************************************************************
    //! SPI Read Byte Array

    void ReadByteArray(uint16_t address, uint8_t *rxd, uint16_t nBytes);

    // *****************************************************************************
    //! SPI Write Byte Array

    void WriteByteArray(uint16_t address, uint8_t *txd, uint16_t nBytes);

    // *****************************************************************************
    //! SPI SFR Write Byte Safe
    /*!
       Writes Byte to SFR at address using SPI CRC. Byte gets only written if CRC matches.

       Remark: The function doesn't check if the address is an SFR address.
    */

    void WriteByteSafe(uint16_t address, uint8_t txd);

    // *****************************************************************************
    //! SPI RAM Write Word Safe
    /*!
       Writes Word to RAM at address using SPI CRC. Word gets only written if CRC matches.

       Remark: The function doesn't check if the address is a RAM address.
    */

    void WriteDWordSafe(uint16_t address, uint32_t txd);

    // *****************************************************************************
    //! SPI Read Byte Array with CRC. nBytes is array size WITHOUT CRC bytes. returns 1 if read CRC is OK

    uint8_t ReadByteArrayWithCRC(uint16_t address, uint8_t *rxd, uint16_t nBytes, bool fromRam = false);

    // *****************************************************************************
    //! SPI Write Byte Array with CRC. nBytes is array size WITHOUT CRC bytes. CRC calculated within routine

    void WriteByteArrayWithCRC(uint16_t address, uint8_t *txd, uint16_t nBytes, bool fromRam = false);

    // *****************************************************************************
    //! SPI Read Word Array

    void ReadDWordArray(uint16_t address, uint32_t *rxd, uint16_t nWords);

    // *****************************************************************************
    //! SPI Write Word Array

    void WriteDWordArray(uint16_t address, uint32_t *txd, uint16_t nWords);

    // *****************************************************************************
    // *****************************************************************************
    // Section: Configuration

    // *****************************************************************************
    //! CAN Control register configuration

    void Configure(CAN_CONFIG* config);

    // *****************************************************************************
    //! Reset Configure object to reset values
    void ConfigureObjectReset(CAN_CONFIG* config);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Operating mode

    // *****************************************************************************
    //! Select Operation Mode

    void OperationModeSelect(CAN_OPERATION_MODE opMode);

    // *****************************************************************************
    //! Get Operation Mode

    CAN_OPERATION_MODE OperationModeGet();


    // *****************************************************************************
    // *****************************************************************************
    // Section: CAN Transmit

    // *****************************************************************************
    //! Configure Transmit FIFO

    void TransmitChannelConfigure(CAN_TX_FIFO_CONFIG* config, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Reset TransmitChannelConfigure object to reset values

    void TransmitChannelConfigureObjectReset(CAN_TX_FIFO_CONFIG* config);

    // *****************************************************************************
    //! Configure Transmit Queue

    uint8_t TransmitQueueConfigure(CAN_TX_QUEUE_CONFIG* config);

    // *****************************************************************************
    //! Reset TransmitQueueConfigure object to reset values

    void TransmitQueueConfigureObjectReset(CAN_TX_QUEUE_CONFIG* config);

    // *****************************************************************************
    //! Transmit FIFO Update
    /*!
       Sets UINC of the transmit channel. Keeps TXREQ unchanged.
    */

    void TransmitChannelUpdate(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1, bool flush = true);

    // *****************************************************************************
    //! TX Channel Load
    /*!
       Loads data into Transmit channel
       Requests transmission, if flush==true
    */

    int8_t TransmitChannelLoad(CAN_TX_MSGOBJ* txObj, uint8_t *txd, uint32_t txdNumBytes, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1, bool flush = true);

    // *****************************************************************************
    //! TX Channel Flush
    /*!
       Set TXREG of one channel
    */

    void TransmitChannelFlush(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Transmit Channel Status Get

    CAN_TX_FIFO_STATUS TransmitChannelStatusGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Transmit FIFO Reset

    void TransmitChannelReset(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Request transmissions using TXREQ register

    void TransmitRequestSet(CAN_TXREQ_CHANNEL txreq);

    // *****************************************************************************
    //! Get TXREQ register

    uint32_t TransmitRequestGet();

    // *****************************************************************************
    //! Abort transmission of single FIFO

    void TransmitChannelAbort(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Abort All transmissions

    void TransmitAbortAll();

    // *****************************************************************************
    //! Set Transmit Bandwidth Sharing Delay
    void TransmitBandWidthSharingSet(CAN_TX_BANDWITH_SHARING txbws);

    // *****************************************************************************
    // *****************************************************************************
    // Section: CAN Receive

    // *****************************************************************************
    //! Filter Object Configuration
    /*!
       Configures ID of filter object
    */

    void FilterObjectConfigure(CAN_FILTER filter, CAN_FILTEROBJ_ID* id);

    // *****************************************************************************
    //! Filter Mask Configuration
    /*!
       Configures Mask of filter object
    */

    void FilterMaskConfigure(CAN_FILTER filter, CAN_MASKOBJ_ID* mask);

    // *****************************************************************************
    //! Link Filter to FIFO
    /*!
       Initializes the Pointer from Filter to FIFO
       Enables or disables the Filter
    */

    void FilterToFifoLink(CAN_FILTER filter, bool enable, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Filter Enable

    void FilterEnable(CAN_FILTER filter);

    // *****************************************************************************
    //! Filter Disable

    void FilterDisable(CAN_FILTER filter);

    // *****************************************************************************
    //! Set Device Net Filter Count
    void DeviceNetFilterCountSet(CAN_DNET_FILTER_SIZE dnfc);

    // *****************************************************************************
    //! Configure Receive FIFO

    uint8_t ReceiveChannelConfigure(CAN_RX_FIFO_CONFIG* config, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Reset ReceiveChannelConfigure object to reset value

    void ReceiveChannelConfigureObjectReset(CAN_RX_FIFO_CONFIG* config);

    // *****************************************************************************
    //! Receive Channel Status Get

    CAN_RX_FIFO_STATUS ReceiveChannelStatusGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Get Received Message
    /*!
       Reads Received message from channel
    */

    uint8_t ReceiveMessageGet(CAN_RX_MSGOBJ* rxObj, uint8_t *rxd, uint8_t nBytes, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Receive FIFO Reset

    void ReceiveChannelReset(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Receive FIFO Update
    /*!
       Sets UINC of the receive channel.
    */

    void ReceiveChannelUpdate(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Transmit Event FIFO

    // *****************************************************************************
    //! Transmit Event FIFO Status Get

    CAN_TEF_FIFO_STATUS TefStatusGet();

    // *****************************************************************************
    //! Get Transmit Event FIFO Message
    /*!
       Reads Transmit Event FIFO message
    */

    CAN_TEF_MSGOBJ TefMessageGet();

    // *****************************************************************************
    //! Transmit Event FIFO Reset

    void TefReset();

    // *****************************************************************************
    //! Transmit Event FIFO Update
    /*!
       Sets UINC of the TEF.
    */

    void TefUpdate();

    // *****************************************************************************
    //! Configure Transmit Event FIFO

    void TefConfigure(CAN_TEF_CONFIG* config);

    // *****************************************************************************
    //! Reset TefConfigure object to reset value

    void TefConfigureObjectReset(CAN_TEF_CONFIG* config);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Module Events

    // *****************************************************************************
    //! Module Event Get
    /*!
       Reads interrupt Flags
    */

    CAN_MODULE_EVENT ModuleEventGet();

    // *****************************************************************************
    //! Module Event Enable
    /*!
       Enables interrupts
    */

    void ModuleEventEnable(unsigned int flags);

    // *****************************************************************************
    //! Module Event Disable
    /*!
       Disables interrupts
    */

    void ModuleEventDisable(CAN_MODULE_EVENT flags);

    // *****************************************************************************
    //! Module Event Clear
    /*!
       Clears interrupt Flags
    */

    void ModuleEventClear(CAN_MODULE_EVENT flags);

    // *****************************************************************************
    //! Get RX Code

    CAN_RXCODE ModuleEventRxCodeGet();

    // *****************************************************************************
    //! Get TX Code

    CAN_TXCODE ModuleEventTxCodeGet();

    // *****************************************************************************
    //! Get Filter Hit

    CAN_FILTER ModuleEventFilterHitGet();

    // *****************************************************************************
    //! Get ICODE

    CAN_ICODE ModuleEventIcodeGet();

    // *****************************************************************************
    // *****************************************************************************
    // Section: Transmit FIFO Events

    // *****************************************************************************
    //! Transmit FIFO Event Get
    /*!
       Reads Transmit FIFO interrupt Flags
    */

    CAN_TX_FIFO_EVENT TransmitChannelEventGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Get pending interrupts of all transmit FIFOs

    inline uint32_t TransmitEventGet()
    {
      return ReadDWord(cREGADDR_CiTXIF);
    }

    // *****************************************************************************
    //! Get pending TXATIF of all transmit FIFOs

    inline uint32_t TransmitEventAttemptGet()
    {
      return ReadDWord(cREGADDR_CiTXATIF);
    }

    // *****************************************************************************
    //! Transmit FIFO Index Get
    /*!
       Reads Transmit FIFO Index
    */

    uint8_t TransmitChannelIndexGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Transmit FIFO Event Enable
    /*!
       Enables Transmit FIFO interrupts
    */

    void TransmitChannelEventEnable(CAN_TX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Transmit FIFO Event Disable
    /*!
       Disables Transmit FIFO interrupts
    */

    void TransmitChannelEventDisable(CAN_TX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);

    // *****************************************************************************
    //! Transmit FIFO Event Clear
    /*!
       Clears Transmit FIFO Attempts Exhausted interrupt Flag
    */

    void TransmitChannelEventAttemptClear(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH1);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Receive FIFO Events

    // *****************************************************************************
    //! Receive FIFO Event Get
    /*!
       Reads Receive FIFO interrupt Flags
    */

    CAN_RX_FIFO_EVENT ReceiveChannelEventGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Get pending interrupts of all receive FIFOs

    inline uint32_t ReceiveEventGet()
    {
      return ReadDWord(cREGADDR_CiRXIF);
    }

    // *****************************************************************************
    //!Get pending RXOVIF of all receive FIFOs

    inline uint32_t ReceiveEventOverflowGet()
    {
      return ReadDWord(cREGADDR_CiRXOVIF);
    }

    // *****************************************************************************
    //! Receive FIFO Index Get
    /*!
       Reads Receive FIFO Index
    */

    inline uint8_t ReceiveChannelIndexGet(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2)
    {
      return TransmitChannelIndexGet(channel);
    }

    // *****************************************************************************
    //! Receive FIFO Event Enable
    /*!
       Enables Receive FIFO interrupts
    */

    uint8_t ReceiveChannelEventEnable(CAN_RX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Receive FIFO Event Disable
    /*!
       Disables Receive FIFO interrupts
    */

    uint8_t ReceiveChannelEventDisable(CAN_RX_FIFO_EVENT flags, CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);

    // *****************************************************************************
    //! Receive FIFO Event Clear
    /*!
       Clears Receive FIFO Overflow interrupt Flag
    */

    uint8_t ReceiveChannelEventOverflowClear(CAN_FIFO_CHANNEL channel = CAN_FIFO_CH2);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Transmit Event FIFO Events

    // *****************************************************************************
    //! Transmit Event FIFO Event Get
    /*!
       Reads Transmit Event FIFO interrupt Flags
    */

    CAN_TEF_FIFO_EVENT TefEventGet();

    // *****************************************************************************
    //! Transmit Event FIFO Event Enable
    /*!
       Enables Transmit Event FIFO interrupts
    */

    void TefEventEnable(CAN_TEF_FIFO_EVENT flags);

    // *****************************************************************************
    //! Transmit Event FIFO Event Disable
    /*!
       Disables Transmit Event FIFO interrupts
    */

    void TefEventDisable(CAN_TEF_FIFO_EVENT flags);

    // *****************************************************************************
    //! Transmit Event FIFO Event Clear
    /*!
       Clears Transmit Event FIFO Overflow interrupt Flag
    */

    void TefEventOverflowClear();


    // *****************************************************************************
    // *****************************************************************************
    // Section: Error Handling

    // *****************************************************************************
    //! Transmit Error Count Get

    uint8_t ErrorCountTransmitGet();

    // *****************************************************************************
    //! Receive Error Count Get

    uint8_t ErrorCountReceiveGet();

    // *****************************************************************************
    //! Error State Get

    CAN_ERROR_STATE ErrorStateGet();

    // *****************************************************************************
    //! Error Counts and Error State Get
    /*!
       Returns content of complete CiTREC
    */

    void ErrorCountStateGet(uint8_t* tx_err_cnt, uint8_t* rx_err_cnt, CAN_ERROR_STATE* flags);

    // *****************************************************************************
    //! Get Bus Diagnostic Registers: all data at once, since we want to keep them in synch

    CAN_BUS_DIAGNOSTIC BusDiagnosticsGet();

    // *****************************************************************************
    //! Clear Bus Diagnostic Registers

    void BusDiagnosticsClear();

    // *****************************************************************************
    // *****************************************************************************
    // Section: ECC

    // *****************************************************************************
    //! Enable ECC

    void EccEnable();

    // *****************************************************************************
    //! Disable ECC

    void EccDisable();

    // *****************************************************************************
    //! ECC Event Get

    CAN_ECC_EVENT EccEventGet();

    // *****************************************************************************
    //! Set ECC Parity

    inline void EccParitySet(uint8_t parity)
    {
      WriteByte(cREGADDR_ECCCON + 1, parity);
    }

    // *****************************************************************************
    //! Get ECC Parity

    uint8_t EccParityGet()
    {
      return ReadByte(cREGADDR_ECCCON + 1);
    }

    // *****************************************************************************
    //! Get ECC Error Address

    uint16_t EccErrorAddressGet();

    // *****************************************************************************
    //! ECC Event Enable

    void EccEventEnable(CAN_ECC_EVENT flags);

    // *****************************************************************************
    //! ECC Event Disable

    void EccEventDisable(CAN_ECC_EVENT flags);

    // *****************************************************************************
    //! ECC Event Clear

    void EccEventClear(CAN_ECC_EVENT flags);

    // *****************************************************************************
    //! Initialize RAM

    void RamInit(uint8_t d);


    // *****************************************************************************
    // *****************************************************************************
    // Section: CRC

    // *****************************************************************************
    //! CRC Event Enable

    void CrcEventEnable(CAN_CRC_EVENT flags);

    // *****************************************************************************
    //! CRC Event Disable

    void CrcEventDisable(CAN_CRC_EVENT flags);

    // *****************************************************************************
    //! CRC Event Clear

    void CrcEventClear(CAN_CRC_EVENT flags);

    // *****************************************************************************
    //! CRC Event Get

    CAN_CRC_EVENT CrcEventGet();

    // *****************************************************************************
    //! Get CRC Value from device

    inline uint16_t CrcValueGet()
    {
      return ReadWord(cREGADDR_CRC);
    }


    // *****************************************************************************
    // *****************************************************************************
    // Section: Time Stamp

    // *****************************************************************************
    //! Time Stamp Enable

    void TimeStampEnable();

    // *****************************************************************************
    //! Time Stamp Disable

    void TimeStampDisable();

    // *****************************************************************************
    //! Time Stamp Get

    inline uint32_t TimeStampGet()
    {
      return ReadDWord(cREGADDR_CiTBC);
    }

    // *****************************************************************************
    //! Time Stamp Set

    inline void TimeStampSet(uint32_t ts)
    {
      WriteDWord(cREGADDR_CiTBC, ts);
    }

    // *****************************************************************************
    //! Time Stamp Mode Configure

    void TimeStampModeConfigure(CAN_TS_MODE mode);

    // *****************************************************************************
    //! Time Stamp Prescaler Set

    inline void TimeStampPrescalerSet(uint16_t ps)
    {
      WriteWord(cREGADDR_CiTSCON, ps);
    }


    // *****************************************************************************
    // *****************************************************************************
    // Section: Oscillator and Bit Time

    // *****************************************************************************
    //! Enable oscillator to wake-up from sleep

    void OscillatorEnable();

    // *****************************************************************************
    //! Set Oscillator Control

    void OscillatorControlSet(CAN_OSC_CTRL ctrl);

    void OscillatorControlObjectReset(CAN_OSC_CTRL* ctrl);

    // *****************************************************************************
    //! Get Oscillator Status

    CAN_OSC_STATUS OscillatorStatusGet();

    // *****************************************************************************
    //! Configure Bit Time registers (based on CAN clock speed)

    uint8_t BitTimeConfigure(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode, CAN_SYSCLK_SPEED clk);

    // *****************************************************************************
    //! Configure Nominal bit time for 40MHz system clock

    uint8_t BitTimeConfigureNominal40MHz(CAN_BITTIME_SETUP bitTime);

    // *****************************************************************************
    //! Configure Data bit time for 40MHz system clock

    uint8_t BitTimeConfigureData40MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);

    // *****************************************************************************
    //! Configure Nominal bit time for 20MHz system clock

    uint8_t BitTimeConfigureNominal20MHz(CAN_BITTIME_SETUP bitTime);

    // *****************************************************************************
    //! Configure Data bit time for 20MHz system clock

    uint8_t BitTimeConfigureData20MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);

    // *****************************************************************************
    //! Configure Nominal bit time for 10MHz system clock

    uint8_t BitTimeConfigureNominal10MHz(CAN_BITTIME_SETUP bitTime);

    // *****************************************************************************
    //! Configure Data bit time for 10MHz system clock

    uint8_t BitTimeConfigureData10MHz(CAN_BITTIME_SETUP bitTime, CAN_SSP_MODE sspMode);


    // *****************************************************************************
    // *****************************************************************************
    // Section: GPIO

    // *****************************************************************************
    //! Initialize GPIO Mode

    void GpioModeConfigure(GPIO_PIN_MODE gpio0, GPIO_PIN_MODE gpio1);

    // *****************************************************************************
    //! Initialize GPIO Direction

    void GpioDirectionConfigure(GPIO_PIN_DIRECTION gpio0, GPIO_PIN_DIRECTION gpio1);

    // *****************************************************************************
    //! Enable Transceiver Standby Control

    void GpioStandbyControlEnable();

    // *****************************************************************************
    //! Disable Transceiver Standby Control

    void GpioStandbyControlDisable();

    // *****************************************************************************
    //! Configure Open Drain Interrupts

    void GpioInterruptPinsOpenDrainConfigure(GPIO_OPEN_DRAIN_MODE mode);

    // *****************************************************************************
    //! Configure Open Drain TXCAN

    void GpioTransmitPinOpenDrainConfigure(GPIO_OPEN_DRAIN_MODE mode);

    // *****************************************************************************
    //! GPIO Output Pin Set

    uint8_t GpioPinSet(GPIO_PIN_POS pos, GPIO_PIN_STATE latch);

    // *****************************************************************************
    //! GPIO Input Pin Read

    GPIO_PIN_STATE GpioPinRead(GPIO_PIN_POS pos);

    // *****************************************************************************
    //! Configure CLKO Pin

    void GpioClockOutputConfigure(GPIO_CLKO_MODE mode);


    // *****************************************************************************
    // *****************************************************************************
    // Section: Miscellaneous

    // *****************************************************************************
    //! DLC to number of actual data bytes conversion

    inline int8_t DLCtoDataLength(uint32_t dlc) 
    {
      if(dlc < 16) {
        return DLC_DataLength[dlc];
      }
      return -1;
    }
    // *****************************************************************************
    //! Data bytes to DLC conversion

    int8_t DataLengthtoDLC(uint8_t datalength, uint8_t l=0, uint8_t r=16);

    // *****************************************************************************
    //! FIFO Index Get

    uint8_t FifoIndexGet(CAN_FIFO_CHANNEL channel);

    // *****************************************************************************
    //! Calculate CRC16

    uint16_t CalculateCRC16(uint8_t* data, uint16_t size);

	// *****************************************************************************
    //! Hardware Initialisation Routine
    void Init(CAN_BITTIME_SETUP selectedBitTime, CAN_FIFO_CHANNEL tx_fifo_ch = CAN_FIFO_CH1, CAN_FIFO_CHANNEL rx_fifo_ch = CAN_FIFO_CH2);
	
	// *****************************************************************************
    //! De-assert CS
    inline void RESET_CS()
    {
      *cs_reg &= ~cs_mask;
      //digitalWrite(cs_pin, LOW);
    }

    // *****************************************************************************
    //! Assert CS
    inline void SET_CS()
    {	  
      *cs_reg |= cs_mask;
      //digitalWrite(cs_pin, HIGH);
    }
	
	// *****************************************************************************
    //! Read int1 pin on MCP2517FD
    inline uint8_t available()
    {  
#ifdef DEBUG_FLAG
    	Serial.print("hello, available intr_reg and intr_mask is ");
    	Serial.print(*intr_reg, BIN);
    	Serial.print("  ");
    	Serial.println(intr_mask, BIN);
#endif
	    return ((*intr_reg & intr_mask) ? 0 : 1); //int1 LOW == Data available
	    //return (digitalRead(intr_pin) ? 0 : 1);
    }

    // *****************************************************************************
    //! Constructor
    //mcp2517fd(uint8_t cs, uint8_t intr, unsigned long spi = 20000000UL)  chaoqian modify
    mcp2517fd(uint8_t cs, uint8_t intr, unsigned long spi = 20000000UL)
    {
      cs_pin = cs;
      intr_pin = intr;
      spi_speed = spi;
    }

    // *****************************************************************************
    //! Deconstructor
    ~mcp2517fd()
    {
      #ifdef SPI_Init
      #undef SPI_Init
      SPI.end();
      #endif
    }
    // *****************************************************************************
    //chaoqian modify
    CAN_TX_FIFO_EVENT txFlags;
    CAN_RX_FIFO_EVENT rxFlags;
    CAN_ERROR_STATE   errorFlags;
    uint8_t txd[MAX_DATA_BYTES]; //tramitted data frame array
    uint8_t rxd[MAX_DATA_BYTES]; //receiveing data frame array
    CAN_TX_MSGOBJ txObj; //transmitted message FD frame
    CAN_RX_MSGOBJ rxObj; //received message FD frame
    volatile uint32_t can_msg_count = 0;
    const int test_pin = 8;
    uint8_t intr_pin;
    const int int0_pin = 2;
    bool itest = 0;
  private:
    // *****************************************************************************
    // *****************************************************************************
    // Section: Private Variables

    uint8_t spiTransmitBuffer[SPI_DEFAULT_BUFFER_LENGTH];
    uint8_t spiReceiveBuffer[SPI_DEFAULT_BUFFER_LENGTH];
    unsigned long spi_speed;
    uint8_t cs_pin;

    REGTYPE cs_mask, intr_mask;
    volatile REGTYPE *cs_reg, *intr_reg;
};

#endif
