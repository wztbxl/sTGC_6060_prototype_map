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

#include "sTGC_DataDst.cpp"

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
TH2D* hChannelvsTimebin[6];
TH2D* hLayerFireMap[3];

void BookHistogram();
void WriteHistrogram();

void Analysis( string filename = "stgc_s07_phy01107117_dbg.root" )
{
    BookHistogram();

    TChain* chain = new TChain("sTGC_DataDst");

    chain->Add( filename.data() );

    sTGC_DataDst* event = new sTGC_DataDst(chain);

    int nEvts = chain->GetEntries();

    for ( int i = 0; i < nEvts; i++ )
    {
        event->GetEntry(i);

        int RunId = event->mEvtID;
        int nFireChannel = event->nFireChannel;

        hnFireChannel->Fill(nFireChannel);

        int TotalTimebin = 0;
        for ( int j = 0; j < nFireChannel; j++ )
        {
            int Channel_Number = event->Channel_Number[j];
            int Strip_number = event->Strip_Number[j];
            int nFireTimeBin = event->nFireTimeBin[j];

            int FEE, ALTRO, Channel, Strip, Row;
            FEE = Channel_Number/10000;
            ALTRO = Channel_Number/100%100;
            Channel = Channel_Number%100;
            Row = Strip_number/1000;
            Strip = Strip_number%1000;

            hnFireTimebin->Fill(nFireTimeBin);
            for ( int k = TotalTimebin; k < TotalTimebin+nFireTimeBin; k++ )
            {
                int Timebin = event->TimeBin[k];
                int ADC = event->ADC[k];
                if (FEE <= 15)
                {
                    hChannelvsTimebin[Row-1]->Fill(Strip,Timebin,ADC);
                    hLayerFireMap[0]->Fill(Strip,Row,ADC);
                }
                if ( FEE > 15 )
                {
                    hChannelvsTimebin[Row-1+3]->Fill(Strip,Timebin,ADC);
                    hLayerFireMap[1]->Fill(Strip,Row,ADC);
                }
            }
        }

    }

    TFile* outFile = new TFile("outFile.root","recreate");
    WriteHistrogram();

}

void BookHistogram()
{
    hnFireChannel = new TH1D("hnFireChannel","hnFireChannel; nFireStrip; nEvts",188,0,188);
    hnFireTimebin = new TH1D("hnFireTimebin","hnFireTimebin; n_{Timebin}; nEvts",200,0,200);
    for ( int j = 0; j <2 ; j++ )
    {
        for ( int i = 0; i < 3; i++ )
        {
            TString name = Form("Channel_vs_TimeBin_Layer%d_Row%d",j,i);
            TString title = Form("Channel_vs_TimeBin_Layer%d_Row%d; Strip; Time bin",j,i);
            hChannelvsTimebin[j*3+i] = new TH2D(name.Data(),title.Data(),188,0,188,100,0,100);
        }
    }
    for ( int i = 0; i < 2; i++ )
    {
        TString name = Form("Fire_Map_layer%d",i);
        TString title = Form("Fire_Map_layer%d ; Strip; Row",i);
        hLayerFireMap[i] = new TH2D(name.Data(), title.Data(), 188,0,188,3,1,4);
    }
}

void WriteHistrogram()
{
    hnFireChannel->Write();
    hnFireTimebin->Write();
    for ( int j = 0; j <2 ; j++ )
    {
        for ( int i = 0; i < 3; i++ )
        {
                hChannelvsTimebin[j*3+i]->Write();
        }
    }
    for ( int i = 0; i < 2; i++ )
    {
        hLayerFireMap[i]->Write();
    }
}