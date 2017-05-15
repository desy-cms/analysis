#!/usr/bin/python
"""Script to create PU distribution in MC from deffined probabilities.

"""

import ROOT

def createHisto(name, probValue):
    """Funcction to create histogram according to the probabilities
    
    """
    histogram = ROOT.TH1F(name,"",60,0,60)
    for x in range(1,len(probFunctionVariable) + 1):
         histogram.SetBinContent(x,x-1,probValue[x-1])
    return histogram

def createRootFile(name):
    file = ROOT.TFile(name,"recreate")
    return file

# Define array of probabilities
# For 2016 ICHEP dataset
# from: https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_8_patchX/SimGeneral/MixingModule/python/mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi.py#L25
probFunctionVariable = (0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49)
probValue = (
         0.000829312873542,
         0.00124276120498,
         0.00339329181587,
         0.00408224735376,
         0.00383036590008,
         0.00659159288946,
         0.00816022734493,
         0.00943640833116,
         0.0137777376066,
         0.017059392038,
         0.0213193035468,
         0.0247343174676,
         0.0280848773878,
         0.0323308476564,
         0.0370394341409,
         0.0456917721191,
         0.0558762890594,
         0.0576956187107,
         0.0625325287017,
         0.0591603758776,
         0.0656650815128,
         0.0678329011676,
         0.0625142146389,
         0.0548068448797,
         0.0503893295063,
         0.040209818868,
         0.0374446988111,
         0.0299661572042,
         0.0272024759921,
         0.0219328403791,
         0.0179586571619,
         0.0142926728247,
         0.00839941654725,
         0.00522366397213,
         0.00224457976761,
         0.000779274977993,
         0.000197066585944,
         7.16031761328e-05,
         0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
         0.0,
         0.0,
        0.0 )
 
file = createRootFile("MC_Spring16_PU25ns_V1.root")
h = createHisto("pileup",probValue)
h.Write()
file.Close()