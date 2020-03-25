
if ((forceControlBits & POL_BIT) > 0) {
	
	float accumLimited = 0;
	if (pol_count >= pol_amountLimit) {
	    accumLimited = pol_accum;
	} else {
	    pol_count = 0;
	}


	vector newPolite = normalize(oldVelo) * -1 * accumLimited * pol_forceStrength * pol_pMult;

	ForcePushComponent(forceNames, forceVectors, forcePriorities, POL_FC, newPolite, pol_priority);

	if (pol_writeAffectors) {
		Spa("st_i_polAffect", ptnum, pol_count);
		SubmitAffectorPoints("pol", ptnum, pol_affectedBy);
	}


}