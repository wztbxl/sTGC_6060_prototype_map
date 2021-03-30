// work log 
// 2021年 3月 9日 星期二 18时34分44秒 EST
// issue 1 : first  nFireTimeBin is not correct, thinking about how to init
// issue 2 : Timebin and ADC did not work, all of them is 0

// 2021/03/16 try to remove the line of read information line, directly use sTGC line to get the nEvt

#include <iostream>
#include <string>
#include <fstream>
// #include "../Strip2CH/Strip2CH.cxx"
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
string TimeBin;
string ADC;
string FEB, Sec, RDO;
string VMM, Channel,Channel_str, ADC_str, BCID;
string Evt;
int i_Evt, i_Sec, i_RDO;


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

    BookTree();
    memset(&mEvtData, 0, sizeof(mEvtData));

    while ( RawData.good() && !RawData.eof() )
    {
        getline ( RawData, line );
        if ( !line.size() ) continue;// ignore empty lines

        if ( line[0] == 'S' && line[1] == 'T' ) //
        {
            Evt = line.substr( line.find("evt ")+4, line.find(": sec") );
            i_Evt = stoi(Evt); 
            if ( nEvts != i_Evt && nEvts != 0)
            {
                mEvtData.mEventId = nEvts;
                mEvtData.nFireChannel = n_FireChannel;
                mEvtData.Sec = i_Sec;
                mEvtData.RDO = i_RDO;
                mEvtTree->Fill();

                memset(&mEvtData, 0, sizeof(mEvtData));
                n_FireChannel = 0;
            }
            Sec = line.substr( line.find("sec ")+4, line.find(", RDO") );
            RDO = line.substr( line.find("RDO ")+4, 1 );
            i_Sec = stoi(Sec); i_RDO = stoi(RDO);
            nEvts = i_Evt;
        }
        if ( line[2] == 'F' && line[3] == 'E' )
        {
            istringstream sstr(line);
            sstr >> FEB >> VMM >> Channel_str >> Channel >> ADC_str >> ADC >> BCID >> TimeBin ;
            // cout << "FEB = " << FEB << " VMM = " << VMM << " Channel_str = " << Channel_str <<" Channel = " << Channel 
            //      << " ADC_str = " << ADC_str << " ADC = " << ADC << " BCID = " << BCID 
            //      << " Timebin = " << TimeBin << endl;
            int i_FEB, i_VMM, i_ADC, i_BCID, i_Channel;
            i_FEB = stoi( VMM.substr(0,VMM.find(":")) );
            i_VMM = stoi( VMM.substr(VMM.find(":")+1,VMM.length()) );
            i_Channel = stoi( Channel.substr(0,Channel.find(":")) );
            i_ADC = stoi( ADC.substr(0,ADC.find(",")) );
            i_BCID = stoi(TimeBin);

            mEvtData.ADC[n_FireChannel] = i_ADC;
            mEvtData.BCID[n_FireChannel] = i_BCID;
            mEvtData.FEB[n_FireChannel] = i_FEB;
            mEvtData.VMM[n_FireChannel] = i_VMM;
            mEvtData.Channel[n_FireChannel] = i_Channel;

            n_FireChannel++;
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
    mEvtTree->Branch("FEB", mEvtData.FEB, "FEB[nFireChannel]/I");
    mEvtTree->Branch("VMM", mEvtData.VMM, "VMM[nFireChannel]/I");
    mEvtTree->Branch("Channel", mEvtData.Channel, "Channel[nFireChannel]/I");
    mEvtTree->Branch("BCID", mEvtData.BCID, "BCID[nFireChannel]/I");
    mEvtTree->Branch("ADC", mEvtData.ADC, "ADC[nFireChannel]/I");
}