

if ((forceControlBits & PUSH_BIT) > 0) {

	vector pushResult = push_accum * push_pMult * push_forceStrength;

	if (push_relativeVelocity) {
	    float angleFactor = FilterByAngle(forward, pushResult, push_relativeStart, push_relativeEnd);
	    pushResult *= angleFactor;
    }

	ForcePushComponent(forceNames, forceVectors, forcePriorities, PUSH_FC, pushResult, push_priority);
	  
	if (push_writeAffectors) {
		Spa("st_i_pushAffect", ptnum, push_count);
		SubmitAffectorPoints("push", ptnum, push_affectedBy);
	}

}



