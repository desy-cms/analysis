

namespace constants {

  //// Backgrounds ////

  //  Z extrapolation
  static const double ratioZToNuNuZToLL = 5.651; 	//from MCFM 6465/1144 = 5.651
  static const double sigma_Zvv_QCD = 5.651*(3503710/3);
  static const double sigma_Zvv_EWK = 460.*3;
  static const double sigma_Zuu_QCD = 3503710.0/3;
  static const double sigma_Zuu_EWK = 303.;

  // background values and total uncertainty (temporary!)
  static const double nBG_WTau   = 0.;
  static const double stat_WTau  = 0.;


  // systematics (these are fractional values not percentages)
  //   these should not include MC stats !
  static const double syst_effMu      = 0.025;   // no need after applied lepton ID
  static const double syst_lumi       = 0.026;   // 2.6% from LUM POG

  // Z->nunu systematics
  static const double syst_Z          = 0.20;    // 20% extrapolation systematics
  static const double syst_Z_El_m     = 0.0;
  static const double syst_Z_El_p     = 0.0;
  static const double syst_Z_Mu_m     = 0.049;
  static const double syst_Z_Mu_p     = -0.039;
  static const double syst_Z_JES_m    = -0.058;
  static const double syst_Z_JES_p    = -0.006;
  static const double syst_Z_JER_m    = -0.017;
  static const double syst_Z_JER_p    = 0.016;
  static const double syst_Z_UES_m    = -0.016;
  static const double syst_Z_UES_p    = -0.0018;

  // W->munu
  static const double syst_WMu        = 0.20;    // 20% extrapolation systematics
  static const double syst_WMu_El_m   = 0.0;
  static const double syst_WMu_El_p   = 0.0;
  static const double syst_WMu_Mu_m   = 0.029;
  static const double syst_WMu_Mu_p   = -0.015;
  static const double syst_WMu_JES_m  = 0.001;
  static const double syst_WMu_JES_p  = -0.029;
  static const double syst_WMu_JER_m  = -0.060;
  static const double syst_WMu_JER_p  = 0.010;
  static const double syst_WMu_UES_m  = -0.002;
  static const double syst_WMu_UES_p  = -0.004;

  // W->enu
  static const double syst_WEl         = 0.20;    // 20% extrapolation systematics
  static const double syst_WEl_El_m    = 0.028;   //from Analysis B
  static const double syst_WEl_El_p    = -0.029;  //form Analysis B
  static const double syst_WEl_Mu_m    = 0.0;
  static const double syst_WEl_Mu_p    = 0.0;
  static const double syst_WEl_JES_m   = 0.033;
  static const double syst_WEl_JES_p   = -0.128;
  static const double syst_WEl_JER_m   = 0.149;
  static const double syst_WEl_JER_p   = -0.032;
  static const double syst_WEl_UES_m   = 0.018;
  static const double syst_WEl_UES_p   = -0.024;

  // W->taunu
  static const double syst_WTau       = 0.22;    // 20% extrapolation + 8% tau ID + 5% lepton contamination
  static const double syst_WTau_El_m  = -0.01;   //from Analysis B
  static const double syst_WTau_El_p  = 0.011;   //form Analysis B
  static const double syst_WTau_Mu_m  = 0.0001;
  static const double syst_WTau_Mu_p  = -0.0002;
  static const double syst_WTau_JES_m = 0.076;
  static const double syst_WTau_JES_p = 0.019;
  static const double syst_WTau_JER_m = -0.022;
  static const double syst_WTau_JER_p = -0.036;
  static const double syst_WTau_UES_m = 0.007;
  static const double syst_WTau_UES_p = 0.004;

  // QCD 1
  static const double syst_QCD1        = 1.5;      // QCD 1 is not used any more
  static const double syst_QCD1_JES_m  = 0.0;
  static const double syst_QCD1_JES_p  = 0.0;
  static const double syst_QCD1_JER_m  = 0.0;
  static const double syst_QCD1_JER_p  = 0.0;
  static const double syst_QCD1_UES_m  = 0.0;
  static const double syst_QCD1_UES_p  = 0.0;

  // QCD 3
  static const double syst_QCD3        = 0.40;     // 40% shape systematic
  static const double syst_QCD3_El_m   = -0.015;   //0.03/0.02 * 0.01
  static const double syst_QCD3_El_p   = 0.039;    //0.07/0.02 * 0.011
  static const double syst_QCD3_Mu_m   = -0.002;
  static const double syst_QCD3_Mu_p   = -0.0009;
  static const double syst_QCD3_JES_m  = 0.529;
  static const double syst_QCD3_JES_p  = -0.586;
  static const double syst_QCD3_JER_m  = 0.163;
  static const double syst_QCD3_JER_p  = -0.182;
  static const double syst_QCD3_UES_m  = 0.029;
  static const double syst_QCD3_UES_p  = -0.019;

