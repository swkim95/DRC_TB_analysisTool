# TB_analysisTool
## How to user
Location of files (data, dir for plots and validation files, etc.) is set on "location.h"

Before starting, some dirs are should be generated.
```
source init.sh <base directory name>
```

And then, run analysis tool with root like below.
```
root -l 'Run.cc("<fime_number>", "<ped_num>", <analysis_mode>, <ped_mode>")'
```

Available analysis_mode is
```
enum discriminator {
	kIntegral = 0,
	kPeak,
	kPed,
	kWaveform,
	kTest
};
```

Available ped_mode is
```
enum runMode {
	kFull = 0,
  kPrompt
};
```

If want to run 'Integral' mode with 'prompt' ped calculation, run like below.
```
root -l 'Run.cc("<file_num>", "<ped_num>", kIntegral, kPrompt)'
```
