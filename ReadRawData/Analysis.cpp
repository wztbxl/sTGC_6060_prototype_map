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
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

TH1D* hnFireChannel;
TH1D* hnFireTimebin;
TH1D* hADC;
TH1D* hTimeBin;
TH1D* hPedestal[6];
TH2D* hADCvsChannel[6];
TH2D* hChannelvsTimebin[6];
TH2D* hLayerFireMap[3];

void BookHistogram();
void WriteHistrogram();
void GetPedestal( TH1D* hADC_single_channel, double xmin, double xmax );
TH1D* GetPedestal( TH2D* hADC_mult_channel, double xmin, double xmax ); // the 

int Pedestal_tag = 0;

void Analysis( string filename = "stgc_s07_phy01107117_dbg.root", int is_Pedestal = 0 )
{

    gStyle->SetOptFit(11111111);
    BookHistogram();

    TChain* chain = new TChain("sTGC_DataDst");

    chain->Add( filename.data() );

    sTGC_DataDst* event = new sTGC_DataDst(chain);

    int nEvts = chain->GetEntries();
    // nEvts = 1;

    for ( int i = 0; i < nEvts; i++ )
    {
        event->GetEntry(i);

        int RunId = event->mEvtID;
        int nFireChannel = event->nFireChannel;

        hnFireChannel->Fill(nFireChannel);

        int TotalTimebin = 0;
        int nFireTimeBin = 0;
        for ( int j = 0; j < nFireChannel; j++ )
        {
            int Channel_Number = event->Channel_Number[j];
            int Strip_number = event->Strip_Number[j];
            nFireTimeBin = event->nFireTimeBin[j];

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
                    // cout << "i = " << i << " Channel number = " << Channel_Number << " Strip number = " << Strip_number << " Row = " << Row << endl;
                    hADCvsChannel[Row-1]->Fill(Strip,ADC);
                    hLayerFireMap[0]->Fill(Strip,Row,ADC);
                }
                if ( FEE > 15 )
                {
                    hChannelvsTimebin[Row-1+3]->Fill(Strip,Timebin,ADC);
                    cout << "Row = " << Row << endl;
                    hADCvsChannel[Row-1+3]->Fill(Strip,ADC);
                    hLayerFireMap[1]->Fill(Strip,Row,ADC);
                }
                cout << "TimeBin = " << k - TotalTimebin << " ADC = " << ADC << endl; 
                hADC->Fill(ADC);
                hTimeBin->Fill(Timebin);
            }
            TotalTimebin = TotalTimebin+nFireTimeBin;
        }

    }

    if ( is_Pedestal == 1 )
    {
        Pedestal_tag = 1;
        for ( int i = 0; i < 2; i++)
        {
            for ( int j = 0; j < 3; j++ )
            {
                cout << hADCvsChannel[i*3+j] << endl;
                hPedestal[i*3+j] = GetPedestal( hADCvsChannel[i*3+j], 0,200);
            }

        }
    }

    int pos = filename.find(".root");
    filename.replace(pos,string(".root").length(),"_Layer1_test_output.root");
    TFile* outFile = new TFile(filename.data(),"recreate");
    WriteHistrogram();
    

}

void BookHistogram()
{
    hnFireChannel = new TH1D("hnFireChannel","hnFireChannel; nFireStrip; nEvts",188,0,188);
    hnFireTimebin = new TH1D("hnFireTimebin","hnFireTimebin; n_{Timebin}; nEvts",200,0,200);
    hADC = new TH1D("hADC","hADC; ADC; nCounts",1024,0,1024);
    hTimeBin = new TH1D("hTimeBin","hTimeBin;TimeBin;nCounts",400,0,400);
    for ( int j = 0; j <2 ; j++ )
    {
        for ( int i = 0; i < 3; i++ )
        {
            TString name = Form("Channel_vs_TimeBin_Layer%d_Row%d",j,i);
            TString title = Form("Channel_vs_TimeBin_Layer%d_Row%d; Strip; Time bin",j,i);
            hChannelvsTimebin[j*3+i] = new TH2D(name.Data(),title.Data(),199,1,200,100,0,100);
            name = Form("Channel_vs_ADC_Layer%d_Row%d",j,i);
            title = Form("Strip vs ADC Layer%d Row%d; Strip; ADC",j,i);
            hADCvsChannel[j*3+i] = new TH2D( name.Data(), title.Data(), 199,1,200,1024,0,1024);
            hADCvsChannel[j*3+i]->Print("base");
        }
    }
    for ( int i = 0; i < 2; i++ )
    {
        TString name = Form("Fire_Map_layer%d",i);
        TString title = Form("Fire_Map_layer%d ; Strip; Row",i);
        hLayerFireMap[i] = new TH2D(name.Data(), title.Data(), 200,0,200,3,1,4);
        hLayerFireMap[i]->GetZaxis()->SetTitle("Sum ADC");
    }
}

