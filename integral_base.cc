#include <cstdio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <TPad.h>
#include <TFile.h>
#include <utility>
#include <iostream>
#include <TH1.h>


void pad_set(TPad* tPad) {
    tPad->Draw();
    tPad->cd();

    tPad->SetTopMargin(0.08);
    tPad->SetLeftMargin(0.08);    
    tPad->SetRightMargin(0.08);
    tPad->SetBottomMargin(0.08);
}

void integral_base(std::string fileNum, std::string pedNum, enum runMode mode) {

    FILE *fp;
    int file_size;
    int nevt;
    char data[64];
    short adc[32736];

    std::vector<TH1F*> plotCol;

    for ( int i = 0; i < chInput.size(); i++ ) {
        TH1F* plot = new TH1F((TString)("ch"+std::to_string(chInput.at(i))), (TString)(plotName.at(i)+";#font[42]{Integral(ADC)/100};#font[42]{evts}"), 320, -1024., 4095.);

        plotCol.push_back(plot);
        plotCol.at(i)->SetLineWidth(2); 
        plotCol.at(i)->SetLineColor(plotColor.at(i)); 
        plotCol.at(i)->Sumw2(); 
        plotCol.at(i)->GetYaxis()->SetTitle( "" ); 
    }

    std::cout <<  "[INT] loading data file : "<< DATA_DIR+FILE_NAME+"_"+fileNum+".dat" << std::endl;
    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;

    std::vector<float> chPed;
    for (int i = 0; i < 32; i++) chPed.push_back(0);

    if (mode == runMode::kFull) {
        std::ifstream in;
        int i_tmp; float ped_tmp;
        in.open(BASE_DIR+"ped/ped_"+FILE_NAME+"_"+pedNum+".csv",std::ios::in);
        std::cout << "[FULL] loading ped file : " << BASE_DIR+"ped/ped_"+FILE_NAME+"_"+pedNum+".csv" << std::endl;

        while (true) {
            in >> i_tmp >> ped_tmp;
            if (!in.good()) break;
            chPed.at(i_tmp) = ped_tmp;
        }
        in.close();
    }

    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    for (int evt = 0; evt < nevt; evt++) {
        fread(data, 1, 64, fp);
        fread(adc, 2, 32736, fp);

        std::vector< std::vector<int> > waveformCol;
        for (int i = 0; i < chInput.size(); i++ ) {
            std::vector<int> tmpVec;
            waveformCol.push_back(tmpVec);
        }

        if (mode == runMode::kPrompt) {
            for (int i = 0; i < chPed.size(); i++) chPed.at(i) = 0;
            float pedNbin = 50.;
            for (int promptPed = 1; promptPed < pedNbin + 1; promptPed++) {
                for (int chInt = 0; chInt < chInput.size(); chInt++ ) {
                    chPed.at(chInput.at(chInt)) += (float)adc[promptPed * 32 + chInput.at(chInt)] / pedNbin;
                }
            }
        }
        
        for (int i = 1; i < 1001; i++) {
            for (int chInt = 0; chInt < chInput.size(); chInt++ ) {
                waveformCol.at(chInt).push_back(chPed.at(chInput.at(chInt)) - adc[i * 32 + chInput.at(chInt)]);
            }
        }

        for (int chInt = 0; chInt < chInput.size(); chInt++ ) {
            plotCol.at(chInt)->Fill((std::accumulate(waveformCol.at(chInt).begin(), waveformCol.at(chInt).end(), 0.f) / 100.));
        }        
    }
    fclose(fp);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000);

    c->cd();
    TPad* pad_LB = new TPad("pad_LB","pad_LB", 0.01, 0.01, 0.5, 0.5 );
    pad_set(pad_LB);

    c->cd();
    TPad* pad_RB = new TPad("pad_RB","pad_RB", 0.5, 0.01, 0.99, 0.5 );
    pad_set(pad_RB);

    c->cd();
    TPad* pad_LT = new TPad("pad_LT","pad_LT", 0.01, 0.5, 0.5, 0.99 );
    pad_set(pad_LT);

    c->cd();
    TPad* pad_RT = new TPad("pad_RT","pad_RT", 0.5, 0.5, 0.99, 0.99 );
    pad_set(pad_RT);

    c->cd(); pad_LT->cd(); plotCol.at(0)->Draw("Hist");
    c->cd(); pad_RT->cd(); plotCol.at(1)->Draw("Hist");
    c->cd(); pad_LB->cd(); plotCol.at(2)->Draw("Hist");
    c->cd(); pad_RB->cd(); plotCol.at(3)->Draw("Hist");

    if ( mode == runMode::kFull ) {
        c->SaveAs(BASE_DIR+"plots/Integral/png/"+FILE_NAME+"_"+(TString)fileNum+"_full.png");
        c->SaveAs(BASE_DIR+"plots/Integral/svg/"+FILE_NAME+"_"+(TString)fileNum+"_full.svg");
    }
    if ( mode == runMode::kPrompt ) {
        c->SaveAs(BASE_DIR+"plots/Integral/png/"+FILE_NAME+"_"+(TString)fileNum+"_prompt.png");
        c->SaveAs(BASE_DIR+"plots/Integral/svg/"+FILE_NAME+"_"+(TString)fileNum+"_prompt.svg");
    }

    c->cd(); pad_LT->cd();
    TLatex cmspreLatex; 
    cmspreLatex.DrawLatexNDC(0.087, 0.93, "#font[62]{DRC}#font[42]{#it{#scale[0.8]{ Internal}}}");

    TFile* validFile = new TFile((TString)(BASE_DIR+"validFiles/Integral/"+FILE_NAME+"_"+(TString)fileNum+"_validation_"+std::to_string(mode)+".root"), "RECREATE");
    validFile->WriteTObject(plotCol.at(0));
    validFile->WriteTObject(plotCol.at(1));
    validFile->WriteTObject(plotCol.at(2));
    validFile->WriteTObject(plotCol.at(3));
    validFile->Close();
}

