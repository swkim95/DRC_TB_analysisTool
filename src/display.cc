#include <stdio.h>
#include <string>

#include "TH1.h"
#include "TCanvas.h"

int display(){
    //bool r = true;
    TCanvas *c1 = new TCanvas("c1", "Display", 800, 800);
    TH1F* h1 = new TH1F("h1", "Hist 1", 100, -5, 5);
    /*
    while(r) {
        h1->Reset();
        h1->FillRandom("gaus", 10000);
        c1->cd();
        h1->Draw("Hist");
        int i = 0;
        std::cout << "Enter anything" << std::endl;
        scanf("%d", &i);
        if(i == -1) {
            r = false;
        }
    }
    */
    h1->Reset();
    h1->FillRandom("gaus", 10000);
    c1->cd();
    h1->Draw("Hist");
    int i = 0;
    std::cout << "Enter anything" << std::endl;
    scanf("%d", &i);
    return 0;
}