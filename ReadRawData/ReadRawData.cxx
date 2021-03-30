// work log 
// 2021年 3月 9日 星期二 18时34分44秒 EST
// issue 1 : first  nFireTimeBin is not correct, thinking about how to init
// issue 2 : Timebin and ADC did not work, all of them is 0

// 2021/03/16 try to remove the line of read information line, directly use sTGC line to get the nEvt

#include <iostream>
#include <string>
#include <fstream>
#include "../Strip2CH/Strip2CH.cxx"
#include "StsTGCTreeStructure.h"

using namespace std;

//ROOT class
#include "TRandom3.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"

TTree* mEvtTree;
StsTGCData mEvtData;
int TimeBin;
int ADC;
string FEE, ALTRO, Channel;
string Evt;

void BookTree();

void ReadRawData( string name )
{


    ifstream RawData;

    RawData.open( name , ios::in );

    string line;
    int Strip;
    int nEvts = 0;
    int n_FireChannel = 0;
    int n_FireTimeBin = 0;
    int n_TotalFireTimeBin = 0;

    Strip2CH layer1;
    Strip2CH layer2;

    layer1.init("FEE2Slot_Layer1.dat",15);
    layer2.init("FEE2Slot_Layer2.dat",15);

    BookTree();
    memset(&mEvtData, 0, sizeof(mEvtData));
    while ( RawData.good() && !RawData.eof() )
    {
        getline ( RawData, line );
        if ( !line.size() ) continue;// ignore empty lines

        if ( line[0] == 'S' && line[1] == 'T' ) //
        {
            cout << line.find( "FEE" ) << endl;
            FEE = line.substr( line.find( "FEE" )+3, 2 );
            ALTRO = line.substr( line.find( "ALTRO " )+6, 2 );
            Channel = line.substr( line.find( "):" )+2, 2 );
            int Pos1, Pos2;
            Pos1 = line.find( "evt " )+4;
            Pos2 = line.find( ": sec" );
            Evt = line.substr( Pos1, Pos2 );
            int i_FEE, i_ALTRO, i_Channel, i_Evt;
            i_FEE = stoi(FEE); i_ALTRO = stoi(ALTRO); i_Channel = stoi(Channel); i_Evt = stoi(Evt);
            int number = stoi(FEE)*10000+stoi(ALTRO)*100+stoi(Channel);

            if ( nEvts != i_Evt && nEvts != 0)
            {
                mEvtData.mEventId = nEvts;
                mEvtData.nFireChannel = n_FireChannel;
                for ( int i = 0; i < n_TotalFireTimeBin; i++ )
                {
                    cout << " n_TotalFireTimeBin = " << i << " timebin = " << mEvtData.TimeBin[i] << " ADC = " << mEvtData.ADC[i] << endl;
                }
                cout << "n_FireChannel = " << n_FireChannel << " n_FireTimeBin = " << n_FireTimeBin << endl;
                mEvtData.nFireTimeBin[n_FireChannel-1] = n_FireTimeBin;
                mEvtTree->Fill();

                // cout << "n_FireChannel = " << n_FireChannel << " n_FireTimeBin = " << n_FireTimeBin << " timebin = " << mEvtData.TimeBin[n_FireChannel-1][n_FireTimeBin-1] << " ADC = " << mEvtData.ADC[n_FireChannel][n_FireTimeBin-1] << endl;
                memset(&mEvtData, 0, sizeof(mEvtData));
                n_FireChannel = 0;
                n_TotalFireTimeBin = 0;
            }
            nEvts = i_Evt;
            if ( i_FEE <= 15 ) 
            {
                // int Strip = layer1.GetStripNumber( i_FEE, i_ALTRO, i_Channel );
                int Strip = layer1.GetStripNumber_MirrorFlip( i_FEE, i_ALTRO, i_Channel );
                mEvtData.Strip_Number[n_FireChannel] = Strip;
                cout << left << setw(8) <<i_FEE << " " << left << setw(8) << i_ALTRO << " " << left << setw(8) << i_Channel << endl;
                cout << "Strip = " << Strip << endl;
            }
            else
            {
                int Strip = layer2.GetStripNumber( i_FEE, i_ALTRO, i_Channel );
                mEvtData.Strip_Number[n_FireChannel] = Strip;
                cout << left << setw(8) <<i_FEE << " " << left << setw(8) << i_ALTRO << " " << left << setw(8) << i_Channel << endl;
                cout << "Strip = " << Strip << endl;
            }
            
            mEvtData.Channel_Number[n_FireChannel] = number;
            if (n_FireChannel != 0 )
            mEvtData.nFireTimeBin[n_FireChannel-1] = n_FireTimeBin;

            n_FireChannel++;
            n_FireTimeBin = 0;
            // cout << left << setw(8) <<FEE << " " << left << setw(8) << ALTRO << " " << left << setw(8) << Channel << endl;
            // cout << left << setw(8) <<i_FEE << " " << left << setw(8) << i_ALTRO << " " << left << setw(8) << i_Channel << endl;
            cout << "n_FireChannel = " << n_FireChannel << endl;
        }

        if ( line.size() < 20 )
        {
            istringstream sstr(line);
            sstr >> TimeBin >> ADC ; 
            cout << "timebin = " << TimeBin << " ADC = " << ADC << endl;
            mEvtData.TimeBin[n_TotalFireTimeBin] = TimeBin;
            mEvtData.ADC[n_TotalFireTimeBin] = ADC;
            cout << "n_FireChannel = " << n_FireChannel << " n_FireTimeBin = " << n_TotalFireTimeBin << " timebin = " << mEvtData.TimeBin[n_TotalFireTimeBin] << " ADC = " << mEvtData.ADC[n_TotalFireTimeBin] << endl;
            n_FireTimeBin++;
            n_TotalFireTimeBin++;
        }

    }
    cout << "nEvts = " << nEvts << endl;
    int pos = name.find(".txt");
    name.replace( pos, string(".txt").length(), "_test_newLayer.root");
    TFile* OutFile = new TFile( name.data(), "recreate" );
    mEvtTree->Write();
    
    
    
}

void BookTree()
{
    cout << " Booking the event tree " << endl;
    mEvtTree = new TTree("sTGC_DataDst","sTGC_DataDst");
    mEvtTree->SetAutoSave(100000000); // 100 MB

    // Event information
    mEvtTree->Branch("mEvtID", &mEvtData.mEventId, "mEvtID/I");
    mEvtTree->Branch("nFireChannel", &mEvtData.nFireChannel, "nFireChannel/I");

    // Channel information
    mEvtTree->Branch("Channel_Number", mEvtData.Channel_Number, "Channel_Number[nFireChannel]/I");
    mEvtTree->Branch("Strip_Number", mEvtData.Strip_Number, "Strip_Number[nFireChannel]/I");
    mEvtTree->Branch("nFireTimeBin", mEvtData.nFireTimeBin, "nFireTimeBin[nFireChannel]/I");
    mEvtTree->Branch("TimeBin", mEvtData.TimeBin, "TimeBin[100000]/I");
    mEvtTree->Branch("ADC", mEvtData.ADC, "ADC[100000]/I");
}