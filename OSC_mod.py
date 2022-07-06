import lecroyparser
import os, sys
from glob import glob
import argparse
from matplotlib import pyplot as plt
import drawFunction
plt.style.use('default')
plt.rcParams["font.family"] = "helvetica"
plt.rcParams["figure.figsize"] = (8, 8)

parser = argparse.ArgumentParser()

parser.add_argument('--run', '-r', action='store', type=int, required=True, help='Type run number')
parser.add_argument('--input', '-i', action='store', type=str, default='/Users/swkim/DRC/SV_TB_0703/DRC_TB_analysisTool/OSC_data/', help='path to data dir')
parser.add_argument('--output', '-o', action='store', type=str, default='/Users/swkim/DRC/SV_TB_0703/DRC_TB_analysisTool/', help='path to plot saving dir')
parser.add_argument('--maxEvt', '-m', action='store', type=int, default=-1, help='Maximum evt number to see')

parser.add_argument('--search', '-s', action='store_true', default=False, help='Evt search mode')
parser.add_argument('--findMax', '-find', action='store_true', default=False, help='If true, find evt with max V difference')
parser.add_argument('--fromEvt', '-from', action='store_true', default=False, help='If true, see plots from desired evt')
parser.add_argument('--timing', '-t', action='store_true', default=False, help='Draw timing plot')
parser.add_argument('--save', '-sv', action='store_true', default=False, help='Save only mode, will not display plots')

args = parser.parse_args()

save_dir = args.output if (args.output).endswith('/') else (args.output) + '/'
save_dir = (save_dir + 'plots_run_' + str(args.run) + '/')
## -------------------------------------- functions ---------------------------------------- ##
## ------------------------------------ new functions -------------------------------------- ##

def getMinMax(file_list) :
    y_min = 0.
    y_max = 0.
    x_min = 0.
    x_max = 0.
    x_range = 0.
    y_file = ''
    x_file = ''
    for f in file_list :
        data = lecroyparser.ScopeData(f)
        y = data.y
        x = data.x
        this_x_range = x.max() - x.min()
        if ( y_min >= y.min() ) : y_file = f
        if ( x_range <= this_x_range ) : x_file = f
        y_min = ( y.min() if y_min >= y.min() else y_min )
        y_max = ( y.max() if y_max <= y.max() else y_max )
        x_min = ( x.min() if x_min >= x.min() else x_min )
        x_max = ( x.max() if x_max <= x.max() else x_max )
    print("X min, max values : ", x_min, x_max)
    print("Y min, max values : ", y_min, y_max)
    print("File with min Y value : ", os.path.basename(y_file))
    print("File with max X range : ", os.path.basename(x_file))
    return [x_min, x_max, y_min, y_max, y_file, x_file]

def get_multiCh_file_list(file_list) :
    c1_list = []
    c2_list = []
    c3_list = []
    c4_list = []

    Ch_1_name = 'Ch_1'
    Ch_2_name = 'Ch_2'
    Ch_3_name = 'Ch_3'
    Ch_4_name = 'Ch_4'

    Ch_list = {Ch_1_name: c1_list, Ch_2_name: c2_list, Ch_3_name: c3_list, Ch_4_name: c4_list}

    for files in file_list :
        file_name = os.path.basename(files)
        if 'C1' in file_name :
            c1_list.append(files)
        if 'C2' in file_name :
            c2_list.append(files)
        if 'C3' in file_name :
            c3_list.append(files)
        if 'C4' in file_name :
            c4_list.append(files)

    entry = max(len(c1_list), len(c2_list), len(c3_list), len(c4_list))

    if (len(c1_list) == 0) :
        print("ch 1 empty, deleting it")
        del Ch_list[Ch_1_name]

    if (len(c2_list) == 0) :
        print("ch 2 empty, deleting it")
        del Ch_list[Ch_2_name]

    if (len(c3_list) == 0) :
        print("ch 3 empty, deleting it")
        del Ch_list[Ch_3_name]

    if (len(c4_list) == 0) :
        print("ch 4 empty, deleting it")
        del Ch_list[Ch_4_name]
    
    return Ch_list, entry

