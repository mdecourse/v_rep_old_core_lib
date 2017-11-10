
#include "wThread.h"

unsigned int(*Thread::startAddress)(void*)=NULL;
volatile bool Thread::startAddressIsFree=false;

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::run()
{
    connect(this,SIGNAL(finished()),this,SLOT(threadFinished()));
    unsigned int(*startAddressAux)(void*);
    startAddressAux=startAddress;
    startAddressIsFree=true;
    startAddressAux(NULL);
}

void Thread::threadFinished()
{
    delete this;
}
