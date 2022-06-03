#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <TH1F.h>
#include <TString.h>


void full_ped(std::string fileNum, std::string pedNum) {

    gStyle->SetOptFit(1);

    std::vector<TH1F*> histograms;
    std::vector<TF1*> fit_func;
    for ( int ch = 0; ch < 32; ch++ ) {
        TH1F* tmpPlot = new TH1F( ("ch"+std::to_string(ch)).c_str(), ("ch"+std::to_string(ch)+";ADC;evts").c_str(), 4096, 0., 4095.);
        histograms.push_back(tmpPlot);
        histograms.at(ch)->SetLineWidth(2); 
        histograms.at(ch)->SetLineColor(634); 
        histograms.at(ch)->Sumw2(); 
    }


    FILE *fp;
    int file_size;
    int nevt;
    char data[64];
    short adc[32736];

    std::cout << "[PED] loading data file : " << DATA_DIR+FILE_NAME+"_"+fileNum+".dat" << std::endl;

    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;

    fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
    for (int evt = 0; evt < nevt; evt++) {
        fread(data, 1, 64, fp);
        fread(adc, 2, 32736, fp);

        for (int ch=0; ch<32; ch++){
            for (int bin = 1; bin < 1001; bin++) {
                histograms.at(ch)->Fill( (float)adc[bin * 32 + ch] );
            }
        }
    }
    fclose(fp);

    std::ofstream ped_file(BASE_DIR+"ped/ped_"+FILE_NAME+"_"+pedNum+".csv");
    for (int ch = 0; ch < 32; ch++) {
        std::cout << std::endl << "ch " << ch << " fit result" << std::endl;
        TF1* tmpFit = new TF1( ("Fit"+std::to_string(ch)).c_str() , "gaus", 0., 4095.); tmpFit->SetLineColor(kBlue);
        histograms.at(ch)->SetOption("p"); histograms.at(ch)->Fit(tmpFit, "R+&sames");

        std::cout << std::endl;
        fit_func.push_back(tmpFit);

        ped_file << std::to_string(ch) << " " << std::to_string(tmpFit->GetParameter(1)) << "\n";
    }

    TFile* validFile = new TFile((TString)(BASE_DIR+"ped/validFiles/ped_"+FILE_NAME+"_"+pedNum+"_validation.root"), "RECREATE");
    for (int ch = 0; ch < 32; ch++) {
        validFile->WriteTObject(histograms.at(ch));
        validFile->WriteTObject(fit_func.at(ch));
    }
    validFile->Close();

    ped_file.close();
}