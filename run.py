import sys, os
import argparse
import ROOT
from plugin.functions import convert_args_to_enum, get_commands, init_directories ## custom functions

## Using argument parser
parser = argparse.ArgumentParser()
parser.add_argument('--config', '-c', action='store', type=str, default='config.h', help='Path of header file with configs') # == location.h for Severance TB version code
parser.add_argument('--runNum', '-r', action='store', type=str, required=True, help='Enter Run number of data') # == fileNum
parser.add_argument('--pedNum', '-p', action='store', type=str, default='-1', help='Enter pedestal file number')
parser.add_argument('--type', '-t', action='store', type=str, required=True, help='Enter which code to run >> 0 : integral / 1 : peak / 2 : waveform / 3 : calcPed') # == enum discriminator
parser.add_argument('--pedType', '-pt', action='store', type=str, default='-1', help='Enter type of pedestal >> 0 : full / 1 : prompt / 2 : noPed') # == enum runMode
parser.add_argument('--debug', '-d', action='store_true', default=False, help='Activate debug mode, will print out debug info')
args = parser.parse_args()

## Translate args.type, args.pedType into enum string (kIntegral, kFull etc...)
type_dict = convert_args_to_enum(args)

## If debug, print out - which type of plots will be drawn 
##                     - which type of pedestal will be used
if args.debug :
    print('Will execute macro with types')
    for keys in type_dict :
        print(keys,':',type_dict[keys])

## Get actual command lines to be executed using - runNum/pedNum/config from args
##                                               - type/pedType         from type_dict
config_cmd, macro_cmd = get_commands(args, type_dict)

## If debug, print out - which header file will be include. Default is config_cmd = "#include "config.h"
##                     - which macro line will be executed. Ex) ".x integral_base.cc("10", "9", kFull)" etc...
if args.debug :
    print("Reading header :", arguments.config)
    print("Executing macro :", macro_cmd[4:])

ROOT.gInterpreter.ProcessLine(config_cmd) ## == gROOT->ProcessLine("#include \"location.h\"")
init_directories(ROOT.BASE_DIR)           ## == init.sh, Creates log, plots, ped, validFiles in BASE_DIR defined in config.h header file. IMPORTANT :: NEED TO BE EXECUTED AFTER THE HEADER FILE IS INCLUDED
ROOT.gInterpreter.ProcessLine(macro_cmd)  ## == gROOT->ProcessLine(".x macro.cc("arguments~~")");