void WriteHistrogram()
{
    hnFireChannel->Write();
    hnFireTimebin->Write();
    hADC->Write();
    hTimeBin->Write();
    for ( int j = 0; j <2 ; j++ )
    {
        for ( int i = 0; i < 3; i++ )
        {
                hChannelvsTimebin[j*3+i]->Write();
                hADCvsChannel[j*3+i]->Write();
                if ( Pedestal_tag == 1 )
                    hPedestal[j*3+i]->Write();
        }
    }
    for ( int i = 0; i < 2; i++ )
    {
        hLayerFireMap[i]->Write();
    }
}

void GetPedestal( TH1D* hADC_single_channel, double xmin, double xmax )
{
    TCanvas* c1 = new TCanvas("c1","c1",1600,1200);
    TF1* f = new TF1("f","gaus",xmin, xmax);
    hADC_single_channel->GetXaxis()->SetRangeUser(0,200);
    hADC_single_channel->Fit("f","R");
    hADC_single_channel->DrawClone("hist");
    f->DrawClone("same");
    std::cout << " mean = " << f->GetParameter(1);
    std::cout << " sigma = " << f->GetParameter(2);
    c1->SaveAs("Pedestal_Fit.png");
}

TH1D* GetPedestal( TH2D* hADC_mult_channel, double xmin, double xmax )
{
    // the TH2D format should be :
    // x axis : # of Strip; y xaixs : ADC distribution in one Strip;

    const int nStrip = hADC_mult_channel->GetNbinsX();
    const int nADCBins = hADC_mult_channel->GetNbinsY();

    TString his_name = hADC_mult_channel->GetName();
    his_name = his_name+"pedestal";
    TH1D* out_his = new TH1D(his_name.Data(),"; Strip; Pedestal",nStrip+9,1,nStrip+10);

    TString name = "mkdir ./plots/";
    system( name.Data() );
    name = name + hADC_mult_channel->GetName() + "/";
    system ( name.Data() );

    TF1* f = new TF1("f","gaus",xmin, xmax);
    for ( int i = 1; i <= nStrip; i++ )
    {
        TH1D* his_tem = (TH1D*)hADC_mult_channel->ProjectionY(Form("Strip_%d",i),i,i);
        his_tem->Fit("f","R");
        TCanvas* can = new TCanvas("tem","tem",1600,1200);
        his_tem->GetXaxis()->SetRangeUser(0,200);
        his_tem->DrawClone("hist");
        f->DrawClone("same");

        // fit method 
        // out_his->SetBinContent(i,f->GetParameter(1));
        // out_his->SetBinError(i,f->GetParameter(2));
        // std::cout << i << "th strip mean = " << f->GetParameter(1) << endl;
        // std::cout << i << "th sigma = " << f->GetParameter(2) << endl;

        // directly get mean and RMS
        out_his->SetBinContent(i,his_tem->GetMean());
        out_his->SetBinError(i,his_tem->GetRMS());
        std::cout << i << "th strip mean = " << his_tem->GetMean() << endl;
        std::cout << i << "th sigma = " << his_tem->GetRMS() << endl;

        TString name1 = "./plots/" + TString(hADC_mult_channel->GetName()) + "/" + Form("Strip_%d.png",i);
        can->SaveAs(name1.Data());
        delete can;
    }

    return out_his;
}

