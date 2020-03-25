

float push_effectMin = chf("../push_effectmin");
float push_effectMax = chf("../push_effectmax");
float push_forceRadius = chf("../push_forceradius");
float push_forceStrength = chf("../push_forcestrength");
float push_rangeExp = chf("../push_exponent");
int push_relativeVelocity = chi("../push_relativevelocity");
float push_relativeStart = chf("../push_relativestart");
float push_relativeEnd = chf("../push_relativeend");
float push_priority = chf("../push_priority");
int push_writeAffectors = chi("../push_writeaffectors");

int push_affectedBy[];
float push_pMult = fit01(pRatio, push_effectMin, push_effectMax);

vector push_accum = { 0,0,0 };
int push_count = 0;