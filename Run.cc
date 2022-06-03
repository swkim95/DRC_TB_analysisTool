#include <location.h>

void Run(TString fileNum, TString pedNum, enum discriminator opt, enum runMode mode) {

	// if (opt > 5) std::cout << "Allowed arg 3 option is [kIntegral, kPeak, kPed, kWaveform, kTest]" << std::endl;
	// if (mode > 2) std::cout << "Allowed arg 4 option is [kFull, kPrompt]" << std::endl;

	if( opt == discriminator::kIntegral &&  mode == runMode::kFull )   gROOT->ProcessLine(".x integral_base.cc(\""+fileNum+"\", \""+pedNum+"\", kFull)");
	if( opt == discriminator::kIntegral &&  mode == runMode::kPrompt ) gROOT->ProcessLine(".x integral_base.cc(\""+fileNum+"\", \""+pedNum+"\", kPrompt)");
	
	if( opt == discriminator::kPeak &&  mode == runMode::kFull )   gROOT->ProcessLine(".x peak_base.cc(\""+fileNum+"\", \""+pedNum+"\", kFull)");
	if( opt == discriminator::kPeak &&  mode == runMode::kPrompt ) gROOT->ProcessLine(".x peak_base.cc(\""+fileNum+"\", \""+pedNum+"\", kPrompt)");

	if( opt == discriminator::kWaveform ) gROOT->ProcessLine(".x plot_waveform_4ch.C(\""+fileNum+"\")");
	if( opt == discriminator::kPed )      gROOT->ProcessLine(".x full_ped.cc(\""+fileNum+"\", \""+pedNum+"\")");
	if( opt == discriminator::kTest )     gROOT->ProcessLine(".x test.cc");
}

