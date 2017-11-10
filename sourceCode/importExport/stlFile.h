
#pragma once

#include "vrepMainHeader.h"
#include "importExport.h"
#include "shape.h"

class CStlFile : public CImportExport
{
public:
    CStlFile(int format);
    virtual ~CStlFile();
    bool importFunc(std::string fileName);
    bool exportFunc(std::string fileName,bool poly,std::vector<int>* sel);
    bool exportFunc(std::string fileName,bool poly,const std::vector<std::vector<float>*>& vertices,const std::vector<std::vector<int>*>& indices,const std::vector<std::string>& names);

private:
    bool _readFile(VArchive& archive,int format);
    void _writeTriangle(VArchive& archive,C3Vector& v0,C3Vector& v1,C3Vector& v2);
};
