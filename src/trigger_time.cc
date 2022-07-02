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

void trigger_time(std::string fileNum, std::string pedNum, enum runMode mode) {

    FILE *fp;
    int file_size;
    int nevt;
    char data[64];
    short adc[32736];

    long long fine_time;
    long long ltmp;
    long long coarse_time;
    long long tcb_trig_time;

    std::cout <<  "[INT] loading data file : "<< DATA_DIR+FILE_NAME+"_"+fileNum+".dat" << std::endl;
    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;

    std::vector<long long> full_time;
    std::vector<long long> normalized_time;
    std::vector<long long> dTime;

    std::vector<double> dfull_time;
    std::vector<double> dnormalized_time;
    std::vector<double> ddTime;
    std::vector<double> xCor;


    long long refTime = 0;
    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    for (int evt = 0; evt < nevt; evt++) {


        fread(data, 1, 64, fp);
        fread(adc, 2, 32736, fp);

        fine_time = data[11] & 0xFF;
        fine_time = fine_time * 11;     // actually * (1000 / 90)
        coarse_time = data[12] & 0xFF;
        ltmp = data[13] & 0xFF;
        ltmp = ltmp << 8;
        coarse_time = coarse_time + ltmp;
        ltmp = data[14] & 0xFF;
        ltmp = ltmp << 16;
        coarse_time = coarse_time + ltmp;
        ltmp = data[15] & 0xFF;
        ltmp = ltmp << 24;
        coarse_time = coarse_time + ltmp;
        ltmp = data[16] & 0xFF;
        ltmp = ltmp << 32;
        coarse_time = coarse_time + ltmp;
        ltmp = data[17] & 0xFF;
        ltmp = ltmp << 40;
        coarse_time = coarse_time + ltmp;
        coarse_time = coarse_time * 1000;   // get ns
        tcb_trig_time = fine_time + coarse_time;

        if ( evt == 0 ) refTime = tcb_trig_time;

        full_time.push_back(tcb_trig_time);
        normalized_time.push_back(tcb_trig_time - refTime);
        if ( evt == 0 ) {
            dTime.push_back(0);
        } else {
            dTime.push_back(tcb_trig_time - full_time.at(evt-1));
        }

        dfull_time.push_back((double)full_time.at(evt)/1000000.);
        dnormalized_time.push_back((double)normalized_time.at(evt)/1000000.);
        ddTime.push_back((double)dTime.at(evt)/1000000.);
        xCor.push_back(evt);


        // std::cout << evt << " " << full_time.at(evt) << " " << normalized_time.at(evt) << " " << dTime.at(evt) << std::endl;
        std::cout << evt << " " << dfull_time.at(evt) << " " << dnormalized_time.at(evt) << " " << ddTime.at(evt) << std::endl;
    }
    fclose(fp);

    TGraph* normTimeGR = new TGraph(xCor.size(), &(xCor[0]), &(dnormalized_time[0]));
    TGraph* dTimeGR = new TGraph(xCor.size(), &(xCor[0]), &(ddTime[0]));

    std::vector<double> x194; x194.push_back(194.); x194.push_back(194.);
    std::vector<double> x146; x146.push_back(146); x146.push_back(146);
    std::vector<double> y194; y194.push_back(0); y194.push_back(300000);
    std::vector<double> y146; y146.push_back(0); y146.push_back(300000);

    TGraph* l194 = new TGraph(x194.size(), &(x194[0]), &(y194[0]));
    l194->SetLineColor(kRed+1); l194->SetLineStyle(4);
    TGraph* l146 = new TGraph(x146.size(), &(x146[0]), &(y146[0]));
    l146->SetLineColor(kBlue+1); l146->SetLineStyle(4);   

    normTimeGR->SetTitle("normalized time;# evt; triggered time [ms]");
    dTimeGR->SetTitle("diff time;# evt; diff time [ms]");


    TCanvas* c = new TCanvas("c", "c", 1100, 1000);

    c->cd();
    normTimeGR->Draw("apl"); l194->Draw("l&sames"); l146->Draw("l&sames");
    c->SaveAs("a_normTime.png");

    dTimeGR->Draw("apl"); l194->Draw("l&sames"); l146->Draw("l&sames");
    c->SaveAs("a_dTime.png"); 

}

