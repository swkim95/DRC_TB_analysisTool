#include <stdio.h>
#include <chrono>
#include <thread>
#include <TPad.h>
#include <string>
// #include <Windows.h>

constexpr int TIME_TO_SLEEP = 200;
//std::thie_thread::sleep_for( std::chrono::milliseconds(TIME_TO_SLEEP) )

void plot_waveform_4ch_original(std::string fileNum, enum runMode opt)
{
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char data[64];
  short adc[32736];

  int evtNum;
  if (opt == runMode::kPrompt) {
    std::cout << "enter evt num" << std::endl;
    scanf("%d", &evtNum);
  }

  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1000, 1000);
  c1->Divide(2, 2);
  TH1F *plot1 = new TH1F("plot1", (TString)( "Waveform ch"+std::to_string(chInput.at(0)) ), 1000, 0, 200); 
  TH1F *plot2 = new TH1F("plot2", (TString)( "Waveform ch"+std::to_string(chInput.at(1)) ), 1000, 0, 200); 
  TH1F *plot3 = new TH1F("plot3", (TString)( "Waveform ch"+std::to_string(chInput.at(2)) ), 1000, 0, 200); 
  TH1F *plot4 = new TH1F("plot4", (TString)( "Waveform ch"+std::to_string(chInput.at(3)) ), 1000, 0, 200); 
  plot1->SetStats(0); plot1->SetLineWidth(1); plot1->GetYaxis()->SetRangeUser(0, 4096);  plot1->SetLineColor(1);
  plot2->SetStats(0); plot2->SetLineWidth(1); plot2->GetYaxis()->SetRangeUser(0, 4096);  plot2->SetLineColor(634);
  plot3->SetStats(0); plot3->SetLineWidth(1); plot3->GetYaxis()->SetRangeUser(0, 4096);  plot3->SetLineColor(602);
  plot4->SetStats(0); plot4->SetLineWidth(1); plot4->GetYaxis()->SetRangeUser(0, 4096);  plot4->SetLineColor(1);

  // get # of events in file
  fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");

  for (int evt = 0; evt < nevt; evt++) {

    if (opt == runMode::kPrompt) {
      if (!(evt == evtNum)) continue;
    }

    // read header
    fread(data, 1, 64, fp);
    printf("evt = %d\n", evt);
    // read waveform
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
    plot1->Reset();
    plot2->Reset();
    plot3->Reset();
    plot4->Reset();
//    for (i = 0; i < 1023; i++) {                      // org
//      plot->Fill(i, adc[i * 32 + ch_to_plot]);         // org
    for (int i = 0; i < 1000; i++) {                      // mod
      plot1->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(0)]);         // mod
      plot2->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(1)]);         // mod
      plot3->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(2)]);         // mod
      plot4->Fill(i * 0.2, adc[(i + 1) * 32 + chInput.at(3)]);         // mod
    }

    c1->cd(1); plot1->Draw("hist");
    c1->cd(2); plot2->Draw("hist");
    c1->cd(3); plot3->Draw("hist"); plot2->Draw("hist&sames");
    c1->cd(4); plot4->Draw("hist");
    c1->Modified();
    c1->Update();

    c1->cd(1);
    TLatex* nEvtLatex = new TLatex(); 
    nEvtLatex->DrawLatexNDC(0.05, 0.93,  "#font[42]{#scale[1.4]{"+(TString)std::to_string(evt)+"}}");
    c1->Modified();
    c1->Update();

    std::this_thread::sleep_for( std::chrono::milliseconds(TIME_TO_SLEEP) );
    nEvtLatex->~TLatex();
    c1->Modified();
    c1->Update();
  }

  fclose(fp);
}

