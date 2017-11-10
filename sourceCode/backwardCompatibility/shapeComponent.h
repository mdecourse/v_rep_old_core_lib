
// This is needed only to be backward compatible with serialization version 15 (Serialization version 16 since Nov. 2012)

#pragma once

#include "visualParam.h"
#include "textureProperty.h"
#include "geomWrap.h"

class CGeomObject_OLD;

class CShapeComponent_OLD  
{
public:
    CShapeComponent_OLD();
    virtual ~CShapeComponent_OLD();

    bool getInsideAndOutsideFacesSameColor();
    CTextureProperty* getTextureProperty();
    void setTextureProperty(CTextureProperty* tp);
    bool getVisibleEdges();
    int getEdgeWidth();
    bool getCulling();
    bool getWireframe();
    float getGouraudShadingAngle();
    std::string getName();
    float getMass();
    bool getBulletStickyContact();
    float getBulletRestitution();
    float getBulletFriction();
    float getBulletLinearDamping();
    float getBulletAngularDamping();
    bool getBulletNonDefaultCollisionMargin();
    float getBulletNonDefaultCollisionMarginFactor();
    int getOdeMaxContacts();
    float getOdeFriction();
    float getOdeSoftERP();
    float getOdeSoftCFM();
    float getOdeLinearDamping();
    float getOdeAngularDamping();
    bool isCompound();

    float* getHeightfieldData(int& xCount,int& yCount,float& minHeight,float& maxHeight);
    int getPurePrimitiveType();
    void getPurePrimitiveSizes(C3Vector& s);
    C7Vector getLocalInertiaFrame();
    C3Vector getPrincipalMomentsOfInertia();

    void buildEquivalentGeometricsAndInsert(bool firstItem,CGeomObject_OLD* geomObj,CGeomWrap* geomWrap);
    void serialize(CSer& ar);

    CVisualParam color;
    CVisualParam insideColor;
    CVisualParam edgeColor;
    int vertStart;
    int vertRange;
    int triStart;
    int triRange;
    int renderingOrder;

    std::vector<CShapeComponent_OLD*> childrenList;

    std::vector<float> _heightfieldHeights;
    int _heightfieldXCount;
    int _heightfieldYCount;

    C7Vector _purePrimitiveConfigurationRelativeToMultishapeFrame;

private:
    bool _visibleEdges;
    bool _culling;
    bool _insideAndOutsideFacesSameColor;
    bool _wireframe;
    int _edgeWidth;
    float _gouraudShadingAngle;
    std::string _name;

    int _purePrimitive;
    float _purePrimitiveXSizeOrDiameter;
    float _purePrimitiveYSize;
    float _purePrimitiveZSizeOrHeight;
    float _mass;

    bool _bulletStickyContact;
    float _bulletRestitution;
    float _bulletFriction;
    float _bulletLinearDamping;
    float _bulletAngularDamping;
    float _bulletNonDefaultCollisionMarginFactor;
    bool _bulletNonDefaultCollisionMargin;

    int _odeMaxContacts;
    float _odeFriction;
    float _odeSoftERP;
    float _odeSoftCFM;
    float _odeLinearDamping;
    float _odeAngularDamping;

    C7Vector _localInertiaFrame;
    C3Vector _principalMomentsOfInertia;
    CTextureProperty* _textureProperty;
    unsigned char _level;
};