def plot_multiCh_waveform_continuous(file_list, maxEvt) :

    minMax = getMinMax(file_list)
    Ch_list, entry = get_multiCh_file_list(file_list)
    print("Total # of evt = ", entry)
    if (maxEvt >= entry) :
        print("Maximum evt number is " + str(entry-1) + ", setting maxEvt = entry")
        maxEvt = entry
    if (maxEvt == -1) : maxEvt = entry

    for evt in range(maxEvt+1) :
        ch_file_list = list( [Ch_names, Ch_list[Ch_names][evt]] for Ch_names in Ch_list.keys() )
        for chData in ch_file_list :
            osc_data = lecroyparser.ScopeData( chData[1] )
            label = chData[0]
            x = osc_data.x
            y = osc_data.y
            plt.plot(x, y, label=label)
        x_min = minMax[0]
        x_max = minMax[1]
        y_min = minMax[2]
        y_max = minMax[3]
        plt.ylim( [y_min - abs(y_min/100) , y_max + abs(y_max/100)] )
        plt.xlim( [x_min - abs(x_min/100) , x_max + abs(x_max/100)] )
        plt.title("Waveform of evt : " + str(evt))
        plt.ylabel('V', loc='top', fontsize=12)
        plt.xlabel('sec', loc='center',fontsize=12)
        plt.xticks(fontsize=10)
        plt.yticks(fontsize=10)
        plt.grid(linestyle='--', alpha=0.5)
        plt.legend(frameon=True,fontsize=10,loc="lower left")
        plt.locator_params(axis='y', nbins=10)
        plt.locator_params(axis='x', nbins=10)
        if not (os.path.exists(save_dir + 'all_evt')) : os.makedirs(save_dir + 'all_evt')
        plt.savefig(save_dir + 'all_evt/Waveform_evt_' + str(evt) + '.png')
        if (args.save) :
            plt.close()
            continue
        plt.show(block=False)
        plt.pause(0.75)
        plt.close()

def plot_multiCh_max(file_list) :

    minMax = getMinMax(file_list)
    max_file = minMax[4]
    max_idx = int(os.path.basename(max_file)[11:-4])
    Ch_list, entry = get_multiCh_file_list(file_list)
    print("Total # of evt = ", entry)

    ch_file_list = list( [keys, Ch_list[keys][max_idx]] for keys in Ch_list.keys() )
    for chData in ch_file_list :
        osc_data = lecroyparser.ScopeData( chData[1] )
        label = chData[0]
        x = osc_data.x
        y = osc_data.y
        plt.plot(x, y, label=label)
    x_min = minMax[0]
    x_max = minMax[1]
    y_min = minMax[2]
    y_max = minMax[3]
    plt.ylim( [y_min - abs(y_min/100) , y_max + abs(y_max/100)] )
    plt.xlim( [x_min - abs(x_min/100) , x_max + abs(x_max/100)] )
    plt.title("Waveform of evt : " + str(max_idx))
    plt.ylabel('V', loc='top', fontsize=12)
    plt.xlabel('sec', loc='center',fontsize=12)
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)	
    plt.grid(linestyle='--', alpha=0.5)
    plt.legend(frameon=True,fontsize=10,loc="lower left")
    plt.locator_params(axis='y', nbins=10)
    plt.locator_params(axis='x', nbins=10)
    if not (os.path.exists(save_dir + 'max_evt')) : os.makedirs(save_dir + 'max_evt')
    plt.savefig(save_dir + 'max_evt/Waveform_max_evt_' + str(max_idx) + '.png')
    if (args.save) :
        plt.close()
    plt.show(block=False)
    print("Press enter to continue...")
    input()
    plt.close()

