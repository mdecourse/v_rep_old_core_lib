
#pragma once

class CSer;

class CSerBase
{
public:
    CSerBase(){}
    virtual ~CSerBase(){}

    void handleVerSpecConstructor1(CSer* ser){}
    void handleVerSpecConstructor2(CSer* ser){}
    bool handleVerSpecWriteClose1(bool compress){return compress;}
    void handleVerSpecWriteClose2(CSer* ser){}
    void handleVerSpecReadOpen(CSer* ser){}
};
