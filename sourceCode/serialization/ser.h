
#pragma once

#include "serBase.h"
#include "vrepMainHeader.h"
#include "vArchive.h"

#define SER_VREP_HEADER "VREP" // The file header since 03/07/2009
#define SER_END_OF_OBJECT "EOO"
#define SER_NEXT_STEP "NXT"
#define SER_END_OF_FILE "EOF"

class CSer : public CSerBase
{
public:
    CSer(VArchive& ar);
    CSer(std::vector<char>& bufferArchive);
    virtual ~CSer();

    void writeOpen();
    void writeClose(bool compress);

    int readOpen(int& serializationVersion,unsigned short& vrepVersionThatWroteThis,int& licenseTypeThatWroteThis,char& revNumber);
    void readClose();

    CSer& operator<< (const int& v);
    CSer& operator<< (const float& v);
    CSer& operator<< (const double& v);
    CSer& operator<< (const unsigned short& v);
    CSer& operator<< (const unsigned int& v);
    CSer& operator<< (const quint64& v);
    CSer& operator<< (const long& v);
    CSer& operator<< (const unsigned char& v);
    CSer& operator<< (const char& v);
    CSer& operator<< (const std::string& v);

    CSer& operator>> (int& v);
    CSer& operator>> (float& v);
    CSer& operator>> (double& v);
    CSer& operator>> (unsigned short& v);
    CSer& operator>> (unsigned int& v);
    CSer& operator>> (quint64& v);
    CSer& operator>> (long& v);
    CSer& operator>> (unsigned char& v);
    CSer& operator>> (char& v);
    CSer& operator>> (std::string& v);

    void flush(bool writeNbOfBytes=true);
    
    bool isStoring();
    void setCountingMode(bool force=false);
    bool setWritingMode(bool force=false);
    void disableCountingModeExceptForExceptions();
    std::vector<unsigned char>* getBufferPointer();
    std::vector<unsigned char>* getFileBuffer();
    int getFileBufferReadPointer();
    void addOffsetToFileBufferReadPointer(int off);
    int getCounter();
    void loadUnknownData();
    bool getFoundUnknownCommands();

    unsigned short getVrepVersionThatWroteThisFile();
    int getLicenseTypeThatWroteThisFile();
    int getSerializationVersionThatWroteThisFile();
    
    void storeDataName(const char* name);
    std::string readDataName();
    int readBytesButKeepPointerUnchanged(unsigned char* buffer,int desiredCount);
    VArchive& getArchive();

    static int SER_SERIALIZATION_VERSION;
    static int SER_MIN_SERIALIZATION_VERSION_THAT_CAN_READ_THIS;
    static int SER_MIN_SERIALIZATION_VERSION_THAT_THIS_CAN_READ;

private:

    void _commonInit();
    VArchive* theArchive;
    std::vector<char>* _bufferArchive;

    int counter;
    int countingMode;
    bool _coutingModeDisabledExceptForExceptions;
    std::vector<unsigned char> buffer;
    std::vector<unsigned char> _fileBuffer;
    int _fileBufferReadPointer;
    bool _foundUnknownCommands;

    unsigned short _vrepVersionThatWroteThis;
    int _licenseTypeThatWroteThis;
    int _serializationVersionThatWroteThisFile;
};
