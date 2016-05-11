#!/bin/zsh
#$ -S /bin/zsh
#$ -l h_rt=23:59:59
#$ -j y
#$ -cwd
#$ -l site=hh
#$ -V
#$ -m ae

if [[ ${#} -ne 1 ]]
then
    echo "You have to specify the input path."
    exit 1
fi

echo "Job started at: $(date)\n"
analyze_background_model_studies.py -i ${1}
echo "Job ended at: $(date)\n"
