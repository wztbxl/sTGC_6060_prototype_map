#include "Strip2CH.h"

#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

void Strip2CH :: init( string filename, int n_FEEs)
{
    // initialize number of last strip of one strip group
    ifstream inFile;
    inFile.open( "./LastStrip.dat");
    int LastStrip = 0;
    int nGroup = 1;
    while ( inFile >> LastStrip)
    {
        cout << LastStrip << endl;
        StripGroupLast[nGroup] = LastStrip;
        nGroup++;
    }
    inFile.close();

    // initialize FEEs to Slots
    inFile.open( filename );
    nGroup = 1;
    int FEEnumber = 0;
    while (inFile >> FEEnumber)
    {
        cout << FEEnumber << endl;
        FEEs2Slot[ FEEnumber ] = nGroup;
        nGroup++;
        if (nGroup > n_FEEs) break;
    }
    inFile.close();

    // initialize FEE channel to Strip channel
    inFile.open( "ALTRO1_2_Strip.dat" );
    int FEE_Channel;
    int Strip_Channel;
    while (inFile >> FEE_Channel >> Strip_Channel )
    {
        cout << "FEE channel is " << FEE_Channel << " Strip Channel is " << Strip_Channel << endl;
        ALTRO1_2_Strip[FEE_Channel] = Strip_Channel;
    }
    inFile.close();

    // initialize FEE channel to Strip channel
    inFile.open( "ALTRO2_2_Strip.dat" );
    while (inFile >> FEE_Channel >> Strip_Channel )
    {
        cout << "FEE channel is " << FEE_Channel << " Strip Channel is " << Strip_Channel << endl;
        ALTRO2_2_Strip[FEE_Channel] = Strip_Channel;
    }
    inFile.close();

}

int Strip2CH :: GetStripNumber( int FEE, int ALTRO, int Channel )
{
    int GroupLastStrip = StripGroupLast[ FEEs2Slot[ FEE ] ];
    map < int , int > Maps;
    if ( ALTRO == FEE*2 ) Maps = ALTRO1_2_Strip;
    if ( ALTRO == FEE*2+1 ) Maps = ALTRO2_2_Strip;

    int Strip_number = GroupLastStrip-ALTRO2_2_Strip[Channel]+1;
    return Strip_number;
}