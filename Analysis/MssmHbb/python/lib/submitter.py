#!/usr/bin/python

'''Class for submission jobs to naf/lxplus/shell

'''

import sys, os, commands
import logging
from subprocess import call
import uuid
ROOT_DIR = os.path.dirname(os.path.abspath(__file__)) + '/../' # full path of ROOT folder


class submitter(object):
    def __init__(self,type,command = '',parameters = ''):
        self.type = type
        self._command = command
        self._parameters = parameters

    def choose_cluster(type,parameters = ''):
        if type == 'naf':
            return naf()
        elif type == 'lxplus':
            return lxplus()
        elif type == 'shell':
            return shell()
        assert 0, 'Wrong cluster selected: ' + type
    choose_cluster = staticmethod(choose_cluster)

    def submit(self,cmd_args,basis_arr):
        """The main method.

        Used to submit jobs to naf/lxplus/shell use private methods that
        are overwritten in the derived classes
        """

        #check whether output exists:
        if not os.path.exists(cmd_args.output_dir):
            os.makedirs(cmd_args.output_dir)
        os.chdir(cmd_args.output_dir)

        # Count number of processed points:
        points = 0
        # Loop over the basises
        for basis in basis_arr:
            job = int(points/cmd_args.pointsPerJob) + 1

            if( self._StartNewJob(points, cmd_args.pointsPerJob) or points == 0):
                self.PrepareWorkDir(job,basis)

            self.AddOnePoint(job, basis)
            if( self._StartNewJob(points+1, cmd_args.pointsPerJob) or points == len(basis_arr) - 1):
                self.SubmitJob(job)

            points += 1

        print 'Submission DONE!'

    def PrepareWorkDir(self,job,par_set):
        """Method used to prepare working directory for farm usage.

        Also .csh scripts and datacards.in are prepared
        """

        root_dir = os.getcwd()
        # clear directory with the same name
        if os.path.exists('job_' + str(job)): call('rm -r job_' + str(job),shell=True)
        os.makedirs('job_' + str(job))
        os.chdir('job_' + str(job))#go to job_i

        # copy binary
        self.CopyFile(ROOT_DIR + 'bin', '.', 'sushi')

        #prepare new csh file to run at the farm
        newCsh = 'job_'+str(job)+'.csh'
        outCsh = open(newCsh, 'w')
        outCsh.write('#!/bin/csh' + '\n')
        outCsh.close()
        call('chmod +x ' + newCsh, shell=True)

        job_dir = os.getcwd()
        command1 = 'cd ' + job_dir
        self.UpdateSubmissionCsh(command1, newCsh)
        # Get back to the parent directory:
        os.chdir(root_dir)
        # self.AddOnePoint(job, par_set)

        # newCsh = self.UpdateSubmissionCsh(command1, newCsh)

    def AddOnePoint(self,job,par_set):
        """Method that doesn't create a directory but modify the csh.

        """
        root_dir = os.getcwd()
        # Change directory
        os.chdir('job_' + str(object=job))
        # Copy new datacard
        self.CopyFile(ROOT_DIR + 'datacards','.','2HDMC_' + str(par_set.basis) + '.in')
        # Modify datacard with current set of parameters
        card_name = self._UpdateDataCard(par_set)
        # Check whether csh script exists:
        cshFile = 'job_'+str(job)+'.csh'
        if not os.path.exists(cshFile):
            raise AttributeError("ERROR: No .csh file found at " + os.getcwd())
        command = './sushi ' + card_name + '.in ' + card_name + '.out' + ' > ' + card_name + '.log'
        cshFile = self.UpdateSubmissionCsh(command, cshFile)
        # Cat 2HDMC output in the same .out
        command2 = 'cat 2HDMC.out >> ' + card_name + '.out'
        cshFile = self.UpdateSubmissionCsh(command2, cshFile)
        # Delete output of the 2HDMC:
        command3 = 'rm 2HDMC.out'
        cshFile = self.UpdateSubmissionCsh(command3, cshFile)
        # Get back to the parent directory:
        os.chdir(root_dir)

    def _StartNewJob(self,points, pointsPerJob):
        """Private method to Check whether a folder for new job should be created.

        """
        flag = False
        if (points)%pointsPerJob == 0:
            flag = True
        return flag

    def SubmitJob(self,job):
        """Method to submit single job.

        """
        root_dir = os.getcwd()
        # Change directory
        os.chdir('job_' + str(object=job))
        thisCsh = 'job_' + str(job) + '.csh'
        command1 = 'tar -cvzf out_job_' + str(job) + '.tar.gz *.out'
        thisCsh = self.UpdateSubmissionCsh(command1, thisCsh)
        print 'submitting job',str(job)
