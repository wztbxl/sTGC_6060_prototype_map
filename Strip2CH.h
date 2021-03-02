//some rules : #FEE have 2 ALTROs, ALTROs number : #FEE*2 & #FEE*2+1

#ifndef STRIP2CH_H
#define STRIP2CH_H

#include <map>
#include <string>



class Strip2CH
{
private:
    std :: map< int , int > StripGroupLast; 
    std :: map< int , int > FEEs2Slot;
    std :: map< int , int > ALTRO1_2_Strip;
    std :: map< int , int > ALTRO2_2_Strip;
public:
    Strip2CH();
    void init( std :: string filename, int n_FEEs ); // init the last #strip of every strip group, read from file.
    void initFEEs( int* FEEs, int n_FEEs );
    int GetStripNumber( int FEE, int ALTRO, int Channel);
    ~Strip2CH();
};

Strip2CH::Strip2CH()
{
}

Strip2CH::~Strip2CH()
{
}


#endif