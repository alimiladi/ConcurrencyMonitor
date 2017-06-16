/** @file readerwritepriowriter_sem.h
 *  @brief Reader-writer with priority for writers
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for writers.
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
  QString name;


public:

  QString getName(){
      return name;
  }

  ReaderWriterPrioWriter_Sem() :
    mutexReaders(1),
    mutexWriters(1),
    reader(1),
    writer(1),
    mutex(1),

    nbReaders(0),
    nbWriters(0),
    name("Reader-Writer-PrioWriter_Semaphore"){
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
