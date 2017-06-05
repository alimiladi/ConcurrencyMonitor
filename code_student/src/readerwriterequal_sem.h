/** @file readerwriterequal.h
 *  @brief Reader-writer with equal priorities
 *
 *  Implementation of a reader-writer resource manager with equal priorities.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */


#ifndef READERWRITEREQUAL_H
#define READERWRITEREQUAL_H

#include <QSemaphore>

#include "abstractreaderwriter.h"


class ReaderWriterEqual : public AbstractReaderWriter {
protected:
  QSemaphore mutex;
  QSemaphore fifo;
  QSemaphore writer;
  int nbReaders;

public:
  ReaderWriterEqual() :
    mutex(1),
    fifo(1),
    writer(1),
    nbReaders(0) {}

  void lockReading() {
    fifo.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders==1) {
      writer.acquire();
    }
    mutex.release();
    fifo.release();
  }

  void unlockReading() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders==0) {
      writer.release();
    }
    mutex.release();
  }

  void lockWriting() {
    fifo.acquire();
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
    fifo.release();
  }
};

#endif // READERWRITEREQUAL_H
