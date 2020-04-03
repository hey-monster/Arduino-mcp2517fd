#include "mcp2517fd.h"

//cs_pin = 10, intr_pin = 2
mcp2517fd CAN_FD_Ch1(10, 2);
unsigned char flagRecv = 0;
void setup() {
  delay(1000);
  Serial.begin(115200);

  Serial.println("setup start");
  CAN_FD_Ch1.Init(CAN_500K_2M);       // 500k arbitration rate and 2Mbps data rate
  // APP_CANFDSPI_Init(CAN_500K_4M);       // 500k arbitration rate and 2Mbps data rate
  // APP_CANFDSPI_Init(CAN_500K_8M);       // 500k arbitration rate and 2Mbps data rate

  CAN_FD_Ch1.txObj.bF.ctrl.IDE = 0;      // Extended CAN ID false
  CAN_FD_Ch1.txObj.bF.id.SID   =  0x322;    // CAN ID
  CAN_FD_Ch1.txObj.bF.ctrl.BRS = 1;      // Switch Bitrate true (switch to 2Mbps)
  CAN_FD_Ch1.txObj.bF.ctrl.FDF = 1;      // CAN FD true
  CAN_FD_Ch1.txObj.bF.ctrl.DLC = 15;     // Data length, 64 bytes

  CAN_FD_Ch1.txd[4] = CAN_FD_Ch1.can_msg_count >> 24;
  CAN_FD_Ch1.txd[5] = CAN_FD_Ch1.can_msg_count >> 16;
  CAN_FD_Ch1.txd[6] = CAN_FD_Ch1.can_msg_count >> 8;
  CAN_FD_Ch1.txd[7] = CAN_FD_Ch1.can_msg_count;
  //CAN_FD_Ch1.txd[4] = 0xd5;
  //CAN_FD_Ch1.txd[5] = 0x00;
  //CAN_FD_Ch1.txd[6] = CAN_FD_Ch1.can_msg_count >> 8;
  //CAN_FD_Ch1.txd[7] = CAN_FD_Ch1.can_msg_count;
  Serial.println("setup tx msg is ok");

  attachInterrupt(0,MCP2517_ISR, FALLING); // start interrupt
  attachInterrupt(1,MCP2517_ISR2, FALLING); // start interrupt 
  delay(100);
  //CAN_FD_Ch1.APP_TransmitMessageQueue();
}

void MCP2517_ISR()
{
    flagRecv = 1;
}
void MCP2517_ISR2()
{
    flagRecv = 2;
}
void loop() {
  //Serial.println("CAN Bus loop start----------------------");
  //CAN_FD_Ch1.APP_TransmitMessageQueue();

  //CAN_FD_Ch1.txd[4] = CAN_FD_Ch1.can_msg_count >> 24;
  //CAN_FD_Ch1.txd[5] = CAN_FD_Ch1.can_msg_count >> 16;
  //CAN_FD_Ch1.txd[6] = CAN_FD_Ch1.can_msg_count >> 8;
  //CAN_FD_Ch1.txd[7] = CAN_FD_Ch1.can_msg_count;

  //CAN_FD_Ch1.can_msg_count++;
  delay(500);
  CAN_FD_Ch1.APP_ReceiveMessage_Tasks();
  if(flagRecv == 1)
  {
    //Serial.println("flagRecv is 1");
  }
  if(flagRecv == 2)
  {
    //Serial.println("flagRecv is 2");
  }
  //Serial.println("CAN Bus loop end----------------------");
}
