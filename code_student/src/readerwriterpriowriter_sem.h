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

  void lockReading(unsigned int id) {
    mutexReaders.acquire(id);
    reader.acquire(id);
    mutex.acquire(id);
    nbReaders++;
    if (nbReaders==1)
      writer.acquire(id);
    mutex.release(id);
    reader.release(id);
    mutexReaders.release(id);
  }

  void unlockReading(unsigned int id) {
    mutex.acquire(id);
    nbReaders--;
    if (nbReaders==0)
      writer.release(id);
    mutex.release(id);
  }

  void lockWriting(unsigned int id) {
    mutexWriters.acquire(id);
    nbWriters++;
    if (nbWriters==1)
      reader.acquire(id);
    mutexWriters.release(id);
    writer.acquire(id);
  }

  void unlockWriting(unsigned int id) {
    writer.release(id);
    mutexWriters.acquire(id);
    nbWriters--;
    if (nbWriters==0)
      reader.release(id);
    mutexWriters.release(id);
  }
};

#endif // READERWRITERPRIOWRITERSEM_H