#     cmd = 'bsub -q ' + queue + ' ' + thisCsh
        cmd = self._command + ' ' + self._parameters + ' ' + thisCsh
        call(cmd,shell=True)
        # self.processCmd(cmd)
        # Get back to the parent directory:
        os.chdir(root_dir)

    def processCmd(cmd, quite = 0):
        """Method to process cmd command for submission.

        """
        status, output = commands.getstatusoutput(cmd)
        if (status !=0 and not quite):
            print 'Error in processing command:\n   ['+cmd+']'
            print 'Output:\n   ['+output+'] \n'

    def CopyFile(self,oldLoc, newLoc, oldName, newName = ''):
        """Method to copy file from one directory to another.

        """
        #check whether directory exists
        if not os.path.exists(oldLoc):
            raise AttributeError("ERROR: Location " + oldLoc + " doens't exist. Please check spelling")

        # check if it's end with '/'
        if not oldLoc.endswith('/'):
            oldLoc += '/'

        #check whether input file exist:
        if not os.path.exists(oldLoc+oldName):
            raise AttributeError("ERROR: File " + oldName + " doens't exist at " + oldLoc + ". Please check spelling")

        # check whether new directory exists
        if not os.path.exists(newLoc):
            raise AttributeError("ERROR: Location " + newLoc + " doens't exist. Please check spelling")

        if not newLoc.endswith('/'):
            newLoc += '/'

        #copy file:
        call('cp ' + oldLoc + oldName + ' ' + newLoc + newName, shell=True)

    def UpdateSubmissionCsh(self,command, cshFile):
        """Method to update csh script.

        """
        # sanity check
        if not os.path.exists(cshFile):
            raise AttributeError("ERROR: File " + cshFile + " doens't exist. Please check spelling")
        call('echo $"' + command + '" >> ' + cshFile, shell=True)
        return cshFile

    def _UpdateDataCard(self,basis):
        """Methods to update the datacard with current configuration with physical basis.

        """
        if basis.basis == 'physicalbasis':
            temp_name = self._UpdatePhysicalbasisDataCard(basis.higgsType, basis.thdmType, basis.tanBeta, basis.m12, basis.mh, basis.mH, basis.mA, basis.mC, basis.sinB_A, basis.lambda6, basis.lambda7)
        elif basis.basis == 'lambdabasis':
            temp_name = self._UpdateLambdabasisDataCard(basis.higgsType, basis.thdmType, basis.tanBeta, basis.m12, basis.lambda1, basis.lambda2, basis.lambda3, basis.lambda4, basis.lambda5, basis.lambda6, basis.lambda7)
        return temp_name

    def _UpdatePhysicalbasisDataCard(self,higgsType,thdmType,tanBeta,m12,mh,mH,mA,mC,sinB_A,lambda6,lambda7):
        # Input data card
        name = '2HDMC_physicalbasis'
        templateInput = name + '.in'

        # Redefine the name of the input datacard and output
        # tempInputName = 'type' + str(int(thdmType)) + '_Htype_' + str(int(higgsType)) + '_tanB' + str(round(tanBeta,2)) + '_m12_' + str(round(m12,2)) \
        # + '_mh' + str(round(mh,2)) + '_mH' + str(round(mH,2)) + '_mA' + str(round(mA,2)) + '_mC' + str(round(mC,2)) + '_sinBA' + str(round(sinB_A,2)) + '_L6' + str(round(lambda6,2)) + '_L7' + str(round(lambda7,2));
        tempInputName = 'type' + str(int(thdmType)) + '_Htype_' + str(int(higgsType)) + '_' + str(object=uuid.uuid1()) #Couldn't use meaningfull name because of limitation on the string size fomr sushi side
        tempInput = tempInputName + '.in';

        # Check whether input datacard exists:
        if not os.path.exists(templateInput): raise BaseException('ERROR:PROBLEM in submitter::_UpdatePhysicalbasisDataCard - wrong dir')

        # Copy initial.in file to new name
        call('mv ' + templateInput + ' ' + tempInput,shell=True)
        # Change parameters inside the datacard
        # mh
        call("sed -i 's/HLOWM/" + str(mh) + "/g' " + tempInput,shell=True)
        # mH
        call("sed -i 's/HCAPM/" + str(mH) + "/g' " + tempInput,shell=True)
        # mA
        call("sed -i 's/HAMASS/" + str(mA) + "/g' " + tempInput,shell=True)
        # mC
        call("sed -i 's/HCMASS/" + str(mC) + "/g' " + tempInput,shell=True)
        # sin(beta-alpha)
        call("sed -i 's/SINB_A/" + str(sinB_A) + "/g' " + tempInput, shell=True)
        # lambda6
        call("sed -i 's/L6/" + str(lambda6) + "/g' " + tempInput, shell=True)
        # lambda7
        call("sed -i 's/L7/" + str(lambda7) + "/g' " + tempInput, shell=True)
        # thdm type
        call("sed -i 's/THDMTYPE/" + str(thdmType) + "/g' " + tempInput, shell=True)
        # higgs types
        call("sed -i 's/HIGGSTYPE/" + str(higgsType) + "/g' " + tempInput, shell=True)
        # tanBeta
        call("sed -i 's/TANBETA/" + str(tanBeta) + "/g' " + tempInput, shell=True)
        # m12
        call("sed -i 's/M12/" + str(m12) + "/g' " + tempInput, shell=True)

        return tempInputName

    def _UpdateLambdabasisDataCard(self,higgsType,thdmType,tanBeta,m12,lambda1,lambda2,lambda3,lambda4,lambda5,lambda6,lambda7):
        """with lambda basis.
        
        """
        # Input data card
        name = '2HDMC_lambdabasis'
        templateInput = name + '.in'

        # Redefine the name of the input datacard and output
        # tempInputName = 'type' + str(int(thdmType)) + '_Htype_' + str(int(higgsType)) + '_tanB' + str(round(tanBeta,2)) + '_m12' + str(round(m12,2)) \
        # + '_L1' + str(round(lambda1,2)) + '_L2' + str(round(lambda2,2)) + '_L3' + str(round(lambda3,2)) + '_L4' + str(round(lambda4,2)) + '_L5' + str(round(lambda5,2)) + '_L6' + str(round(lambda6,2)) + '_L7' + str(round(lambda7,2))
        tempInputName = 'type' + str(int(thdmType)) + '_Htype_' + str(int(higgsType)) + '_' + str(object=uuid.uuid1())
        tempInput = tempInputName + '.in';

        # Check whether input datacard exists:
        if not os.path.exists(templateInput): raise BaseException('ERROR:PROBLEM in submitter::_UpdateLambdabasisDataCard - wrong dir')

        # Copy initial.in file to new name
        call('mv ' + templateInput + ' ' + tempInput,shell=True)
        # Change parameters inside the datacard
        # lambda1
        call("sed -i 's/L1/" + str(lambda1) + "/g' " + tempInput,shell=True)
        # lambda2
        call("sed -i 's/L2/" + str(lambda2) + "/g' " + tempInput,shell=True)
        # lambda3
        call("sed -i 's/L3/" + str(lambda3) + "/g' " + tempInput,shell=True)
        # lambda4
        call("sed -i 's/L4/" + str(lambda4) + "/g' " + tempInput,shell=True)
        # lambda5
        call("sed -i 's/L5/" + str(lambda5) + "/g' " + tempInput, shell=True)
        # lambda6
        call("sed -i 's/L6/" + str(lambda6) + "/g' " + tempInput, shell=True)
        # lambda7
        call("sed -i 's/L7/" + str(lambda7) + "/g' " + tempInput, shell=True)
        # thdm type
        call("sed -i 's/THDMTYPE/" + str(thdmType) + "/g' " + tempInput, shell=True)
        # higgs types
        call("sed -i 's/HIGGSTYPE/" + str(higgsType) + "/g' " + tempInput, shell=True)
        # tanBeta
        call("sed -i 's/TANBETA/" + str(tanBeta) + "/g' " + tempInput, shell=True)
        # m12
        call("sed -i 's/M12/" + str(m12) + "/g' " + tempInput, shell=True)

        return tempInputName

