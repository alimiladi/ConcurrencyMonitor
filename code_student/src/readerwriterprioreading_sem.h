/** @file readerwriteprioreader_sem.h
 *  @brief Reader-writer with priority for readers when reading
 *
 *  Implementation of a reader-writer resource manager with priority
 *  for readers only when a reading is currently under way.
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

#ifndef READERWRITERPRIOREADINGSEM_H
#define READERWRITERPRIOREADINGSEM_H


#include "osemaphore.h"

#include "abstractreaderwriter.h"


class ReaderWriterPrioReading_Sem : public AbstractReaderWriter {
protected:
  OSemaphore mutexReaders;
  OSemaphore writer;
  int nbReaders;
  QString name;

public:

  QString getName(){
      return name;
  }

  ReaderWriterPrioReading_Sem() :
  mutexReaders(1),
  writer(1),
  nbReaders(0),
  name("Reader-Writer-PrioReading_Sem"){
      mutexReaders.setName("mutexReaders");
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
    writer.acquire();
  }

  void unlockWriting() {
    writer.release();
  }
};


#endif // READERWRITERPRIOREADINGSEM_H
