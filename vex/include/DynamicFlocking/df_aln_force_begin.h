
float aln_sizePowerMin = chf("../aln_sizepowermin");
float aln_sizePowerMax = chf("../aln_sizepowermax");
int aln_limitWithDir = chi("../aln_limitwithdirection");
float aln_directionLimitLow = chf("../aln_directionlimitlow");
float aln_directionLimitHigh = chf("../aln_directionlimithigh");
float aln_effectMin = chf("../aln_effectmin");
float aln_effectMax = chf("../aln_effectmax");
float aln_forceRadius = chf("../aln_forceradius");
float aln_forceStrength = chf("../aln_forcestrength");
float aln_rangeExp = chf("../aln_exponent");
float aln_crowdExponent = chf("../aln_crowdexponent");
int aln_amountLimit = chi("../aln_amountlimit");
int aln_relativeVelocity = chi("../aln_relativevelocity");
float aln_relativeStart = chf("../aln_relativestart");
float aln_relativeEnd = chf("../aln_relativeend");
float aln_priority = chf("../aln_priority");
int aln_writeAffectors = chi("../aln_writeaffectors");

// a multiplier on me
float aln_pMult = fit01(pRatio, aln_effectMin, aln_effectMax);
vector aln_accum = { 0,0,0 };

int aln_affectedBy[];
int aln_count = 0;