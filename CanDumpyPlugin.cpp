#include "CanDumpyPlugin.h"

#include <QtDebug>

#include "Helpers.h"

// Uncomment to enable debugging of this class
#define CAN_FF_DEBUG

#ifdef CAN_FF_DEBUG
   #define cffDebug qDebug
   #define cffWarning qWarning
#else
   #define cffDebug if(0) qDebug
   #define cffWarning if(0) qWarning
#endif

CanDumpyFrameFactory::CanDumpyFrameFactory():
   theFrameCount(0),
   theTotalBytesReceived(0),
   theTotalBytesFramed(0)
{
   cffDebug() << "Can Dumpy frame factory created";
}

CanDumpyFrameFactory::~CanDumpyFrameFactory()
{
   cffDebug() << "CAN Dumpy frame factory deleted";
}

/**
 *
 * CAN Dumpy Data Format (From CHV CTF at Defcon31
 *
 * Example Frames
 * (1691555726.720355) vcan0 184#535172DE483A7FBB
 * (1691555726.881234) vcan0 118#02B64CA6EE
 *
 * Timestamp
 * Can Interface
 * CAN ID
 * CAN Payload
 *
 */
void CanDumpyFrameFactory::pushMsgBytes(QByteArray msgData)
{
   theTotalBytesReceived += msgData.length();
   theDataBytes.append(msgData);

   cffDebug() << __PRETTY_FUNCTION__ << " received " << msgData.length() << "bytes of data";
   // dlDebug() << "WHOLE RAW: " << Helpers::qbyteToHexString(msgData);

   findFrames();

}

void CanDumpyFrameFactory::findFrames()
{
   while(findNextFrame())
   {
      theFrameCount++;
   }
}

bool CanDumpyFrameFactory::findNextFrame()
{
   int nextNewline = theDataBytes.indexOf('\n');

   if (nextNewline == -1)
   {
       // No frame end sequences
       return false;
   }

   QByteArray frameData = theDataBytes.left(nextNewline+1);
   theDataBytes.remove(0, nextNewline + 1);

   cffDebug() << "RAW: " << Helpers::qbyteToHexString(frameData);

   CanFrame* cf = new CanFrame(frameData);
   theFrames.append(cf);

   theTotalBytesFramed += nextNewline;

   return true;
}


bool CanDumpyFrameFactory::isFrameReady()
{
   cffDebug() << __PRETTY_FUNCTION__ << ( theFrames.empty() ? ": NO" : ": YES");
   return !theFrames.empty();

}

DataFrame* CanDumpyFrameFactory::getNextFrame()
{
   cffDebug() << __PRETTY_FUNCTION__ << " called";

   if (isFrameReady())
   {
      DataFrame* retVal = theFrames.front();
      theFrames.pop_front();
      return retVal;
   }
   else
   {
      cffWarning() << "Asked for a frame, but we have none!";
      return nullptr;
   }
}

QString CanDumpyFrameFactory::statusToString()
{
   QString retVal;
   retVal += "BytesRxed=";
   retVal += QString::number(theTotalBytesReceived);
   retVal += ", BytesFramed=";
   retVal += QString::number(theTotalBytesFramed);
   retVal += ", FrameCount=";
   retVal += QString::number(theFrameCount);
   retVal += ", BytesBuffered=";
   retVal += QString::number(theDataBytes.length());
   retVal += ", BytesDiscarded=";
   retVal += QString::number(theTotalBytesReceived - theTotalBytesFramed - theDataBytes.length());

   return retVal;
}

QString CanDumpyFrameFactory::protocolName()
{
   return "CanDumpy";
}
