
float coh_effectMin = chf("../coh_effectmin");
float coh_effectMax = chf("../coh_effectmax");
float coh_forceRadius = chf("../coh_forceradius");
float coh_forceStrength = chf("../coh_forcestrength");
int coh_relativeVelocity = chi("../coh_relativevelocity");
float coh_relativeStart = chf("../coh_relativestart");
float coh_relativeEnd = chf("../coh_relativeend");
int coh_bufferSize = chi("../coh_buffersize");
float coh_fade = chf("../coh_fade");
int coh_amountLimit = chi("../coh_amountlimit");
float coh_priority = chf("../coh_priority"); 
int coh_writeAffectors = chi("../coh_writeaffectors");

coh_bufferSize *= subSteps;
float coh_fadeReal = (1-coh_fade) / float(subSteps);
float coh_pMult = fit01(pRatio, coh_effectMin, coh_effectMax);

int coh_affectedBy[];
vector coh_accum = { 0,0,0 };
int coh_count =0;

if ((forceControlBits & COH_BIT) > 0) {

	if (coh_relativeEnd < coh_relativeStart ) {
		printf("abnormal ranges in coh.\n");
	} 

// coh enabled
}