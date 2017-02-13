#!/usr/bin/python

from ROOT import TTree,TFile,TH3F
from array import array
import math

def TranslateCosB_To_sinA(cos_bins):
    """Function to trasfer cos(beta-alpha) binning in conv. B to sin(beta-alpha) in conv. A.

    convention B: 0 < beta-alpha < Pi
    convention A: -90 < beta-alpha < 90
    """
    sin_beta_alpha_A = []
    for p in cos_bins:
        #calculate the sin
        s_beta_alpha_B = math.sqrt(1 - p**2)
        #translate convention B to A
        s_beta_alpha_A = s_beta_alpha_B
        if p < 0:
            s_beta_alpha_A *= -1
        s_beta_alpha_A = round(s_beta_alpha_A,5)
        sin_beta_alpha_A.append(s_beta_alpha_A)
    return sin_beta_alpha_A

def MakeBins(l):
#    print l
    L = array('f')
    L.append(l[0] - (l[1]-l[0])/2)
    #make bin boundaries
    for i in range(len(l)):
#        print l[i], ' len = ', len(l), 'L = ', l[i]+(l[i]-L[-1])
#        print l[i], L[-1]
        L.append(l[i]+(l[i]-L[-1]))
    #check if bin boundaries are in increasing order
    for i in range(len(L)-1):
#        print L[i], L[i+1], len(L)
        if L[i+1]-L[i] <= 0:
            raise RuntimeError("Check bin boundaries!!!")

    return L

#tanBetas
# tanBetaRange = [1.,100.]
# tanBetaStep = 1
# tanBetas = [ tanBetaRange[0]+tanBetaStep*i for i in range(int((tanBetaRange[1]-tanBetaRange[0])/tanBetaStep+1)) ]
#tanBetas
tanBetaLowRange = [0.5,2]
tanBetaLowStep  = 0.05
tanBetasLow = [ round(tanBetaLowRange[0]+tanBetaLowStep*i,3) for i in range(int((tanBetaLowRange[1]-tanBetaLowRange[0])/tanBetaLowStep)) ]
tanBetaRange = [2.,100.]
tanBetaStep = 0.5
tanBetas = [ tanBetaRange[0]+tanBetaStep*i for i in range(int((tanBetaRange[1]-tanBetaRange[0])/tanBetaStep+1)) ]
tanBetas = tanBetasLow + tanBetas

#H masses
mH = [200,250,300,350,400,500,600,700,800,900,1100,1300]
#h masses
mh = [125.]
#sin(beta - alpha)
cosB_A_Range = [-1,1]
cosB_A_step = 0.005
cosB_As = [cosB_A_Range[0] + cosB_A_step* i for i in range(int((cosB_A_Range[1] - cosB_A_Range[0])/cosB_A_step + 1))]
sinB_As = TranslateCosB_To_sinA(cosB_As)


type_boson = 'type1_mH'
production = 'production_cosB_A_-1_1_tanB_0p5-100_COMBINATION'
inputTxt = '/nfs/dust/cms/user/shevchen/SusHiScaner/output/' + production + '/txtFiles/' + type_boson + '.txt'

# names of the column
lines = [ line.split() for line in open(inputTxt, 'r')]
histNames = lines[0]
print histNames
mH_bin = MakeBins(mH)
tanBetas_bin = MakeBins(tanBetas)
# sinB_As_bin = MakeBins(sinB_As)
cosB_As_bin = MakeBins(cosB_As)

print 'TAN BETAs:'
print(tanBetas)
print 'cos(beta-alpha)s:'
print sinB_As

# print sinB_As_bin
hists = [ TH3F(histNames[i], ";mH;tan(#beta);cos(#beta-#alpha)", len(mH_bin)-1, mH_bin, len(tanBetas_bin)-1, tanBetas_bin, len(cosB_As_bin)-1, cosB_As_bin) for i in range(5, len(histNames))]

# Ouptut file
output_name = '/nfs/dust/cms/user/shevchen/SusHiScaner/output/' + production + '/rootFiles/Histograms3D_' + type_boson + '.root'
# output_name = './Histograms3D_' + type_boson + '.root'
f = TFile(output_name,'recreate')
for i in range(1, len(lines)):
    for j in range(5, len(histNames)):

        massH   = float(lines[i][3])
        tanBeta = float(lines[i][0])
        sinB_A  = float(lines[i][1])
            
        
        #Round
        sinB_A = round(sinB_A,5)
        #special index settings to have cos(B-A) = 1 in the histogram
        sinB_A_index = sinB_As.index(sinB_A)
        if lines[i][1] == '-0.00000000E+00': sinB_A_index = 400
    
#         if massH == 300 and tanBeta == 20 and sinB_A == 0.9: print 'tanBeta: %0.2f, sin(beta-alpha): %0.3f, mH = %f, val = %0.3f' % (tanBeta, sinB_A, massH,float(lines[i][j])) + ' hist name: ' +  histNames[j]

#         print sinB_A
        # if [st not in histNames[j] for st in ('unitarity','perturbativity','stability')]:
        #print(' WTF: mH = %f , tanB = %f , sinB_A = %f , VAL = %f ' % ((massH),(tanBeta),(sinB_A), float(lines[i][j])))
        hists[j-5].SetBinContent(mH.index(massH) + 1, tanBetas.index(tanBeta) + 1, sinB_A_index + 1, float(lines[i][j]))

        if i == len(lines)-1:
            hists[j-5].Write()
# if histNames[j] != 'unitarity' and histNames[j] != 'perturbativity' and histNames[j] != 'stability':
f.Close()