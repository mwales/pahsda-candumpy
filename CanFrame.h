#ifndef CANFRAME_H
#define CANFRAME_H

#include "DataFrame.h"

/**
 * Will divide the CAN framed data into different fields for easy viewing and sorting
 */
class CanFrame : public DataFrame
{
public:
   CanFrame(QByteArray framedData);

   //const int CAN_SOF= 0;
   const int CAN_ID = 0;
   //const int CAN_STUFF_BIT = 2;
   //const int CAN_RTR_BIT = 3;
   //const int CAN_IDE = 4;
   //const int CAN_RESERVED = 5;

   //const int CAN_DL = 6;

   const int CAN_DATA = 1;

   //const int CAN_CRC = 8;
   //const int CAN_CRC_DELIMITER = 9;
   //const int CAN_ACK_SLOT = 10;
   //const int CAN_ACK_DELIMITER = 11;
   //const int CAN_EOF = 12;
protected:
   bool theValidFrameFlag;
};

#endif // CANFRAME_H
