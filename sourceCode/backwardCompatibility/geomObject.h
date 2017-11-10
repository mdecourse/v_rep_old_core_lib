
// This is needed only to be backward compatible with serialization version 15 (Serialization version 16 since Nov. 2012)

#pragma once

#include "shapeComponent.h"
#include "geomProxy.h"

class C3DObject;

class CGeomObject_OLD   
{
public:
    CGeomObject_OLD();
    virtual ~CGeomObject_OLD();

    void _commonInit();
    CGeomProxy* buildEquivalentGeomProxy();
    void computeBoundingBox();
    void serialize(CSer& ar);

    CShapeComponent_OLD* _geomStruct;
    C7Vector _creationTransf;

    std::vector<float> visibleVertices;
    std::vector<int> visibleIndices;
    std::vector<float> visibleNormals;
    std::vector<unsigned char> visibleEdges;
    std::vector<float> visibleTextureCoords_notCopiedNorSerialized;

    void* collNode;
    std::vector<int> calcEdgeIDs;
    std::vector<int> calcFaceIDs;
    std::vector<int> calcPointIDs;  
    std::vector<float> calcVertices;
    std::vector<int> calcIndices;
    std::vector<int> calcSegments;
    std::vector<std::vector<int> > calcPolygons;

    C3Vector _boundingBoxHalfSizes;
private:
    void loadPackedIntegers(CSer& ar,std::vector<int>& data);
};
