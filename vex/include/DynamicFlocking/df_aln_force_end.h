

if ((forceControlBits & ALN_BIT) > 0) {

	vector avgVelocity = {0,0,0};
	if (aln_count >= aln_amountLimit) {
	    avgVelocity = aln_accum / float(aln_count);
	} else {
	    aln_count = 0;
	}

	float avgVeloLen = length(avgVelocity);
	
	// create a multiplier based on the amount of affecting points
	int crowdBase = clamp(aln_count - aln_amountLimit, 0, aln_count) + 1;
	float crowdMult = pow(crowdBase, aln_crowdExponent);

	//vector oldVelocity -> avgVelocity

	vector diff = avgVelocity  - ( normalize(oldVelo) * avgVeloLen );

	vector newAlign = diff * aln_forceStrength * aln_pMult * crowdMult;

	if (aln_relativeVelocity) {
	    float relativeFactor = FilterByAngle(forward, newAlign, aln_relativeStart, aln_relativeEnd);
	    newAlign *= relativeFactor;
	    }
	    
	ForcePushComponent(forceNames, forceVectors, forcePriorities, ALN_FC, newAlign, aln_priority);
	  
	if (aln_writeAffectors) {
		// the amount of points affecting the result
		Spa("st_i_alnAffect", ptnum, aln_count);
		SubmitAffectorPoints("aln", ptnum, aln_affectedBy);
	}


//aln enabled
}