def plot_multiCh_waveform_from(file_list, maxEvt) :

    minMax = getMinMax(file_list)

    Ch_list, entry = get_multiCh_file_list(file_list)
    print("Total # of evt = ", entry)

    if (maxEvt >= entry) :
        print("Maximum evt number is " + str(entry-1) + ", setting maxEvt = entry")
        maxEvt = entry
    if (maxEvt == -1) : maxEvt = entry

    print("Please enter evt num to start with (press q to exit)")
    fromEvt = input()
    if (fromEvt == 'q') : sys.exit()
    if not type(fromEvt) == int : fromEvt = int(fromEvt)
    if (fromEvt > maxEvt) :
        print("Start evt number exceeds the max evt num, please check")
        sys.exit()

    for evt in range(maxEvt+1) :
        if not (evt >= fromEvt) : continue
        ch_file_list = list( [keys, Ch_list[keys][evt]] for keys in Ch_list.keys() )
        for chData in ch_file_list :
            osc_data = lecroyparser.ScopeData( chData[1] )
            label = chData[1]
            x = osc_data.x
            y = osc_data.y
            plt.plot(x, y, label=label)
        x_min = minMax[0]
        x_max = minMax[1]
        y_min = minMax[2]
        y_max = minMax[3]
        plt.ylim( [y_min - abs(y_min/100) , y_max + abs(y_max/100)] )
        plt.xlim( [x_min - abs(x_min/100) , x_max + abs(x_max/100)] )
        plt.title("Waveform of evt : " + str(evt))
        plt.ylabel('V', loc='top', fontsize=12)
        plt.xlabel('sec', loc='center',fontsize=12)
        plt.xticks(fontsize=10)
        plt.yticks(fontsize=10)	
        plt.grid(linestyle='--', alpha=0.5)
        plt.legend(frameon=True,fontsize=10,loc="lower left")
        plt.locator_params(axis='y', nbins=10)
        plt.locator_params(axis='x', nbins=10)
        if not (os.path.exists( save_dir + 'evt_from_' + str(fromEvt) + '_to_' + str(maxEvt) )) : os.makedirs(save_dir + 'evt_from_' + str(fromEvt) + '_to_' + str(maxEvt))
        plt.savefig(save_dir + 'evt_from_' + str(fromEvt) + '_to_' + str(maxEvt) +'/Waveform_evt_' + str(evt) + '.png')
        if (args.save) :
            plt.close()
            continue
        plt.show(block=False)
        plt.pause(0.75)
        plt.close()

def plot_multiCh_waveform_discrete(file_list) :

    minMax = getMinMax(file_list)
    Ch_list, entry = get_multiCh_file_list(file_list)
    print("Total # of evt = ", entry)

    while(True) :
        print("Enter evt num (q to exit)")
        evtNum = input()
        if (evtNum == 'q') : break
        if not (type(evtNum) == int) : evtNum = int(evtNum)
        if (evtNum >= entry) :
            print("Maximum evt number is " + str(entry-1) + ", please check")
            continue

        ch_file_list = list( [keys, Ch_list[keys][evtNum]] for keys in Ch_list.keys() )
        for chData in ch_file_list :
            osc_data = lecroyparser.ScopeData( chData[1] )
            label = chData[0]
            x = osc_data.x
            y = osc_data.y
            plt.plot(x, y, label=label)
        x_min = minMax[0]
        x_max = minMax[1]
        y_min = minMax[2]
        y_max = minMax[3]
        plt.ylim( [y_min - abs(y_min/100) , y_max + abs(y_max/100)] )
        plt.xlim( [x_min - abs(x_min/100) , x_max + abs(x_max/100)] )
        plt.title("Waveform of evt : " + str(evtNum))
        plt.ylabel('V', loc='top', fontsize=12)
        plt.xlabel('sec', loc='center',fontsize=12)
        plt.xticks(fontsize=10)
        plt.yticks(fontsize=10)	
        plt.grid(linestyle='--', alpha=0.5)
        plt.legend(frameon=True,fontsize=10,loc="lower left")
        plt.locator_params(axis='y', nbins=10)
        plt.locator_params(axis='x', nbins=10)
        if not (os.path.exists( save_dir + 'Evt_search' )) : os.makedirs( save_dir + 'Evt_search' )
        plt.savefig(save_dir + 'Waveform_evt_' + str(evtNum) + '.png')
        if (args.save) :
            plt.close()
            continue
        plt.show(block=False)
        print("Press enter to continue...")
        input()
        plt.close()

