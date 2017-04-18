#!/usr/bin/python

from ROOT import TTree,TFile,TH1F,TCanvas,TLegend
from array import array
import math
import os

def Renormalise(h):
    h.Scale(1./h.Integral())

cmsswBase = os.environ['CMSSW_BASE']

pu_distr_folder = "/src/Analysis/MssmHbb/bin/input_corrections/"
# ReReco MC
fMC = TFile(cmsswBase + pu_distr_folder + "MC_Autumn16_PU25ns_V1.root")
# ReReco Data
fData_Central  = TFile(cmsswBase + pu_distr_folder + "Data_Pileup_2016_ReReco.root")            # Central 
fData_2SigmaUp = TFile(cmsswBase + pu_distr_folder + "Data_Pileup_2016_ReReco_up2Sigma.root")   # Rereco Data 2Sigma Up
fData_2SigmaDn = TFile(cmsswBase + pu_distr_folder + "Data_Pileup_2016_ReReco_down2Sigma.root") # Rereco Data 2Sigma Down 

hMC = fMC.Get("pileup")
Renormalise(hMC)
hMC.SetLineColor(1)
hData_Central = fData_Central.Get("pileup")
hData_Central.SetMarkerStyle(20)
# hData_Central.Rebin(10)
Renormalise(hData_Central)
hData_2SigmaUp = fData_2SigmaUp.Get("pileup")
hData_2SigmaUp.SetMarkerStyle(20)
hData_2SigmaUp.SetMarkerColor(4)
# hData_2SigmaUp.Rebin(10)
Renormalise(hData_2SigmaUp)
hData_2SigmaDn = fData_2SigmaDn.Get("pileup")
hData_2SigmaDn.SetMarkerStyle(20)
hData_2SigmaDn.SetMarkerColor(2)
# hData_2SigmaDn.Rebin(10)
Renormalise(hData_2SigmaDn)

#Legend
leg = TLegend(0.6,0.6,0.9,0.9)
leg.AddEntry(hMC,"MC","l")
leg.AddEntry(hData_Central,"Data, central", "p")
leg.AddEntry(hData_2SigmaUp,"Data, 2G up", "p")
leg.AddEntry(hData_2SigmaDn,"Data, 2G down", "p")

canva = TCanvas("can","can",800,600)

hMC.Draw("hist")
hMC.SetMaximum(1.1*hData_2SigmaDn.GetMaximum())
hMC.SetStats(0)
hData_Central.Draw("same")
hData_2SigmaUp.Draw("same")
hData_2SigmaDn.Draw("same")
leg.Draw()

canva.Print("can.pdf")