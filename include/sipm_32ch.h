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

std::string BASE_DIR = "/Users/khwang/scratch/TestBeam/github/TB_analysisTool/";
std::string SCENARIO = "SiPM_32ch";
std::string DATA_DIR = "/Users/khwang/scratch/TestBeam/github/TB_analysisTool/sampleData/";
std::string FILE_NAME = "elec";

int NofDAQModule = 2;

std::vector<std::vector<int>> chInput   {
											{0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 11, 12, 13, 14, 15,
											 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
											{0, 8, 16}
										};

std::vector<std::vector<int>> plotColor {
											{1, 600, 632, 416, 800, 416, 616, 432, 
										     1, 600, 632, 416, 800, 416, 616, 432,
										     1, 600, 632, 416, 800, 416, 616, 432,
										     1, 600, 632, 416, 800, 416, 616, 432},
											{1, 1, 432}
										};

std::vector<TString> plotName = {
                                "#font[42]{#scale[0.8]{Ext Trigg. 1}}",
                                "#font[42]{#scale[0.8]{Ext Trigg. 2}}",
                                "#font[42]{#scale[0.8]{Discriminator}}"
                                };




// kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
// kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
// kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900
