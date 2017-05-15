from subprocess import call
import shutil
import os
import exceptions

cmsswBase = os.environ['CMSSW_BASE']

def SetupDics(mass,selection):
    """Function that returns dictionary for current mass.
    
    """
    # Number of observations
    obs_full_r = 371729
    obs_sub_r1 = 259399#358932#
    obs_sub_r2 = 105053#147630#
    obs_sub_r3 = 26760#38906#
    
    # add_path 
    signal_path = cmsswBase + '/src/Analysis/MssmHbb/output/'
    # Specify ditionaries with TAG vs VALUE
    dictionary = {#200 : {'OBSERVATION' : obs_sub_r1, 'SIGNAL_SHAPE_WS' : signal_path + 'signal_M-300', 'BG_SHAPE_WS' : signal_path + 'signal_M-300', 'DATA_SHAPE_WS' : signal_path + 'signal_M-300'},
#                  250 : {'OBSERVATION' : obs_sub_r1, 'SIGNAL_SHAPE_WS' : signal_path + 'signal_M-300', 'BG_SHAPE_WS' : signal_path + 'signal_M-300', 'DATA_SHAPE_WS' : signal_path + 'signal_M-300'},
                  300 : {'OBSERVATION'     : obs_sub_r1,
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.057',
                         'OFFLINE_SFL' : '1.0003',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : 'CMS_PAR3_13TeV            shapeU    -   10.0',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  350 : {'OBSERVATION' : obs_sub_r1,
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-350_TuneCUETP8M1_13TeV-pythia8.root', 
                         'OFFLINE_SFB' : '1.060', 
                         'OFFLINE_SFL' : '1.0002',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : 'CMS_PAR3_13TeV            shapeU    -   10.0',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  400 : {'OBSERVATION' : obs_sub_r1, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-400_TuneCUETP8M1_13TeV-pythia8.root', 
                         'OFFLINE_SFB' : '1.065', 
                         'OFFLINE_SFL' : '1.0004',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : 'CMS_PAR3_13TeV            shapeU    -   10.0',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  500 : {'OBSERVATION' : obs_sub_r1,
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.078', 
                         'OFFLINE_SFL' : '1.0005',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : 'CMS_PAR3_13TeV            shapeU    -   10.0',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  600 : {'OBSERVATION' : obs_sub_r2, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-600_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.089', 
                         'OFFLINE_SFL' : '1.0007',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : '',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  700 : {'OBSERVATION' : obs_sub_r2, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-700_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.096', 
                         'OFFLINE_SFL' : '1.0009',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : '',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  900 : {'OBSERVATION' : obs_sub_r2, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-900_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.105', 
                         'OFFLINE_SFL' : '1.0012',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : '',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  1100: {'OBSERVATION' : obs_sub_r3, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1100_TuneCUETP8M1_13TeV-pythia8.root', 
                         'OFFLINE_SFB' : '1.11', 
                         'OFFLINE_SFL' : '1.0016',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : '',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''},
                  1300: {'OBSERVATION' : obs_sub_r3, 
                         'SIGNAL_SHAPE_ROOT' : signal_path + selection + '_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1300_TuneCUETP8M1_13TeV-pythia8.root',
                         'OFFLINE_SFB' : '1.118', 
                         'OFFLINE_SFL' : '1.0019',
                         'SHAPE_BG1'    : 'CMS_PAR0_13TeV            shapeU    -   10.0',
                         'SHAPE_BG2'    : 'CMS_PAR1_13TeV            shapeU    -   10.0',
                         'SHAPE_BG3'    : 'CMS_PAR2_13TeV            shapeU    -   10.0',
                         'SHAPE_BG4'    : '',
                         'SHAPE_BG5'    : '',
                         'SHAPE_BG6'    : ''}}
    
    if dictionary[mass] != None:
        return dictionary[mass]
    else:
        raise AttributeError("No rulles for mass = " + mass + " were specified in SetupDics")
    

    
def MakeDir(path):
    """Method to create a directory.
    
    """
    if os.path.exists(path):
        shutil.rmtree(path)
    # create new one
    os.makedirs(path)
    # return the path
    return path

def ConstructDataCardName(mass):
    """Function to construct the datacard name.
    
    """
    name = 'hbb_mbb' + str(mass) + '_mssm-13TeV.txt'
    return name

def CopyTemplateDatacard(path_to_template,path_to_new): 
    if not os.path.exists(path_to_template):
        raise IOError("No txt file at " + path_to_template)
    shutil.copyfile(path_to_template,path_to_new)
    
def ReplaceSpecChar(string):
    """Method to replace special characters in string.
    
    """
    temp_string = string
    for i in "/":#"!@#$%^&*()/[]\{};:,.<>?|`~-=_+":
        if i in temp_string:
            temp_string = temp_string.replace(i , '\\'+i)
    return temp_string

def ReplaceTag(path_to_datacard,tag,value):
    """Method to actually replace current TAG for value.
    
    """
    # If Value is a PATH then all / should be used with \/:
    corrected_value = ReplaceSpecChar(str(value))
    command = "sed -i 's/" + str(tag) + "/" + str(corrected_value) + "/g' " + path_to_datacard
    call(command,shell=True)

def AdjustDatacard(path_to_datacard,dictionary):
    """Function to adjust datacard for current mass according to dic.
    
    """
    # check whether it exists
    if not os.path.exists(path_to_datacard):
        raise IOException("No txt file at " + path_to_datacard)
    # replace 
    for tag, value in dictionary.iteritems():
        ReplaceTag(path_to_datacard,tag,value)

if __name__ == '__main__':
    # CMSSW Base
    CMSSW_BASE = os.environ['CMSSW_BASE']
    add_path ='/src/Analysis/MssmHbb/datacards/'
    selection = '201701/18/OldWP/'
    signal_names = 'MssmHbbSignal_ReReco_oldWP'
    path_to_dir = ( CMSSW_BASE + add_path + selection )
    # Make dir if it doesn't exist
    MakeDir(path_to_dir)

    basename_of_template = 'histo_datacard_template.txt'
    path_to_template = CMSSW_BASE + add_path + basename_of_template

    masses = [300,350,400,500,600,700,900,1100,1300]
    for m in masses:
        #construct datacard name
        datacard_name = ConstructDataCardName(m)
        # construct path to datacard
        path_to_datacard = path_to_dir + datacard_name
        #Copy template datacard
        CopyTemplateDatacard(path_to_template,path_to_datacard)
        # Get dictionary for particular mass:
        current_dic = SetupDics(m,signal_names)
        AdjustDatacard(path_to_datacard,current_dic)

