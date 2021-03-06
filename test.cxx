#include <iostream>
#include <string>
#include "Strip2CH.cxx"

using namespace std;

//ROOT class
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
// void main()
// {

// }

void test( string name = "FEE2Slot_Layer1.dat" )
{
    Strip2CH Layer1;
    Layer1.init( name , 15 );
    int FEE, ALTRO, Channel;
    for ( int i = 0; i < 10; i++)
    {
        // FEE = (int)gRandom->Uniform(1,16);
        // ALTRO = FEE*2 + (int)gRandom->Uniform(0.5,1.5);
        // Channel = (int)gRandom->Uniform(1,16);
        for ( int i = 1; i <= 15; i++ )
        {
            FEE = i;
            for ( int j = 0; j <= 1; j++ )
            {
                ALTRO = FEE*2+j;
                for ( int k = 0; k <= 15; k++ )
                {
                    Channel = k;
                    int Strip = Layer1.GetStripNumber(FEE,ALTRO,Channel);
                    cout << "FEE is " << FEE << " ALTRO is " << ALTRO << " Channel is " << Channel << endl;
                    cout << " #Strip is " << Strip << endl; 
                }
            }
        } 
    }
    

}