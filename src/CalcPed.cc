#include <cstdio>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <TH1F.h>
#include <TString.h>


void CalcPed(std::string fileNum) {

    gStyle->SetOptFit(1);

    FILE *fp;
    int file_size;
    int nevt;
    char data[64];
    short adc[32736];

    TCanvas* c1 = new TCanvas("c1", "c1", 500, 500);

    std::vector<std::vector<TH1F*>> promptPedHist;
    for (int j = 0; j < NofDAQModule; j++) {
        std::vector<TH1F*> tmpPlotCol; 
        promptPedHist.push_back(tmpPlotCol);
        for ( int i = 0; i < chInput.at(j).size(); i++ ) {
            TH1F* tmpPedHist = new TH1F((TString)("mid"+std::to_string(j+1)+" ch"+std::to_string(chInput.at(j).at(i))+" Prompt Ped"), 
                                  (TString)(";#font[42]{ADC};#font[42]{# evts}"), 
                                  4096, 0., 4095.);

            promptPedHist.at(j).push_back(tmpPedHist);
            promptPedHist.at(j).at(i)->SetLineWidth(1); 
            promptPedHist.at(j).at(i)->SetLineColor(1);
            promptPedHist.at(j).at(i)->Sumw2(); 
            promptPedHist.at(j).at(i)->SetStats(0);
        }
    }

    std::vector<std::vector<TH1F*>> fullPedHist;
    for (int j = 0; j < NofDAQModule; j++) {
        std::vector<TH1F*> tmpPlotCol; 
        fullPedHist.push_back(tmpPlotCol);
        for ( int i = 0; i < chInput.at(j).size(); i++ ) {
            TH1F* tmpPedHist = new TH1F((TString)("mid"+std::to_string(j+1)+" ch"+std::to_string(chInput.at(j).at(i))+" Full Ped"), 
                                  (TString)(";#font[42]{ADC};#font[42]{# evts}"), 
                                  4096, 0., 4095.);

            fullPedHist.at(j).push_back(tmpPedHist);
            fullPedHist.at(j).at(i)->SetLineWidth(1); 
            fullPedHist.at(j).at(i)->SetLineColor(1);
            fullPedHist.at(j).at(i)->Sumw2(); 
            fullPedHist.at(j).at(i)->SetStats(0);
        }
    }

    for (int j = 0; j < NofDAQModule; j++) { 
        std::cout << "[PED] loading data file : " << DATA_DIR+FILE_NAME+"_"+std::to_string(NofDAQModule)+"_"+fileNum+".dat" << std::endl;

        fp = fopen((DATA_DIR+FILE_NAME+"_"+std::to_string(NofDAQModule)+"_"+fileNum+".dat").c_str(), "rb");
        fseek(fp, 0L, SEEK_END);
        file_size = ftell(fp);
        fclose(fp);
        nevt = file_size / 65536;


        fp = fopen((DATA_DIR+FILE_NAME+"_"+std::to_string(NofDAQModule)+"_"+fileNum+".dat").c_str(), "rb");
        for (int evt = 0; evt < nevt; evt++) {
            fread(data, 1, 64, fp);
            fread(adc, 2, 32736, fp);

            float pedNbin = 50.;
            for (int promptPed = 1; promptPed < 1001; promptPed++) {
                for (int chInt = 0; chInt < chInput.at(j).size(); chInt++ ) {
                    if (promptPed <= pedNbin) promptPedHist.at(j).at(chInt)->Fill( (float)adc[promptPed * 32 + chInput.at(j).at(chInt)] );
                    fullPedHist.at(j).at(chInt)->Fill( (float)adc[promptPed * 32 + chInput.at(j).at(chInt)] );
                }
            }
        }
                     
        std::ofstream Fullped_file((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Ped/Full/"+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+".csv"));
        for ( int i = 0; i < chInput.at(j).size(); i++ ) {
            TF1* tmpFitFull = new TF1( (TString)("Full Ped mid"+std::to_string(j+1)+" ch"+std::to_string(chInput.at(j).at(i))) , "gaus", 3000., 4000.); 
            tmpFitFull->SetLineColor(kBlue);
            fullPedHist.at(j).at(i)->SetOption("p"); fullPedHist.at(j).at(i)->Fit(tmpFitFull, "R+&sames"); 
            Fullped_file << std::to_string(chInput.at(j).at(i)) << " " << std::to_string(tmpFitFull->GetParameter(1)) << "\n";

            c1->cd();  fullPedHist.at(j).at(i)->Draw();
            c1->SaveAs((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Ped/Full/"+std::to_string(j+1)+"/"+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+"_ch"+std::to_string(chInput.at(j).at(i))+".png"));
        }
        Fullped_file.close();

        std::ofstream Promptped_file((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Ped/Prompt/"+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+".csv"));
        for ( int i = 0; i < chInput.at(j).size(); i++ ) {
            TF1* tmpFitPrompt = new TF1( (TString)("Promprt Ped mid"+std::to_string(j+1)+" ch"+std::to_string(chInput.at(j).at(i))) , "gaus", 3000., 4000.); 
            tmpFitPrompt->SetLineColor(kBlue);
            promptPedHist.at(j).at(i)->SetOption("p"); promptPedHist.at(j).at(i)->Fit(tmpFitPrompt, "R+&sames"); 
            Promptped_file << std::to_string(chInput.at(j).at(i)) << " " << std::to_string(tmpFitPrompt->GetParameter(1)) << "\n";

            c1->cd();  promptPedHist.at(j).at(i)->Draw();
            c1->SaveAs((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Ped/Prompt/"+std::to_string(j+1)+"/"+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+"_ch"+std::to_string(chInput.at(j).at(i))+".png"));
        }
        Promptped_file.close();   

        fclose(fp);
    }
}
