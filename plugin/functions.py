
import os, sys

## ------------------------------------------------------------------------------------- ##
## Custom function 1 : convert_args_to_enum( argument parser )                   
##                                                                               
## Translate args.type, args.pedType into enum string (kIntegral, kFull etc...)
## ------------------------------------------------------------------------------------- ##
def convert_args_to_enum(arguments):
    ## Declare dictionary initialized with zeros
    type_dict            = {'type': 0, 'pedType': 0}

    ## List of candidates available for each enum discriminator
    int_candidate        = ['integral', 'kIntegral', 'kintegral', 'int', 'Integral', '0']
    peak_candidate       = ['peak', 'kPeak', 'kpeak', 'p', 'Peak', '1']
    wave_candidate       = ['waveform', 'kWaveform', 'kwaveform', 'w', 'Waveform', 'wave', '2']
    calPed_candidate     = ['calcPed', 'c', 'Cal', 'cal', 'kPed', 'kped', 'Ped', 'ped', '3']

    ## List of candidates available for each enum run mode
    full_ped_candidate   = ['full', 'kFull', 'kfull', 'f', 'Full', '0']
    prompt_ped_candidate = ['prompt', 'kPrompt', 'kprompt', 'p', 'Prompt', '1']
    no_ped_candidate     = ['no', 'kNo', 'kno', 'n', 'No', '2']
    dummy_ped_candidate  = ['-1']

    ## Insert appropriate enum discriminator into type_dict['type']
    if arguments.type in int_candidate :
        type_dict['type'] = 'kIntegral'
    if arguments.type in peak_candidate :
        type_dict['type'] = 'kPeak'
    if arguments.type in wave_candidate :
        type_dict['type'] = 'kWaveform'
    if arguments.type in calPed_candidate :
        type_dict['type'] = 'kPed'

    ## Insert appropriate enum run mode into type_dict['pedType']
    if arguments.pedType in full_ped_candidate :
        type_dict['pedType'] = 'kFull'
    if arguments.pedType in prompt_ped_candidate :
        type_dict['pedType'] = 'kPrompt'
    if arguments.pedType in no_ped_candidate :
        type_dict['pedType'] = 'kNoPed'
    if arguments.pedType in dummy_ped_candidate :
        type_dict['pedType'] = 'kDummyPed'

    ## Next, make sure every arguments are properly defined.
    ## If none of available candidates were found, print an error message and exit macro
    if (type_dict['type'] == 0) :
        print('[Argument Error]', arguments.type, 'is not an allowed type, please check')
        sys.exit('Argument type error')
    ## If none of available candidates were found, print an error message and exit macro
    if (type_dict['pedType'] == 0) :
        print('[Argument Error]', arguments.pedType, 'is not an allowed ped type, please check')
        sys.exit('Argument pedestal type error')
    ## kNoPed pedestal mode is only available with kIntegral mode, so check it
    if ( (type_dict['pedType'] == 'kNoPed') and not (type_dict['type'] == 'kIntegral') ) :
        print('[Argument Error] Ped type kNoPed is only allowed for the integral plot, please check')
        print('[Argument Error] Current ped type :', type_dict['pedType'], 'and current type :', type_dict['type'])
        sys.exit('kNoPed misused error')
    ## The only type which don't need pedNum is waveform mode
    if ( (arguments.pedNum == '-1') and not (type_dict['type'] == 'kWaveform' ) ) :
        print('[Argument Error]', arguments.type, 'cannot run without pedNum, please check')
        sys.exit('Argument missing error')
    ## The only type which don't need pedType is pedestal calculation mode
    if ( (arguments.pedType == '-1') and not (type_dict['type'] == 'kPed') ) :
        print('[Argument Error]', arguments.type, 'cannot run without pedType, please check')
        sys.exit('Argument missing error')

    ## Return dictionary with type, ped type stored in it
    return type_dict
## ------------------------------------------------------------------------------------- ##


## ------------------------------------------------------------------------------------- ##
## Custom function 2 : get_commands( argument parser, type_dict )
##
## Get actual command lines to be executed using - runNum/pedNum/config from args
##                                               - type/pedType         from type_dict
## ------------------------------------------------------------------------------------- ##
def get_commands(arguments, type_dict):
    ## type_dict from function convert_args_to_enum()
    run_type = type_dict['type']
    ped_type = type_dict['pedType']

    ## Get command line to include configuration header file
    config_cmd = "#include \"./include/%s\"" % (arguments.config)
    
    ## Get proper command lines to be executed based on ped type, run type
    if (run_type == 'kPed'):
        macro_cmd = ".x ./src/full_ped.cc(\"%s\", \"%s\")" % (arguments.runNum, arguments.pedNum)

    if (run_type == 'kWaveform'):
        macro_cmd = ".x ./src/plot_waveform_4ch.cc(\"%s\", %s)" % (arguments.runNum, ped_type)

    if (run_type == 'kIntegral'):
        macro_cmd = ".x ./src/integral_base.cc(\"%s\", \"%s\", %s)" % (arguments.runNum, arguments.pedNum, ped_type)

    if (run_type == 'kPeak'):
        macro_cmd = ".x ./src/peak_base.cc(\"%s\", \"%s\", %s)" % (arguments.runNum, arguments.pedNum, ped_type)

    ## Return each command lines
    return config_cmd, macro_cmd
## ------------------------------------------------------------------------------------- ##

## ------------------------------------------------------------------------------------- ##
## Custom function 3 : init_directories( baseDir in string )
##
## Same as init.sh
## Creates log, plots, ped, validFiles in BASE_DIR which is defined in config.h file.
## IMPORTANT :: MUST BE EXECUTED AFTER THE HEADER FILE IS INCLUDED
## ------------------------------------------------------------------------------------- ##
def init_directories(baseDir) :
    ## List of directories to be created under the baseDir 
    dir_list = ['results/log',
                'results/ped/validFiles',
                'results/plots/Integral/png', 'results/plots/Integral/svg', 'results/plots/Peak/png', 'results/plots/Peak/svg',
                'results/validFiles/Integral', 'results/validFiles/Peak']
    for dirs in dir_list :
        ## required_dir = base dir + dir in list
        required_dir = os.path.join(baseDir, dirs)
        ## Check if directories already exist to make sure they are not overwritten. If not, create them.
        if not (os.path.exists(required_dir)) :
            print("No dir", required_dir, ", creating it")
            os.makedirs(required_dir)
## ------------------------------------------------------------------------------------- ##
