#include <stdio.h>
// #include <Windows.h>

int plot_waveform_4ch(std::string fileNum)
{
  int ch_to_plot;
  FILE *fp;
  int file_size;
  int nevt;
  char data[64];
  short adc[32736];
  int evt;
  int data_length;
  int run_number;
  int tcb_trig_type;
  int tcb_trig_number;
  long long tcb_trig_time;
  int mid;
  int local_trig_number;
  int local_trigger_pattern;
  long long local_trig_time;
  long long diff_time;
  long long fine_time;
  long long coarse_time;
  int itmp;
  long long ltmp;
  int i;
  int cont;

  TCanvas *c1 = new TCanvas("c1", "CAL DAQ", 1000, 1000);
  c1->Divide(2, 2);
  TH1F *plot1 = new TH1F("plot1", "Waveform ch1", 1000, 0, 200); 
  TH1F *plot2 = new TH1F("plot2", "Waveform ch5", 1000, 0, 200); 
  TH1F *plot3 = new TH1F("plot3", "Waveform ch11", 1000, 0, 200); 
  TH1F *plot4 = new TH1F("plot4", "Waveform ch15", 1000, 0, 200); 
  plot1->SetStats(0);
  plot2->SetStats(0);
  plot3->SetStats(0);
  plot4->SetStats(0);

  // get # of events in file
  fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");
  fseek(fp, 0L, SEEK_END);
  file_size = ftell(fp);
  fclose(fp);
  nevt = file_size / 65536;
  
  printf("-----------------------------------------------------------------------\n");
  fp = fopen((DATA_DIR+FILE_NAME+"_"+fileNum+".dat").c_str(), "rb");

  for (evt = 0; evt < nevt; evt++) {
    // read header
    fread(data, 1, 64, fp);
    
    // data length
    data_length = data[0] & 0xFF;
    itmp = data[1] & 0xFF;
    itmp = itmp << 8;
    data_length = data_length + itmp;
    itmp = data[2] & 0xFF;
    itmp = itmp << 16;
    data_length = data_length + itmp;
    itmp = data[3] & 0xFF;
    itmp = itmp << 24;
    data_length = data_length + itmp;

    // run number
    run_number = data[4] & 0xFF;
    itmp = data[5] & 0xFF;
    itmp = itmp << 8;
    run_number = run_number + itmp;
    
    // trigger type
    tcb_trig_type = data[6] & 0xFF;
    
    // TCB trigger #
    tcb_trig_number = data[7] & 0xFF;
    itmp = data[8] & 0xFF;
    itmp = itmp << 8;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[9] & 0xFF;
    itmp = itmp << 16;
    tcb_trig_number = tcb_trig_number + itmp;
    itmp = data[10] & 0xFF;
    itmp = itmp << 24;
    tcb_trig_number = tcb_trig_number + itmp;

    // TCB trigger time
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
    
    // mid
    mid = data[18] & 0xFF;

    // local trigger #
    local_trig_number = data[19] & 0xFF;
    itmp = data[20] & 0xFF;
    itmp = itmp << 8;
    local_trig_number = local_trig_number + itmp;
    itmp = data[21] & 0xFF;
    itmp = itmp << 16;
    local_trig_number = local_trig_number + itmp;
    itmp = data[22] & 0xFF;
    itmp = itmp << 24;
    local_trig_number = local_trig_number + itmp;

    // local trigger #
    local_trigger_pattern = data[23] & 0xFF;
    itmp = data[24] & 0xFF;
    itmp = itmp << 8;
    local_trigger_pattern = local_trigger_pattern + itmp;
    itmp = data[25] & 0xFF;
    itmp = itmp << 16;
    local_trigger_pattern = local_trigger_pattern + itmp;
    itmp = data[26] & 0xFF;
    itmp = itmp << 24;
    local_trigger_pattern = local_trigger_pattern + itmp;

    // local trigger time
    fine_time = data[27] & 0xFF;
    fine_time = fine_time * 11;     // actually * (1000 / 90)
    coarse_time = data[28] & 0xFF;
    ltmp = data[29] & 0xFF;
    ltmp = ltmp << 8;
    coarse_time = coarse_time + ltmp;
    ltmp = data[30] & 0xFF;
    ltmp = ltmp << 16;
    coarse_time = coarse_time + ltmp;
    ltmp = data[31] & 0xFF;
    ltmp = ltmp << 24;
    coarse_time = coarse_time + ltmp;
    ltmp = data[32] & 0xFF;
    ltmp = ltmp << 32;
    coarse_time = coarse_time + ltmp;
    ltmp = data[33] & 0xFF;
    ltmp = ltmp << 40;
    coarse_time = coarse_time + ltmp;
    coarse_time = coarse_time * 1000;   // get ns
    local_trig_time = fine_time + coarse_time;

    diff_time = local_trig_time - tcb_trig_time;
    printf("evt = %d, data length = %d, run # = %d, mid = %d\n", evt, data_length, run_number, mid);
    printf("trigger type = %X, local trigger pattern = %X\n", tcb_trig_type, local_trigger_pattern);
    printf("TCB trigger # = %d, local trigger # = %d\n", tcb_trig_number, local_trig_number);
    printf("TCB trigger time = %lld, local trigger time = %lld, difference = %lld\n", tcb_trig_time, local_trig_time, diff_time);
    printf("-----------------------------------------------------------------------\n");
    
    // read waveform
    fread(adc, 2, 32736, fp);
    
    // fill waveform for channel to plotgecit 
    plot1->Reset();
    plot2->Reset();
    plot3->Reset();
    plot4->Reset();
//    for (i = 0; i < 1023; i++) {                      // org
//      plot->Fill(i, adc[i * 32 + ch_to_plot]);         // org
    for (i = 0; i < 1000; i++) {                      // mod
      plot1->Fill(i * 0.2, adc[(i + 1) * 32 + 0]);         // mod
      plot2->Fill(i * 0.2, adc[(i + 1) * 32 + 4]);         // mod
      plot3->Fill(i * 0.2, adc[(i + 1) * 32 + 10]);         // mod
      plot4->Fill(i * 0.2, adc[(i + 1) * 32 + 14]);         // mod
//if (adc[i * 32 + ch_to_plot] < 3000) printf("abnormal value = %d @ %d\n", adc[i * 32 + ch_to_plot], evt);
    }

    c1->cd(1); plot1->Draw("hist");
    c1->cd(2); plot2->Draw("hist");
    c1->cd(3); plot3->Draw("hist");
    c1->cd(4); plot4->Draw("hist");
    c1->Modified();
    c1->Update();

    c1->cd(1);
    TLatex* nEvtLatex = new TLatex(); 
    // nEvtLatex->DrawLatexNDC(0.1, 0.99,  "#font[42]{#scale[2]{"+(TString)std::to_string(evt)+"}}");
    nEvtLatex->DrawLatexNDC(0.05, 0.93,  "#font[42]{#scale[1.4]{"+(TString)std::to_string(evt)+"}}");
    c1->Modified();
    c1->Update();
    // Sleep(300);

      
    //printf("Continue? ");
    //scanf("%d", &cont);
    
    //if (cont == 0)
    //  evt = nevt;

    //if(evt == 10) {
    //  evt = nevt;
    //}
    nEvtLatex->~TLatex();
    c1->Modified();
    c1->Update();
  }

  fclose(fp);

  return 0;
}

