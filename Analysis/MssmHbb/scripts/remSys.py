#!/usr/bin/python

import os


cmsswBase = os.environ['CMSSW_BASE']
input_dir = cmsswBase + "/src/Analysis/MssmHbb/datacards/"
output_dir = input_dir #+ "OneByOne/"
mass_points = [100,120,160,200,250,300,350,400,500,600,700,900,1100,1300]
syst = ["CMS_JES_13TeV","CMS_JER_13TeV","CMS_PU_13TeV","CMS_PtEff_13TeV","CMS_SFl_13TeV","CMS_SFb_13TeV","CMS_PAR0_13TeV","CMS_PAR1_13TeV","CMS_PAR2_13TeV","CMS_lumi_13TeV","CMS_onlBtag_13TeV"]

#mass_points = [100]
#syst = ["CMS_JES_13TeV", "CMS_JER_13TeV"]

for s in syst:
	#create file with list of .root combine outputs
	limit_file_name="Hbb.limits.Remove_" + s
	limit_file = open(output_dir + limit_file_name,"w")
	print limit_file_name + " at " + input_dir + " was opened for writing!"
	print ("cd " + input_dir + "; PlotLimit -i " + limit_file_name + " -o " + s)
	for m in mass_points:
		data_card = "hbb_mbb" + str(m) + "_mssm-13TeV.txt"
		
		#read file, line by line:
		temp_card = input_dir + "temp_" + s + "_" + data_card
		with open(input_dir + data_card,"r") as oldfile, open(temp_card,"w") as newfile:
			for line in oldfile:
				if s in line:
#					if "shape" in line or "lnN" in line or "gnN" in line:
#						print line + " was excluded!"
#					else:
#						newfile.write(line)
					continue
				else:
					newfile.write(line)
			print ("Line with " + s + " in " + str(m) + " GeV file was deleted")
		print "Run combine tool for this configuration"
		bash_combine = "cd " + input_dir + "; combine -M Asymptotic -n HbbRemove_" + s + " -m " + str(m) + " " + temp_card + " ; cd - "
		os.system(bash_combine)
		os.system("rm " + temp_card)
		root_card = "higgsCombineHbbRemove_" + s + ".Asymptotic.mH" + str(m) + ".root\n"
		limit_file.write(root_card)
	bash_plotLimit = "cd " + input_dir + "; PlotLimit -i " + limit_file_name + " -o " + s
        oldfile.close()
        newfile.close()
	limit_file.close()

	os.system(bash_plotLimit)

