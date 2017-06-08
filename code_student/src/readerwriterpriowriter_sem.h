/** @file readerwritepriowriter_sem.h
 *  @brief Reader-writer with priority for writers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for writers.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */


#ifndef READERWRITERPRIOWRITERSEM_H
#define READERWRITERPRIOWRITERSEM_H

#include <QSemaphore>
#include "osemaphore.h"

#include "abstractreaderwriter.h"

class ReaderWriterPrioWriter_Sem : public AbstractReaderWriter {
protected:
  OSemaphore mutexReaders;
  OSemaphore mutexWriters;
  OSemaphore reader;
  OSemaphore writer;
  OSemaphore mutex;
  int nbReaders, nbWriters;


public:
  ReaderWriterPrioWriter_Sem() :
    mutexReaders(1),
    mutexWriters(1),
    reader(1),
    writer(1),
    mutex(1),

    nbReaders(0),
    nbWriters(0){
      mutex.setName("mutex");
      mutexReaders.setName("mutexReaders");
      mutexWriters.setName("mutexWriters");
      reader.setName("reader");
      writer.setName("writer");
  }

  void lockReading() {
    mutexReaders.acquire();
    reader.acquire();
    mutex.acquire();
    nbReaders++;
    if (nbReaders==1)
      writer.acquire();
    mutex.release();
    reader.release();
    mutexReaders.release();
  }

  void unlockReading() {
    mutex.acquire();
    nbReaders--;
    if (nbReaders==0)
      writer.release();
    mutex.release();
  }

  void lockWriting() {
    mutexWriters.acquire();
    nbWriters++;
    if (nbWriters==1)
      reader.acquire();
    mutexWriters.release();
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
    mutexWriters.acquire();
    nbWriters--;
    if (nbWriters==0)
      reader.release();
    mutexWriters.release();
  }
};

#endif // READERWRITERPRIOWRITERSEM_H
