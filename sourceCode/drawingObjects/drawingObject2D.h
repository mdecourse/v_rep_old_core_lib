
#pragma once

#include "vrepMainHeader.h"

enum { // Keep values and order!! Is serialized!!! (well, not really, but for compatibility reasons!)
    sim_drawing2d_plus=0,
    sim_drawing2d_cross,
    sim_drawing2d_frame,
    sim_drawing2d_square,
    sim_drawing2d_circle,
    sim_drawing2d_disc,
    sim_drawing2d_arrow,
    sim_drawing2d_line,
    sim_drawing2d_triangle,
    sim_drawing2d_rectangle,
    sim_drawing2d_text,

    sim_drawing2d_blackbkground =0x00000100,
    sim_drawing2d_pixelsize     =0x00000200,
};


class CDrawingObject2D  
{
public:
    CDrawingObject2D(int theObjectType,float* vertices,float col[3],float size,float dir,char* txt);
    virtual ~CDrawingObject2D();
    void draw(int minViewPos[2],int maxViewPos[2]);

    int _objectType;
    float _size;
    float _vertices[6];
    float _col[3];
    float _dir;
    std::string _txt;
};
