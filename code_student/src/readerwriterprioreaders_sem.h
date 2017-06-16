/** @file readerwriteprioreader_sem.h
 *  @brief Reader-writer with priority for readers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers.
 *
 *
 *  @author Yann Thoma : modifié par Ali Miladi, Quentin Zeller,
 *  Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe est reprise du professeur. Pour cela nous n'ajouterons pas plus
 * de commentaire ou modifications si ce n'est le passage de QSemaphore à
 * OSemaphore, permettant ainsi la journalisation de logs sur les différentes files.
 * Nous avons, par conséquent, aussi du nommer ces différentes files
 * ainsi que cette ressource.
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

  QString getName(){
      return name;
  }

  ReaderWriterPrioReaders_Sem() :
  mutexReaders(1),
  mutexWriters(1),
  writer(1),
  nbReaders(0),
  name("Reader-Writer-PrioReaders_Sem"){
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
