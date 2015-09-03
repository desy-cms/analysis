sed -i "s/process.mix.input.fileNames/#process.mix.input.fileNames/g" $1
sed -i "93ifrom pu_sample import PUFILES" $1
sed -i "94iprocess.mix.input.fileNames = cms.untracked.vstring(PUFILES)" $1
