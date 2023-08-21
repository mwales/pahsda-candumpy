#include "CanFrame.h"

#include <QtDebug>
#include <QByteArray>
#include <QVector>

// Uncomment to enable debugging of this class
// #define CAN_FRAME_DEBUG

#ifdef CAN_FRAME_DEBUG
   #define cFrameDebug qDebug
   #define cFrameWarning qWarning
#else
   #define cFrameDebug if(0) qDebug
   #define cFrameWarning if(0) qWarning
#endif


CanFrame::CanFrame(QByteArray framedData)
   :DataFrame()
{
   cFrameDebug() << "Creating a CAN Frame";
   theValidFrameFlag = false;

   addField(CAN_ID,                "ID",              "ID");
   addField(CAN_DATA,              "CAN Data",        "CAN Data");

   QList<int> sortingOrder;
   sortingOrder.append(CAN_ID);
   setSortingIndexes(sortingOrder);

   int firstSpace = framedData.indexOf(' ');
   int secondSpace = framedData.indexOf(' ', firstSpace + 1);
   int firstHash = framedData.indexOf('#');

   if ( (firstSpace == -1) || (secondSpace == -1) || (firstHash == -1) )
   {
       cFrameWarning() << "It's all gone wrong, both spaces (indexes = " <<
                          firstSpace << " and " << secondSpace << ") and hash (" <<
                          firstHash << ") not found";
   }

   if (secondSpace > firstHash)
   {
       cFrameWarning() << "Frame format invlaid (second space after hash symbol";
   }


   updateFieldValue(CAN_ID,
                    QByteArray::fromHex(framedData.mid(secondSpace + 1, firstHash - secondSpace)));
   updateFieldValue(CAN_DATA,
                    QByteArray::fromHex(framedData.mid(firstHash + 1)));

   QVector<int> highlightList;
   highlightList << CAN_DATA;
   setHighlighting(highlightList, 4);
}

