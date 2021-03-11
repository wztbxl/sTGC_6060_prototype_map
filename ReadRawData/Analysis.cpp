#include <cstdio> 
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "sys/types.h"
#include "dirent.h"
#include "math.h"
#include "string.h"

#include "sTGC_DataDst.h"

//ROOT class
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

TH1D* hnFireChannel;
TH1D* hnFireTimebin;
TH2D* hChannelvsTimebin[3];
TH2D* hLayerFireMap[3];

void BookHistogram();

void Analysis( String name = "stgc_s07_phy01107117_dbg.root" )
{
    TChain* chain = new TChain("sTGC_DataDst");

    chain->Add( name.data() );

    sTGC_DataDst* event = new sTGC_DataDst(chain);

    int nEvts = chain->GetEntries();

    for ( int i = 0; i < nEvts; i++ )
    {
        event->GetEntry(i);

        int RunId = event->mEvtID;
        int nFireChannel = event->nFireChannel;
        int TotalTimebin = 0;
        for ( int j = 0; j < nFireChannel; j++ )
        {
            int Channel_Number = event->Channel_Number[j];
            int Strip_number = event->Strip_Number[j];
            int nFireTimeBin = event->nFireTimeBin[j];

            
        }

    }

}

void BookHistogram()
{
    hnFireChannel = new TH1D("hnFireChannel","hnFireChannel; nFireChannel; nEvts",188,0,188);
    hnFireTimebin = new TH1D("hnFireTimebin","hnFireTimebin; n_{Timebin}; nEvts",200,0,200);
    for ( int i = 0; i < 3; i++ )
    {
        TString name = Form("Channel_vs_TimeBin_Row%d",i);
        TString title = Form("Channel_vs_TimeBin_Row%d; Channel; Time bin",i);
        hChannelvsTimebin[i] = new TH2D(name.Data(),title.Data(),188,0,188,100,0,100);
    }
    hChannelvsTimebin[3];
    hLayerFireMap[2];
}