#include <cstdio>
#include <algorithm>
#include <vector>
#include <numeric>
#include <TPad.h>
#include <utility>
#include <iostream>

enum discriminator {
	kIntegral = 0,
	kPeak,
	kPed,
	kWaveform,
	kTest
};

enum runMode {
	kFull = 0,
    kPrompt
};

std::string BASE_DIR = "/root/scratch/Prompt/baseDir/";
std::string DATA_DIR = "/root/scratch/Prompt/baseDir/DataFiles/";
std::string FILE_NAME = "muon_06_02";

std::vector<int> chInput = {0, 4, 10, 14};
std::vector<int> plotColor = {634, 634, 602, 634};

std::vector<TString> plotName = {
                                "#font[42]{#scale[0.8]{Module A - #color[634]{S ch}}}",
                                "#font[42]{#scale[0.8]{Module B - #color[634]{S ch}}}",
                                "#font[42]{#scale[0.8]{Module B - #color[602]{C ch}}}",
                                "#font[42]{#scale[0.8]{Module C - #color[634]{S ch}}}"
                                };




// kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
// kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
// kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900