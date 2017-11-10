
#pragma once

#include "vrepMainHeader.h"
#include "ser.h"

class CTextureObject
{
public:
    CTextureObject(); // for serialization
    CTextureObject(int sizeX,int sizeY);
    virtual ~CTextureObject();

    void setObjectID(int newID);
    int getObjectID();
    bool isSame(CTextureObject* obj);
    void setObjectName(const char* newName);
    std::string getObjectName();
    void getTextureSize(int& sizeX,int& sizeY);
    void setImage(bool rgba,bool horizFlip,bool vertFlip,unsigned char* data);
    CTextureObject* copyYourself();
    void serialize(CSer& ar);
    void setTextureBuffer(const std::vector<unsigned char>& tb);
    void getTextureBuffer(std::vector<unsigned char>& tb);
    unsigned char* getTextureBufferPointer();
    void lightenUp();
    void setRandomContent();

    bool announceGeneralObjectWillBeErased(int objectID,int subObjectID);
    void addDependentObject(int objectID,int subObjectID);
    void clearAllDependencies();
    void transferDependenciesToThere(CTextureObject* receivingObject);

    char* readPortionOfTexture(int posX,int posY,int sizeX,int sizeY);
    bool writePortionOfTexture(const unsigned char* rgbData,int posX,int posY,int sizeX,int sizeY,bool circular,float interpol);

    unsigned int getCurrentTextureContentUniqueId();

    void setOglTextureName(unsigned int n);
    unsigned int getOglTextureName();
    bool getChangedFlag();
    void setChangedFlag(bool c);

protected:
    std::vector<unsigned char> _textureBuffer;
    unsigned int _oglTextureName;
    int _objectID;
    std::string _objectName;
    int _textureSize[2];
    bool _providedImageWasRGBA;     // just needed to reduce serialization size!
    bool _changedFlag;
    unsigned int _currentTextureContentUniqueId;

    std::vector<int> _dependentObjects;
    std::vector<int> _dependentSubObjects;
    static unsigned int _textureContentUniqueId;
};
