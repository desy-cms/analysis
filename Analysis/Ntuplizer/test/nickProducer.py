import re
from grid_control import datasets, DatasetError
from grid_control.datasets import NickNameProducer

def lookup(data, keyMap, default = None):
	for key in keyMap:
		if key in data:
			return keyMap[key]
	return (default, '')[default == None]

def lookupBegin(data, keyMap, default = None):
	print data
	for key in keyMap:
		if data.startswith(key):
			return keyMap[key]
	return (default, '')[default == None]


def addPart(data):
	return ('_%s' % data, '')[data == '']

def parseCuts(data):
	tmp = re.findall('([0-9]+)to([0-9]+)', data.lower())
	if tmp:
		return tmp[0]
	return filter(lambda x: int(x) > 0, re.findall('pt[-_]*([0-9]+)', data.lower()))

#/SUSYBBHToTauTau_M-*_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-*/AODSIM
#/SUSYGluGluToHToTauTau_M-*_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-*/AODSIM
#/VBF_HToTauTau_M-*_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-*/AODSIM
#/GluGluToHToTauTau_M-*_7TeV-powheg-pythia6/Summer11-PU_S4_START42_V11-*/AODSIM
#/WH_ZH_TTH_HToTauTau_M-*_7TeV-pythia6-tauola/Summer11-PU_S4_START42_V11-*/AODSIM


class signalNicks(NickNameProducer):
	def getName(self, oldnick, dataset, block):
		if oldnick != '':
			return oldnick

		try:
			(pd, sd, tier) = dataset.lstrip('/').split('/')
			#(process, masspart, other) = pd.split("_")
			pdParts = pd.split("_")
			#print pd, sd, tier, pdParts
		
			processMap = {
				"SUSYBBHToTauTau" : "SUSYBBHToTauTau",
				"SUSYGluGluToHToTauTau" : "SUSYGluGluToHToTauTau",
				"VBF_HToTauTau" : "VBFHToTauTau",
				"VBFHToTauTau" : "VBFHToTauTau",
				"GluGluToHToTauTau" : "SMGluGluToHToTauTau",
				"WH_ZH_TTH_HToTauTau" : "WHZHTTHToTauTau",
				"GluGluToHToWWTo2LAndTau2Nu" : "SMGluGluToHToWWTo2LAndTau2Nu",
				"VBF_HToWWTo2LAndTau2Nu" : "VBFHToWWTo2LAndTau2Nu",
			}

			tuneMap = {
				'D6T': 'D6T',
				'pythia8': 'T1',
				'ProPT0': 'ProPT0',
				'Z2': 'Z2',
				'Zstar2': 'Z2star',
			}
			genMap = {
				'powheg': 'powheg',
				'madgraph': 'madgraph',
				'alpgen': 'alpgen',
				'herwigjimmy': 'herwigjimmy',
				'herwig': 'herwig',
				'pythia8': 'pythia8',
				'pythia': 'pythia',
				'minloHJJ': 'minloHJJ',
			}

			condMap = {
				'Summer11-PU_S3_START42_V11': 'SU11S42v11PUS3',
				'Summer11-PU_S4_START42_V11': 'SU11S42v11PUS4',
				'Fall11-PU_S6_START42_V14B': 'FA11S42v14BPUS6',
				'Summer12-PU_S7_START52_V9': 'SU12S52v9PUS7',
				'Summer12_DR53X-PU_S10_START53_V7A': 'SU12S53v7APUS10',
				'Summer12_DR53X-PU_S10_START53_V7C': 'SU12S53v7CPUS10',
				'Summer12_DR53X-PU_S10_START53_V19': 'SU12S53v19PUS10',
			}

			nick = lookupBegin(pd, processMap, '!')	
			if "lepdecay" in pd:
				nick += addPart("lepdecay")

			nick += addPart(lookup(pd, tuneMap, 'Z2star'))
			nick += addPart(lookup(pd, genMap, '!'))
			nick += addPart('%04d' % int(re.search('M-([0-9]+)', pd).group(1)))
			nick += addPart(lookup(sd, condMap, '!'))
			nick += addPart(re.search('-(v[0-9]?)', dataset).group(1))
			print "result:"
			print "\t", dataset
			print "\t", nick
		except:
			print oldnick, dataset
			raise
		if "!" in nick:
			print ""
			print "Dataset:  %s" % dataset
			print "Nickname: %s" % nick
			raise "Automatic nick name creation failed! Please check the code of the nickname source!"
		return nick

Summer12SignalNicks=signalNicks
Summer11SignalNicks=signalNicks
Fall11SignalNicks=signalNicks
