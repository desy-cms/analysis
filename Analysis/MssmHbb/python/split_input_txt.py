#!/usr/bin/env python
"""Script to split large .txt files with root files.

"""
import argparse
import os
import glob
import shutil
from subprocess import call, Popen

__author__ = "Rostyslav Shevchenko"
__email__ = "rostyslav.shevchenko@desy.de"

def NameOfSplitDir(input_dir):
    """Method to get the name for splitted files.
    
    """
    name = os.path.abspath(input_dir + '/../') + '/' + os.path.basename(input_dir) + '_split'
    return name

def MakeCleanDir(path_to_dir):
    """Method to create clean dir.
    
    """
    if os.path.exists(path_to_dir):
        shutil.rmtree(path_to_dir)
        # create new one
    os.makedirs(path_to_dir)
    # return the path
    path_to_dir = os.path.abspath(os.path.join(os.getcwd(), path_to_dir))
    return path_to_dir


def SplitFile(file,options):
    """Method to split file using gnu split.
    
    """
    basename = os.path.splitext(file)[0] + '_'
    command = 'split ' + file + ' ' + basename + ' ' + options
    call(command,shell=True)
    
    return basename

def MoveFile(splitted_file,dir_name):
    """Method to move files to another dir.
    
    """
    shutil.move(splitted_file,dir_name)

def AddExtension(splitted_file):
    """Method to add .txt extension to splitted files.
    
    """
    extension = '.txt' 
    new_name = splitted_file + extension
    os.rename(splitted_file,new_name)
    return new_name

def ParseOption():
    """Method to parse user input.
    
    """
    parser = argparse.ArgumentParser(description='Splitter of txt files')
    parser.add_argument('-i',       dest='input_folder',type=str, help='Input folder')
    parser.add_argument('-l',       dest='number_of_roots',type=int, help='Number of root files in the ouput files', default=2)
    args = parser.parse_args()
    # convert output_dir to absolute path
    args.input_folder = os.path.abspath(os.path.join(os.getcwd(), args.input_folder))
    
    return args

if __name__ == '__main__':
    
    args = ParseOption()
    options = '-l ' + str(args.number_of_roots) + ' -d'
    #Make new dir for splitted files:
    split_folder_name = NameOfSplitDir(args.input_folder)
    MakeCleanDir(split_folder_name)
    
    for file in glob.glob(args.input_folder + '/*.txt'):
        #Split file:
        splitted = SplitFile(file,options)
        for splitted_file in glob.glob(splitted + '*'):
            name_with_ext = AddExtension(splitted_file)
            MoveFile(name_with_ext,split_folder_name)