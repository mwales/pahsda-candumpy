#ifndef CAN_DUMPY_FRAME_FACTORY_H
#define CAN_DUMPY_FRAME_FACTORY_H

#include <QObject>
#include <QList>

#include "DataFrameFactoryInterface.h"
#include "CanFrame.h"
/**
 * Plugin for PAHSDA
 *
 * Decodes data stream into CAN bus messages
 */
class CanDumpyFrameFactory: public QObject, public DataFrameFactoryInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID DataFactoryInterface_iid)
   Q_INTERFACES(DataFrameFactoryInterface)

public:
   CanDumpyFrameFactory();

   virtual ~CanDumpyFrameFactory();

   virtual void pushMsgBytes(QByteArray msgData);

   virtual bool isFrameReady();

   virtual DataFrame* getNextFrame();

   virtual QString statusToString();

   virtual QString protocolName();

protected:

   void findFrames();

   bool findNextFrame();

   QByteArray theDataBytes;

   int theFrameCount;

   int theTotalBytesReceived;

   int theTotalBytesFramed;

   QList<CanFrame*> theFrames;
};

#endif // CAN_DUMPY_FRAME_FACTORY_H
