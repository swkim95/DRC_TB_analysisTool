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
	kTiming,
	kTest
};

enum runMode {
	kFull = 0,
    kPrompt,
    kCoarse,
    kNoPed
};

std::string BASE_DIR = "/Users/swkim/DRC/0703_SV_develop/DRC_TB_analysisTool/";
std::string SCENARIO = "GenericPMT";
std::string DATA_DIR = "/Users/swkim/DRC/SV_TB_0604/data_220604/test_data/";
std::string FILE_NAME = "elec_07_03";

int NofDAQModule = 2;

std::vector<std::vector<int>> chInput   {
											{0, 8},
											{0, 8, 16}
									    };

std::vector<std::vector<int>> plotColor {
											{632, 600},
											{1, 1, 432}
										};


std::vector<TString> plotName = {
                                "#font[42]{#scale[0.8]{Ext Trigg. 1}}",
                                "#font[42]{#scale[0.8]{Module B - #color[634]{S ch}}}",
                                "#font[42]{#scale[0.8]{Module B - #color[602]{C ch}}}",
                                "#font[42]{#scale[0.8]{Ext Trigg. 2}}"
                                };




// kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
// kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
// kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