  // ttbar
  static const double syst_TTbar         = 0.07;   // 7% x-section
  static const double syst_TTbar_El_m	 = 0.0;
  static const double syst_TTbar_El_p	 = 0.0;
  static const double syst_TTbar_Mu_m	 = 0.0;
  static const double syst_TTbar_Mu_p	 = 0.0;
  static const double syst_TTbar_JES_m   = -0.53;
  static const double syst_TTbar_JES_p   = 0.028;
  static const double syst_TTbar_JER_m   = -0.53;
  static const double syst_TTbar_JER_p   = 0.028;
  static const double syst_TTbar_UES_m   = 0.0;
  static const double syst_TTbar_UES_p   = 0.0;

  // diboson with WG
  static const double syst_Diboson       = 0.18;   // 18% x-section
  static const double syst_Diboson_El_m  = 0.005;  //0.01/0.02 * 0.01
  static const double syst_Diboson_El_p  = -0.006; //0.01/0.02 * 0.011
  static const double syst_Diboson_Mu_m  = 0.002;
  static const double syst_Diboson_Mu_p  = -0.002;
  static const double syst_Diboson_JES_m = -0.327;
  static const double syst_Diboson_JES_p = -0.022;
  static const double syst_Diboson_JER_m = -0.013;
  static const double syst_Diboson_JER_p = -0.26;
  static const double syst_Diboson_UES_m = -0.0001;
  static const double syst_Diboson_UES_p = 0.003;

  // doboson no WG
  static const double syst_DibosonNoWG       = 0.18;   // 18% x-section
  static const double syst_DibosonNoWG_El_m  = 0.02;   //0.04/0.02 * 0.01
  static const double syst_DibosonNoWG_El_p  = -0.022;  //0.04/0.02 * 0.011
  static const double syst_DibosonNoWG_Mu_m  = 0.0;
  static const double syst_DibosonNoWG_Mu_p  = 0.0;
  static const double syst_DibosonNoWG_JES_m = -0.305;
  static const double syst_DibosonNoWG_JES_p = 0.106;
  static const double syst_DibosonNoWG_JER_m = -0.051;
  static const double syst_DibosonNoWG_JER_p = 0.078;
  static const double syst_DibosonNoWG_UES_m = -0.0002;
  static const double syst_DibosonNoWG_UES_p = 0.031;

  // WG
  static const double syst_WG		 = 0.18;   // 18% x-section
  static const double syst_WG_El_m  	 = 0.0;  	
  static const double syst_WG_El_p  	 = 0.0; 
  static const double syst_WG_Mu_m  	 = 0.003;
  static const double syst_WG_Mu_p  	 = -0.003;
  static const double syst_WG_JES_m 	 = -0.33;
  static const double syst_WG_JES_p 	 = -0.067;
  static const double syst_WG_JER_m 	 = 0.0;
  static const double syst_WG_JER_p 	 = -0.377;
  static const double syst_WG_UES_m 	 = 0.0;
  static const double syst_WG_UES_p 	 = -0.006;

  // single T
  static const double syst_SingleT       = 0.16;   // 16% x-section
  static const double syst_SingleT_El_m  = 0.045;  //0.09/0.02 * 0.01
  static const double syst_SingleT_El_p  = -0.05;  //0.09/0.02 * 0.011
  static const double syst_SingleT_Mu_m  = 0.0;
  static const double syst_SingleT_Mu_p  = 0.0;
  static const double syst_SingleT_JES_m = -0.226;
  static const double syst_SingleT_JES_p = 0.509;
  static const double syst_SingleT_JER_m = 0.258;
  static const double syst_SingleT_JER_p = 0.119;
  static const double syst_SingleT_UES_m = -0.001;
  static const double syst_SingleT_UES_p = 0.0;

  // DY
  static const double syst_DYLL          = 0.20;   // 20% x-section
  static const double syst_DYLL_El_m	 = 0.015;  //0.03/0.02 * 0.01
  static const double syst_DYLL_El_p	 = -0.017; //0.03/0.02 * 0.011
  static const double syst_DYLL_Mu_m	 = 0.005;
  static const double syst_DYLL_Mu_p	 = 0.002;
  static const double syst_DYLL_JES_m    = -0.223;
  static const double syst_DYLL_JES_p    = 0.359;
  static const double syst_DYLL_JER_m    = -0.203;
  static const double syst_DYLL_JER_p    = 0.514;
  static const double syst_DYLL_UES_m    = -0.002;
  static const double syst_DYLL_UES_p    = 0.004;