## -------------------------------------------------------------------------------- ##

def CalcTime(data) :
    Time = str(data).split('\n')[-2].split(' ')[-1].split(':')
    floatTime = float(Time[0]) * 3600 + float(Time[1]) * 60 + float(Time[2])
    return floatTime

def Get_Timing(file_list) :
    Empty = True
    single_Ch_filelist = []
    if Empty :
        single_Ch_filelist = list( filter(lambda x: os.path.basename(x).startswith('C1'), file_list) )
        Empty = len(single_Ch_filelist)
    elif Empty :
        single_Ch_filelist = list( filter(lambda x: os.path.basename(x).startswith('C1'), file_list) )
        Empty = len(single_Ch_filelist)
    elif Empty :
        single_Ch_filelist = list( filter(lambda x: os.path.basename(x).startswith('C1'), file_list) )
        Empty = len(single_Ch_filelist)
    elif Empty :
        single_Ch_filelist = list( filter(lambda x: os.path.basename(x).startswith('C1'), file_list) )
    entry = len(single_Ch_filelist)
    if (entry == 0) :
        print("[Error] No channel found, exiting...")
        sys.exit()
    Ref_time = 0
    Ref_for_Norm_time = 0
    time_diff_array = []
    Norm_time_array = []
    evtNum_array = []

    for evt in range(entry) :
        osc_data = lecroyparser.ScopeData( single_Ch_filelist[evt] )
        trig_time = CalcTime(osc_data)

        time_diff = trig_time - Ref_time
        if (evt == 0) :
            time_diff = 0
            Ref_for_Norm_time = trig_time

        Ref_time = trig_time
        Norm_time = trig_time - Ref_for_Norm_time
        evtNum_array.append(evt)
        time_diff_array.append(time_diff)
        Norm_time_array.append(Norm_time)

    return evtNum_array, time_diff_array, Norm_time_array

def plot_timing(file_list) :

    plt.rcParams['figure.figsize'] = [12, 6]

    timing_arr = Get_Timing(file_list)

    ax1 = plt.subplot(1, 2, 1, title='', ylabel='sec', xlabel='Evt Num')
    ax1.set_title("Time difference")
    ax2 = plt.subplot(1, 2, 2, title='', ylabel='sec', xlabel='Evt Num')
    ax2.set_title("Norm time")
    for ax in (ax1, ax2) :
        ax.grid(which='major', axis='both', linestyle='-.')
        ax.grid(which='minor', linestyle=':')
    ax1.plot(timing_arr[0], timing_arr[1], marker='.', markersize=8)
    ax1.set_title("Diff time")
    ax2.plot(timing_arr[0], timing_arr[2], marker='.', markersize=8)
    ax2.set_title("Norm time")
    plt.locator_params(axis='y', nbins=10)
    plt.locator_params(axis='x', nbins=10)
    if not (os.path.exists( save_dir + 'Timing' )) : os.makedirs( save_dir + 'Timing' )
    plt.savefig(save_dir + 'Timing/Timing.png')
    if (args.save) :
        plt.close()
    plt.show(block=False)
    plt.pause(0.75)
    print("Press enter to continue...")
    input()
    plt.close()

## ----------------------------------- main --------------------------------------- ##

file_list = glob(args.input+'*.trc') if args.input.endswith('/') else glob(args.input+'/*.trc')
if len(file_list) == 0 :
    print( "[Error] No *.trc files found in dir : '", args.input, "', please check" )
    sys.exit()
file_list.sort()

if   ( args.findMax ) : plot_multiCh_max(file_list)
elif ( args.timing  ) : plot_timing(file_list)
elif ( args.fromEvt ) : plot_multiCh_waveform_from(file_list, args.maxEvt)
elif ( args.search  ) : plot_multiCh_waveform_discrete(file_list)
else                  : plot_multiCh_waveform_continuous(file_list, args.maxEvt)

## -------------------------------------------------------------------------------- ##
