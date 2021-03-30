#include <iostream>
#include <string>
#include <fstream>
#include "Strip2CH.cxx"

using namespace std;

//ROOT class
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
// void main()
// {

// }


void test( string name = "FEE2Slot_Layer1.dat" )
{
    ofstream outFile;
    outFile.open( "map_out_1.dat", ios::out );

    Strip2CH Layer1;
    Layer1.init( name , 15 );
    int FEE, ALTRO, Channel;
    cout << endl;
    cout << "FEE    ALTRO   Channel Strip" << endl;
    outFile << "FEE    ALTRO   Channel Strip" << endl;
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
                // cout << "FEE is " << FEE << " ALTRO is " << ALTRO << " Channel is " << Channel << endl;
                // cout << " #Strip is " << Strip << endl; 
                cout << FEE << "      " << ALTRO << "     " << Channel <<"    " << Strip << endl;
                outFile << FEE << "      " << ALTRO << "     " << Channel <<"    " << Strip << endl;
            }
        }
    } 
    outFile.close();

    Strip2CH Layer2;
    Layer2.init( "FEE2Slot_Layer2.dat" , 15 );
    outFile.open( "map_out_2.dat", ios::out );
    outFile << left << setw(8)<< "FEE"  << " " << left << setw(8) << "ALTRO" << left << setw(8) << "Channel" << left << setw(8) << "Strip" << endl;
    for ( int i = 16; i <= 30; i++ )
    {
        FEE = i;
        for ( int j = 0; j <= 1; j++ )
        {
            ALTRO = FEE*2+j;
            for ( int k = 0; k <= 15; k++ )
            {
                Channel = k;
                int Strip = Layer2.GetStripNumber( FEE, ALTRO, Channel );
                outFile << left << setw(8) <<FEE << " " << left << setw(8) << ALTRO << " " << left << setw(8) << Channel <<" " << left << setw(8) << Strip << endl;
            }
        }
    }
    outFile.close();

}