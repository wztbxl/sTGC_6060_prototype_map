// 2020.03.12
// all the basic function was added
// 2020.03.21
// A rotation function need to be added, to handle the dismatch between the channel and the strip


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
    // inFile.open( "ALTRO1_2_Strip_Layer1.dat" ); // test for layer 1
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
    // inFile.open( "ALTRO2_2_Strip_Layer1.dat" ); // test for layer 1 
    while (inFile >> FEE_Channel >> Strip_Channel )
    {
        cout << "FEE channel is " << FEE_Channel << " Strip Channel is " << Strip_Channel << endl;
        ALTRO2_2_Strip[FEE_Channel] = Strip_Channel;
    }
    inFile.close();

}

// get the strip number from the FEE ALTRO and channel number from electronics
// must follow this RULE : ALTRO number = FEE*2 or FEE*2+1
int Strip2CH :: GetStripNumber( int FEE, int ALTRO, int Channel )
{
    int GroupLastStrip = StripGroupLast[ FEEs2Slot[ FEE ] ];
    map < int , int > Maps;
    if ( ALTRO == FEE*2 ) Maps = ALTRO1_2_Strip;
    if ( ALTRO == FEE*2+1 ) Maps = ALTRO2_2_Strip;

    int Strip_number = GroupLastStrip-Maps[Channel]+1;
    return Strip_number;
}

// 60*60 prototype layer1, there is a mirror flip between the layer 1 and layer 2,
// the map needs to be modified.
// this function is used to correct the difference between the layer 1 and layer 2
int Strip2CH :: GetStripNumber_MirrorFlip( int FEE, int ALTRO, int Channel )
{
    int GroupLastStrip = StripGroupLast[ FEEs2Slot[ FEE ] ];
    map < int , int > Maps;
    if ( ALTRO == FEE*2 ) Maps = ALTRO2_2_Strip;
    if ( ALTRO == FEE*2+1 ) Maps = ALTRO1_2_Strip;

    int Strip_number;
    if ( Channel <= 7 ) Strip_number = GroupLastStrip-Maps[Channel+8]+1;
    if ( Channel >= 8 ) Strip_number = GroupLastStrip-Maps[Channel-8]+1;
    return Strip_number;
}