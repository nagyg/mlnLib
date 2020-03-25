
float pol_sizePowerMin = chf("../pol_sizepowermin");
float pol_sizePowerMax = chf("../pol_sizepowermax");
int pol_limitWithDir = chi("../pol_dirlimit");
float pol_directionLimitLow = chf("../pol_directionlimitlow");
float pol_directionLimitHigh = chf("../pol_directionlimithigh");
float pol_effectMin = chf("../pol_effectmin");
float pol_effectMax = chf("../pol_effectmax");
float pol_forceRadius = chf("../pol_forceradius");
float pol_forceStrength = chf("../pol_forcestrength");
float pol_rangeExp = chf("../pol_exponent");
int pol_amountLimit = chi("../pol_amountlimit");
float pol_priority = chf("../pol_priority");
int pol_writeAffectors = chi("../pol_writeaffectors");


float pol_pMult = fit01(pRatio, pol_effectMin, pol_effectMax);
int pol_affectedBy[];
int pol_count =0;
float pol_accum =0;
