
float vis_forceRadius = chf("../vis_forceradius");
float vis_forceStrength = chf("../vis_forcestrength");
float vis_effectMin = chf("../vis_effectmin");
float vis_effectMax = chf("../vis_effectmax");
float vis_sizePowerMin = chf("../vis_sizepowermin");
float vis_sizePowerMax = chf("../vis_sizepowermax");
float vis_rangeExp = chf("../vis_exponent");
float vis_velocityFraction = chf("../vis_velocityfraction");
int vis_limitWithDir = chi("../vis_dirlimit");
float vis_dirStart = chf("../vis_dirstart");
float vis_dirEnd = chf("../vis_dirend");
int vis_relativeVelocity = chi("../vis_relativevelocity");
float vis_relativeStart = chf("../vis_relativestart");
float vis_relativeEnd = chf("../vis_relativeend");
float vis_accumMax = chf("../vis_accumax");
int vis_amountLimit = chi("../vis_amountlimit");
int vis_bufferSize = chi("../vis_buffersize");
float vis_priority = chf("../vis_priority");
float vis_fade = chf("../vis_fade");
int vis_writeAffectors = chi("../vis_writeaffectors");

vis_bufferSize *= subSteps;
float vis_fadeReal = (1-vis_fade) / float(subSteps);

// a multiplier on me
float vis_pMult = fit01(pRatio, vis_effectMin, vis_effectMax);
vector vis_accum = { 0,0,0 };

int vis_affectedBy[];
int vis_count = 0;