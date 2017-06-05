/** @file readerwriteprioreader.h
 *  @brief Reader-writer with priority for readers when reading
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers only when a reading is currently under way.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

#ifndef READERWRITERPRIOREADING_H
#define READERWRITERPRIOREADING_H


#include <QSemaphore>

#include "readerwriterprioreading.h"



class ReaderWriterPrioReading : public AbstractReaderWriter {
protected:
  QSemaphore mutexReaders;
  QSemaphore writer;
  int nbReaders;
public:
  ReaderWriterPrioReading() :
  mutexReaders(1),
  writer(1),
  nbReaders(0){}

  void lockReading() {
    mutexReaders.acquire();
    nbReaders++;
    if (nbReaders==1) {
      writer.acquire();
    }
    mutexReaders.release();
  }

  void unlockReading() {
    mutexReaders.acquire();
    nbReaders--;
    if (nbReaders==0) {
      writer.release();
    }
    mutexReaders.release();
  }

  void lockWriting() {
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
  }
};


#endif // READERWRITERPRIOREADING_H
