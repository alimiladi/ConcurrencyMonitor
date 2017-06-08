/** @file readerwriteprioreader_sem.h
 *  @brief Reader-writer with priority for readers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers.
 *
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

#ifndef READERWRITERPRIOREADERSSEM_H
#define READERWRITERPRIOREADERSSEM_H

#include "osemaphore.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioReaders_Sem :
  public AbstractReaderWriter {
protected:
  OSemaphore mutexReaders;
  OSemaphore mutexWriters;
  OSemaphore writer;
  int nbReaders;
  QString name;

public:
  ReaderWriterPrioReaders_Sem() :
  mutexReaders(1),
  mutexWriters(1),
  writer(1),
  nbReaders(0){
      mutexReaders.setName("mutexReaders");
      mutexWriters.setName("mutexWriters");
      writer.setName("writer");
  }

  void lockReading(unsigned int id) {
    mutexReaders.acquire(id);
    nbReaders++;
    if (nbReaders==1) {
      writer.acquire(id);
    }
    mutexReaders.release(id);
  }

  void unlockReading(unsigned int id) {
    mutexReaders.acquire(id);
    nbReaders--;
    if (nbReaders==0) {
      writer.release(id);
    }
    mutexReaders.release(id);
  }

  void lockWriting(unsigned int id) {
    mutexWriters.acquire(id);
    writer.acquire(id);
  }

  void unlockWriting(unsigned int id) {
    writer.release(id);
    mutexWriters.release(id);
  }
};

#endif // READERWRITERPRIOREADERSSEM_H
