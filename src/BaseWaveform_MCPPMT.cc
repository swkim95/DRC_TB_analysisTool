#include <stdio.h>
#include <chrono>
#include <thread>
#include <TPad.h>
#include <string>
// #include <Windows.h>

void pad_set(TPad* tPad) {
    tPad->Draw();
    tPad->cd();

    tPad->SetTopMargin(0.08);
    tPad->SetLeftMargin(0.12);    
    tPad->SetRightMargin(0.08);
    tPad->SetBottomMargin(0.08);
}


int BaseWaveform_MCPPMT(std::string fileNum)
{
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char data[64];
  short adc[32736];

  long long fine_time;
  long long ltmp;
  long long coarse_time;
  long long tcb_trig_time;
  long long refTime = 0;

  std::vector<long long> full_time;
  std::vector<long long> normalized_time;
  std::vector<long long> dTime;

  std::vector<double> dfull_time;
  std::vector<double> dnormalized_time;
  std::vector<double> ddTime;
  std::vector<double> xCor;

  TCanvas* c = new TCanvas("c", "c", 1000, 500);
  TCanvas* c1 = new TCanvas("c1", "c1", 500, 500);
  TCanvas* c2 = new TCanvas("c2", "c2", 1000, 500);

  c->cd();
  TPad* pad_LT = new TPad("pad_LT","pad_LT", 0.01, 0.01, 0.5, 0.99 );
  pad_set(pad_LT);

  c->cd();
  TPad* pad_RT = new TPad("pad_RT","pad_RT", 0.5, 0.01, 0.99, 0.99 );
  pad_set(pad_RT);

  c2->cd();
  TPad* pad_TL = new TPad("pad_TL","pad_TL", 0.01, 0.01, 0.5, 0.99 );
  pad_set(pad_TL);

  c2->cd();
  TPad* pad_TR = new TPad("pad_TR","pad_TR", 0.5, 0.01, 0.99, 0.99 );
  pad_set(pad_TR);


  std::vector<std::vector<TH1F*>> plotCol;

  for (int j = 0; j < NofDAQModule; j++) {
    std::vector<TH1F*> tmpPlotCol; 
    plotCol.push_back(tmpPlotCol);
    for ( int i = 0; i < chInput.at(j).size(); i++ ) {

      plotCol.at(j).push_back(
                              new TH1F((TString)("mid"+std::to_string(j+1)+" ch"+std::to_string(chInput.at(j).at(i))), 
                                       (TString)(";ns;ADC"), 
                                       1000, 0, 200
                                      )
                             );

      plotCol.at(j).at(i)->SetLineWidth(1); 
      plotCol.at(j).at(i)->SetLineColor(plotColor.at(j).at(i)); 
      plotCol.at(j).at(i)->GetYaxis()->SetRangeUser(0, 4096);
      plotCol.at(j).at(i)->Sumw2(); 
      plotCol.at(j).at(i)->SetStats(0);
    }
  }

  for (int j = 0; j < NofDAQModule; j++) { 
    std::cout <<  "[INT] loading data file : "<< DATA_DIR+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+".dat" << std::endl;

    fp = fopen((DATA_DIR+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+".dat").c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fclose(fp);
    nevt = file_size / 65536;

    fp = fopen((DATA_DIR+FILE_NAME+"_"+std::to_string(j+1)+"_"+fileNum+".dat").c_str(), "rb");
    for (int evt = 0; evt < nevt; evt++) {
      for ( int xx = 0; xx < plotCol.at(j).size(); xx++ ) plotCol.at(j).at(xx)->Reset();

      fread(data, 1, 64, fp);
      fread(adc, 2, 32736, fp);

      if (j == 0) {
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
      }


      for (int i = 0; i < 1000; i++) {
        for (int t = 0; t < chInput.at(j).size(); t++) {
          plotCol.at(j).at(t)->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(j).at(t)]); 
        }
      }

      if ( j == 0 ) {
        for (int i = 0; i < 8; i++) {
          if ( i == 0 ) {
              c->cd(); pad_LT->cd(); plotCol.at(j).at(i     )->Draw("Hist");
              c->cd(); pad_RT->cd(); plotCol.at(j).at(i + 8 )->Draw("Hist");
          } else {
              c->cd(); pad_LT->cd(); plotCol.at(j).at(i     )->Draw("Hist&sames");
              c->cd(); pad_RT->cd(); plotCol.at(j).at(i + 8 )->Draw("Hist&sames");
          }
        }
        c->SaveAs((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Waveform/"+std::to_string(j+1)+"/"+FILE_NAME+"_mid"+std::to_string(j+1)+"_"+std::to_string(evt)+"evt_waveform.png"));
      }

      if ( j == 1 ) {
        for (int i = 0; i < chInput.at(j).size(); i++) {
          c1->cd();
          if ( i == 0 ) {
            plotCol.at(j).at(i)->Draw("Hist");
          } else {
            plotCol.at(j).at(i)->Draw("Hist&sames");
          }
        }
        c1->SaveAs((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Waveform/"+std::to_string(j+1)+"/"+FILE_NAME+"_mid"+std::to_string(j+1)+"_"+std::to_string(evt)+"evt_waveform.png"));
      }
    }
    fclose(fp);
  }

  TGraph* normTimeGR = new TGraph(xCor.size(), &(xCor[0]), &(dnormalized_time[0]));
  TGraph* dTimeGR = new TGraph(xCor.size(), &(xCor[0]), &(ddTime[0]));

  normTimeGR->SetTitle("#font[42]{normalized time};#font[42]{# evt};#font[42]{triggered time [ms]}");
  dTimeGR->SetTitle("#font[42]{diff time};#font[42]{# evt};#font[42]{diff time [ms]}");

  c2->cd(); pad_TL->cd(); normTimeGR->Draw("apl");
  c2->cd(); pad_TR->cd(); dTimeGR->Draw("apl");
  c2->SaveAs((TString)(BASE_DIR+SCENARIO+"/"+fileNum+"/Timing/Timimg.png")); 

  return 0;
}