  // Summed all minor BGs
  static const double syst_Other_El_m	 = 0.01;    //0.02/0.02 * 0.01
  static const double syst_Other_El_p	 = -0.011;  //0.02/0.02 * 0.011
  static const double syst_Other_Mu_m	 = 0.0019;
  static const double syst_Other_Mu_p	 = -0.0014;
  static const double syst_Other_JES_m   = -0.317;
  static const double syst_Other_JES_p   = 0.09;
  static const double syst_Other_JER_m   = -0.026;
  static const double syst_Other_JER_p   = -0.12;
  static const double syst_Other_UES_m   = -0.0004;
  static const double syst_Other_UES_p   = 0.003;

  // Summed all minor BGs no WG
  static const double syst_OtherNoWG_El_m    = 0.025;    //0.05/0.02 * 0.01
  static const double syst_OtherNoWG_El_p    = -0.028;   //0.05/0.02 * 0.011
  static const double syst_OtherNoWG_Mu_m    = 0.0009;
  static const double syst_OtherNoWG_Mu_p    = 0.0004;
  static const double syst_OtherNoWG_JES_m   = -0.299;
  static const double syst_OtherNoWG_JES_p   = 0.288;
  static const double syst_OtherNoWG_JER_m   = -0.053;
  static const double syst_OtherNoWG_JER_p   = 0.16;
  static const double syst_OtherNoWG_UES_m   = -0.0008;
  static const double syst_OtherNoWG_UES_p   = 0.012;

  //// Signal properties ////

  // masses
  static const int nM = 14;
  static const double mH[nM]  = { 100.,  120.,  160.,  200., 250., 300., 350., 400., 500., 600., 700., 900., 1100., 1300.};  // GeV

  // cross-sections from YR2
  static const double xsH[nM] = { 5.23, 41.79, 75.32, 35.42, 15.55, 7.64, 4.1, 2.34, 0.88, 0.38, 0.18, 0.05, 0.018, 0.008};  // pb
  
  // systematics
  static const double syst_Sig_QCD[6]    = { 0.04,    0.04,    0.04,    0.04,    0.04,   0.04 };
  static const double syst_Sig_PDF[6]    = { 0.041,   0.046,   0.043,   0.047,   0.054,  0.055 };
  static const double syst_Sig_El_m[6]   = { 0., 0., 0., 0., 0., 0. };  //no effect on signal
  static const double syst_Sig_El_p[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_Mu_m[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_Mu_p[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_JES_m[6]  = { -0.077,  -0.121,  -0.094,  -0.111,  -0.081,  -0.11 };
  static const double syst_Sig_JES_p[6]  = { 0.109,  0.096,  0.083,  0.121,  0.053,  0.10 };
  static const double syst_Sig_JER_m[6]  = { -0.0004,  -0.033,  -0.019,  -0.026,  -0.023,  -0.035 };
  static const double syst_Sig_JER_p[6]  = { 0.034,  0.024,  0.019,  0.020,  0.004,  0.024 };
  static const double syst_Sig_UES_m[6]  = { -0.006,  -0.007,  -0.005,  -0.007,  -0.009,  0.0009 };
  static const double syst_Sig_UES_p[6]  = { 0.004,  0.004,  0.002,  0.0006,  0.004,  0.004 };


  //// ggH signal properties ////

  // cross-sections from YR2
  static const double xsggH[6] = { 24.70, 19.27, 13.55, 7.081, 3.594, 2.921 };  // pb

  // systematics
  static const double syst_Sig_ggH_QCD[6]    = { 0.604, 0.604, 0.604, 0.604, 0.604, 0.604 };
  static const double syst_Sig_ggH_PDF[6]    = { 0.113, 0.113, 0.113, 0.113, 0.113, 0.113 };
  static const double syst_Sig_ggH_El_m[6]   = { 0., 0., 0., 0., 0., 0. };  //no effect on signal
  static const double syst_Sig_ggH_El_p[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_ggH_Mu_m[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_ggH_Mu_p[6]   = { 0., 0., 0., 0., 0., 0. };
  static const double syst_Sig_ggH_JES_m[6]  = { -0.02, -0.24, -0.002, -0.14, -0.06, -0.008 };
  static const double syst_Sig_ggH_JES_p[6]  = { 0.17, 0.13, 0.13, 0.14, 0.08, 0.16 };
  static const double syst_Sig_ggH_JER_m[6]  = { -0.002, 0.13, -0.001, -0.13, -0.02, 0.008 };
  static const double syst_Sig_ggH_JER_p[6]  = { 0.23, 0., 0.013, 0.07, -0.048, 0.086 };
  static const double syst_Sig_ggH_UES_m[6]  = { -0.009, 0., -0.001, -0.003, 0.02, 0. };
  static const double syst_Sig_ggH_UES_p[6]  = { 0.003, 0., 0.112, 0., 0.01, 0.036 };

}

