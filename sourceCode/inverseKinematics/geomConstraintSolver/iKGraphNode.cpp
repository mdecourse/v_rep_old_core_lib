
#include "vrepMainHeader.h"
#include "iKGraphNode.h"

CIKGraphNode::CIKGraphNode()
{
    elementID=-1;
    explorationID=-1;
    userData0=-1;
    userData1=-1;
}

CIKGraphNode::~CIKGraphNode()
{
}

CIKGraphNode* CIKGraphNode::getUnexplored(int pos)
{
    return(NULL);
}

CIKGraphNode* CIKGraphNode::getNeighbour(int pos,bool noLinkNeighbour)
{
    return(NULL);
}

int CIKGraphNode::getNumberOfUnexplored()
{   
    return(0);
}

CIKGraphNode* CIKGraphNode::getNeighbourWithExplorationID(int theID)
{
    return(NULL);
}


CIKGraphNode* CIKGraphNode::getExploredWithSmallestExplorationID()
{
    return(NULL);
}

int CIKGraphNode::getConnectionNumber()
{
    return(0);
}
