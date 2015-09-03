#!/usr/bin/env python

import argparse
import os


# ---------- configure script with command line options
hlt_cfg="/users/rwalsh/run2/mssmhbb/scratch/V34"
hlt_set="/dev/CMSSW_7_4_0/GRun/V34"
l1_menu="L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"

# http://docs.python.org/2/library/argparse.html
parser = argparse.ArgumentParser(description='This tool retrieves a private trigger configuration and a setup.')


parser.add_argument('-c', '--configuration-path', action='store', metavar='PATH', #type=str,
                    default=hlt_cfg,
                    #required=True,
                    help="name of the base hlt configuration path")

parser.add_argument('-s', '--setup-path', action='store', metavar='PATH', #type=str,
                    default=hlt_set,
                    #required=True,
                    help="name of the base hlt setup configuration path")

parser.add_argument('-l1', '--l1-menu', action='store', metavar='NAME', #type=str,
                    default=l1_menu,
                    #required=True,
                    help="name of the base hlt setup configuration path")

#parser.add_argument('-j', '--crab-job', action='store_true', #type= bool,
#                    #default=isCrabJob,
#                    help='configuration file will run in a crab job  ')
#

args = parser.parse_args()
hlt_cfg=args.configuration_path
hlt_set=args.setup_path
l1_menu=args.l1_menu

# Outputs
cfg_file = 'hlt_'+'_'.join(hlt_cfg.split("/")[3:])+'_cfg.py'
set_file = 'setup_'+''.join(hlt_set.split("/")[2].split('_')[1:])+'_'+'_'.join(hlt_set.split("/")[3:])+'_cff.py'

# Get configurations from confDB
## My configuration
os.system('rm -f '+cfg_file )
print "The configuration..."
os.system('hltGetConfiguration '+hlt_cfg+' --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator "stage1,gt" --l1Xml '+l1_menu+' > '+cfg_file)
## Setup configuration
print "The setup..."
if not os.path.exists(set_file):
   os.system('edmConfigFromDB --cff --configName '+hlt_set+' --nopaths --services -PrescaleService,-EvFDaqDirector,-FastMonitoringService > '+set_file)

print "Modifications..."
# Add setup to my configuration 
newline = '6iprocess.load(\''+os.path.splitext(set_file)[0]+'\')'
command = 'sed -i "'+newline+'" '+cfg_file
os.system(command)

# Comment process.DQMOutput
command =  'sed -i "s/process.DQMOutput/#process.DQMOutput/g" '+cfg_file
os.system(command)

# OPENHLT PRODUCERS
command = 'python openHLT.py -p -i /store/mc/Fall13dr/QCD_Pt-1000to1400_Tune4C_13TeV_pythia8/GEN-SIM-RAW/castor_tsg_PU40bx25_POSTLS162_V2-v1/00000/0003CAF9-0A9E-E311-ACEC-0025905A612A.root -g openhlt_go_prod.py -o Producers.root -t '+cfg_file+' -n 100 -j'
os.system(command)

# OPENHLT FILTERS
command = 'python openHLT.py -i Producers.root -g openhlt_go_filt.py -o Filters.root -t '+cfg_file+' -n 100 -j'
os.system(command)


