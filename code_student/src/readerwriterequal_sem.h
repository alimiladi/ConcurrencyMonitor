/** @file readerwriterequal_sem.h
 *  @brief Reader-writer with equal priorities
 *
 *  Implementation of a reader-writer resource manager with equal priorities.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */


#ifndef READERWRITEREQUALSEM_H
#define READERWRITEREQUALSEM_H

#include "osemaphore.h"

#include "abstractreaderwriter.h"


class ReaderWriterEqual_Sem : public AbstractReaderWriter {
protected:
  OSemaphore mutex;
  OSemaphore fifo;
  OSemaphore writer;
  int nbReaders;
  QString name;

public:

  QString getName(){
      return name;
  }

  ReaderWriterEqual_Sem() :
    mutex(1),
    fifo(1),
    writer(1),
    nbReaders(0),
    name("Reader-Writer-Equal_Sem")
  {
    mutex.setName("mutex");
    fifo.setName("fifo");
    writer.setName("writer");
  }

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

#endif // READERWRITEREQUALSEM_H
