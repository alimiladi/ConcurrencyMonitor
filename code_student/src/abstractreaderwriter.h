/** @file abstractreaderwriter.h
 *  @brief Abstract class for reader-writers
 *
 *  This class is the super class of all other reader-writer classes.
 *  It offers the 4 standard methods.
 *
 *  @author Yann Thoma
 *  @date 08.05.2017
 *  @bug No known bug
 */

#ifndef ABSTRACTREADERWRITER_H
#define ABSTRACTREADERWRITER_H


class AbstractReaderWriter {
public:
    AbstractReaderWriter(){};
    virtual ~AbstractReaderWriter(){};
    virtual void lockReading(unsigned int id)   = 0;
    virtual void lockWriting(unsigned int id)   = 0;
    virtual void unlockReading(unsigned int id) = 0;
    virtual void unlockWriting(unsigned int id) = 0;
};

#endif // ABSTRACTREADERWRITER_H
