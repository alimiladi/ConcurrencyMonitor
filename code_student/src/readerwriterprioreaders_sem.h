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
    mutexWriters.acquire();
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
    mutexWriters.release();
  }
};

#endif // READERWRITERPRIOREADERSSEM_H