class naf(submitter):
    def __init__(self,command = "qsub",parameters = "-cwd -V"):
        submitter.__init__(self,'naf',command,parameters)
        self._command = command
        self._parameters = parameters

    def __str__(self):
        return 'Naf: ' + self._command + self._parameters


class lxplus(submitter):
    def __init__(self,command = "bsub",parameters = "-cwd -V"):
        # TODO: test lxplus version!!!!!
        submitter.__init__(self,'lxplus',command,parameters)
        self._command = command
        self._parameters = parameters

    def __str__(self):
        return 'lxplus used for submission'

class shell(submitter):
    def __init__(self):
        submitter.__init__(self,'shell')

    def __str__(self):
        return 'shell used for submission'

    # Redefine submit method
    def submit(self,cmd_args,basis_arr):
        #check whether output exists:
        if not os.path.exists(cmd_args.output_dir):
            os.makedirs(cmd_args.output_dir)
        os.chdir(cmd_args.output_dir)

        # copy binary
        self.CopyFile(ROOT_DIR + 'bin', '.', 'sushi')
        # Copy new datacard
        self.CopyFile(ROOT_DIR + 'datacards','.','2HDMC_' + str(basis_arr[0].basis) + '.in')
        # Modify datacard with current set of parameters
        card_name = self._UpdateDataCard(basis_arr[0])
        # Run sushi
        command = './sushi ' + card_name + '.in ' + card_name + '.out' + ' > ' + card_name + '.log'
        call(command,shell=True)

        print 'Submission DONE!'
