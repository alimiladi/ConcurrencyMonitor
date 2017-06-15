/** @file abstractreaderwriter.h
 *  @brief Abstract class for reader-writers
 *
 *  This class is the super class of all other reader-writer classes.
 *  It offers the 4 standard methods.
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 *
 *
 * Ce fichier a été repris pour faire le labo ainsi nous gardons la
 * même structure que celle vue en cours.
 * Il s'agit de représenter une ressource de manière générale
 * concernant un modèle lecteurs-rédacteurs en PCO.
 */

#ifndef ABSTRACTREADERWRITER_H
#define ABSTRACTREADERWRITER_H


class AbstractReaderWriter {
public:
    AbstractReaderWriter(){};
    virtual ~AbstractReaderWriter(){};
    virtual void lockReading()   = 0;
    virtual void lockWriting()   = 0;
    virtual void unlockReading() = 0;
    virtual void unlockWriting() = 0;
};

#endif // ABSTRACTREADERWRITER_H
