
#include "ptCloudRendering_old.h"

#ifdef SIM_WITH_OPENGL
void displayPtCloud_old(CPtCloud_old* ptCloud,C3DObject* it)
{
    glPushMatrix();

    C3Vector normalVectorForLinesAndPoints=C3Vector::unitZVector;

    if (it!=NULL)
    {
        C7Vector tr=it->getCumulativeTransformationPart1_forDisplay(true);
        glTranslatef(tr.X(0),tr.X(1),tr.X(2));
        C4Vector axis=tr.Q.getAngleAndAxisNoChecking();
        glRotatef(axis(0)*radToDeg_f,axis(1),axis(2),axis(3));
        normalVectorForLinesAndPoints=(tr.Q.getInverse()*C3Vector::unitZVector);
    }

    glPointSize(ptCloud->_pointSize);

    if (ptCloud->_normals.size()==0)
        ogl::setMaterialColor(ptCloud->_defaultColors,ogl::colorBlack,ptCloud->_defaultColors+12);
    else
        ogl::setMaterialColor(ptCloud->_defaultColors,ptCloud->_defaultColors+8,ptCloud->_defaultColors+12);

    if (ptCloud->_colors.size()==0)
    {
        if (ptCloud->_normals.size()==0)
            ogl::drawRandom3dPointsEx(&ptCloud->_vertices[0],(int)ptCloud->_vertices.size()/3,NULL,NULL,NULL,false,normalVectorForLinesAndPoints.data);
        else
            ogl::drawRandom3dPointsEx(&ptCloud->_vertices[0],(int)ptCloud->_vertices.size()/3,&ptCloud->_normals[0],NULL,NULL,false,normalVectorForLinesAndPoints.data);
    }
    else
    {
        if (ptCloud->_normals.size()==0)
            ogl::drawRandom3dPointsEx(&ptCloud->_vertices[0],(int)ptCloud->_vertices.size()/3,NULL,&ptCloud->_colors[0],NULL,(ptCloud->_options&4)!=0,normalVectorForLinesAndPoints.data);
        else
            ogl::drawRandom3dPointsEx(&ptCloud->_vertices[0],(int)ptCloud->_vertices.size()/3,&ptCloud->_normals[0],&ptCloud->_colors[0],NULL,(ptCloud->_options&4)!=0,normalVectorForLinesAndPoints.data);
    }
    glPointSize(1.0f);

    glPopMatrix();
}

#else

void displayPtCloud_old(CPtCloud_old* ptCloud,C3DObject* it)
{

}

#endif



