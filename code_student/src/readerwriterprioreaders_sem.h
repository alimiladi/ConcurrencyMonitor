/** @file readerwriteprioreader.h
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

#ifndef READERWRITERPRIOREADERS_H
#define READERWRITERPRIOREADERS_H

#include <QSemaphore>

#include "abstractreaderwriter.h"

class ReaderWriterPrioReaders :
  public AbstractReaderWriter {
protected:
  QSemaphore mutexReaders;
  QSemaphore mutexWriters;
  QSemaphore writer;
  int nbReaders;

public:
  ReaderWriterPrioReaders() :
  mutexReaders(1),
  mutexWriters(1),
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
    mutexWriters.acquire();
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
    mutexWriters.release();
  }
};

#endif // READERWRITERPRIOREADERS_H
