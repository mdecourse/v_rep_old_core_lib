
#pragma once

#include "vrepMainHeader.h"
#include "importExport.h"
#include "shape.h"

class CObjFile : public CImportExport
{
public:
    CObjFile();
    virtual ~CObjFile();
    bool importFunc(std::string fileName);
    bool exportFunc(std::string fileName,bool poly,std::vector<int>* sel);
    bool exportFunc(std::string fileName,bool poly,const std::vector<std::vector<float>*>& vertices,const std::vector<std::vector<int>*>& indices,const std::vector<std::string>& names);

private:
    bool _readFile(VArchive& archive,const char* fileName);
    bool _getCode(VArchive& archive);
    bool _getMaterialCode(VArchive& archive,unsigned int& archiveCurrentPos,std::string& matCode,std::string& matValue);
    bool _getVertexOrTextureCoordOrNormalValue(std::string& text,float& v);
    int _getIndexValue(std::string& text,int& v,int& vt,int& vn);
    bool _writeVertex(VArchive& archive,C3Vector& v);
    bool _writeIndex(VArchive& archive,int a,int b,int c);
    void _appendMaterialFileToMaterialList(const char* matFile,std::vector<SObjMaterial>& materialList);


    std::string code;
    std::string value;
};
