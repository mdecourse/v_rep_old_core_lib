
#include "vrepMainHeader.h"
#include "importExport.h"

CImportExport::CImportExport()
{
    archiveLength=0;
    actualPosition=0;
    numberOfTrianglesRead=0;
}

CImportExport::~CImportExport()
{
    for (int i=0;i<int(_importedVertices.size());i++)
        delete _importedVertices[i];
    for (int i=0;i<int(_importedIndices.size());i++)
        delete _importedIndices[i];
    for (int i=0;i<int(_importedTextureCoordinates.size());i++)
        delete _importedTextureCoordinates[i];
    for (int i=0;i<int(_importedNormals.size());i++)
        delete _importedNormals[i];
    for (int i=0;i<int(_importedMaterials.size());i++)
        delete _importedMaterials[i];
}

bool CImportExport::importFunc(std::string fileName)
{
    return(true);
}
bool CImportExport::exportFunc(std::string fileName,bool poly,std::vector<int>* sel)
{
    return(true);
}
bool CImportExport::exportFunc(std::string fileName,bool poly,const std::vector<std::vector<float>*>& vertices,const std::vector<std::vector<int>*>& indices,const std::vector<std::string>& names) // ADDED BY MARC
{
    return(true);
}

int CImportExport::getFileFormat()
{
    return(fileFormat);
}

int CImportExport::getGroupCount()
{
    return((int)_importedVertices.size());
}
int CImportExport::getVertexCount(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedVertices.size())) )
        return(0);
    return((int)_importedVertices[groupIndex]->size()/3);
}
int CImportExport::getTriangleCount(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedIndices.size())) )
        return(0);
    return((int)_importedIndices[groupIndex]->size()/3);
}
int CImportExport::getTextureCoordinateCount(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedVertices.size())) )
        return(0);
    return((int)_importedTextureCoordinates[groupIndex]->size()/2);
}

int CImportExport::getNormalsCount(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedVertices.size())) )
        return(0);
    return((int)_importedNormals[groupIndex]->size()/3);
}

bool CImportExport::getVertex(int groupIndex,int vertexIndex,float vertex[3])
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedVertices.size())) )
        return(false);
    if ( (vertexIndex<0)||(vertexIndex>=int(_importedVertices[groupIndex]->size())/3) )
        return(false);
    vertex[0]=_importedVertices[groupIndex]->at(3*vertexIndex+0);
    vertex[1]=_importedVertices[groupIndex]->at(3*vertexIndex+1);
    vertex[2]=_importedVertices[groupIndex]->at(3*vertexIndex+2);
    return(true);
}
bool CImportExport::getTriangle(int groupIndex,int triangleIndex,int triangle[3])
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedIndices.size())) )
        return(false);
    if ( (triangleIndex<0)||(triangleIndex>=int(_importedIndices[groupIndex]->size())/3) )
        return(false);
    triangle[0]=_importedIndices[groupIndex]->at(3*triangleIndex+0);
    triangle[1]=_importedIndices[groupIndex]->at(3*triangleIndex+1);
    triangle[2]=_importedIndices[groupIndex]->at(3*triangleIndex+2);
    return(true);
}

bool CImportExport::getTextureCoordinate(int groupIndex,int coordIndex,float coord[2])
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedVertices.size())) )
        return(false);
    if ( (coordIndex<0)||(coordIndex>=int(_importedTextureCoordinates[groupIndex]->size())/2) )
        return(false);
    coord[0]=_importedTextureCoordinates[groupIndex]->at(2*coordIndex+0);
    coord[1]=_importedTextureCoordinates[groupIndex]->at(2*coordIndex+1);
    return(true);
}

bool CImportExport::getNormal(int groupIndex,int normalIndex,float normal[3])
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedIndices.size())) )
        return(false);
    if ( (normalIndex<0)||(normalIndex>=int(_importedNormals[groupIndex]->size())/3) )
        return(false);
    normal[0]=_importedNormals[groupIndex]->at(3*normalIndex+0);
    normal[1]=_importedNormals[groupIndex]->at(3*normalIndex+1);
    normal[2]=_importedNormals[groupIndex]->at(3*normalIndex+2);
    return(true);
}

std::string CImportExport::getName(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedNames.size())) )
        return("ERROR");
    return(_importedNames[groupIndex]);
}

SObjMaterial* CImportExport::getAndClearMaterial(int groupIndex)
{
    if ( (groupIndex<0)||(groupIndex>=int(_importedMaterials.size())) )
        return(NULL);
    SObjMaterial* retVal=_importedMaterials[groupIndex];
    _importedMaterials[groupIndex]=NULL;
    return(retVal);
}
