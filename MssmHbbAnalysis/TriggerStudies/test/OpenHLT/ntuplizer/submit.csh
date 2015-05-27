

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "30to50"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(1.615e08)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "50to80"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(2.211e07)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "80to120"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(3.0001143e06)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "120to170"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(4.932e05)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "170to300"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(1.203e05)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "300to470"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(7.475e03)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "470to600"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(5.871e02)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "600to800"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(1.67e02)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "800to1000"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(2.825e01)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "1000to1400"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(8.195e00)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "1400to1800"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(7.346e-01)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
sed -e 's/BIN = ""/BIN = "1800"/g' crabConfig.py.temp > crabConfig.py
sed -e 's/CrossSection    = cms.double(1)/CrossSection    = cms.double(1.091e-01)/g' ntuplizer_openhlt_producer_cfg.py.temp > ntuplizer_openhlt_producer_cfg.py
crab submit

rm -f ntuplizer_openhlt_producer_cfg.py crabConfig.py
