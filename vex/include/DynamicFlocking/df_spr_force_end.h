

if ((forceControlBits & SPR_BIT) > 0) {
	
	// the amount of points affecting the result
	
	vector avgSpring = {0,0,0};

	// the case when no affecting points were found
	if (spr_count > 0) {
	    avgSpring = spr_accum / float(spr_count);
	    }

	// create a multiplier based on the amount of affecting points
	float crowdMult = pow(spr_count, spr_crowdExponent);

	vector sprResult = avgSpring * spr_forceStrength * crowdMult;
	float sprLen = length(sprResult);

	// what is this? %%%
	vector newSpring = sprResult - (normalize(forward) * sprLen);

	//relative to our forward
	if (spr_relativeVelocity) {
	    float relativeRate = FilterByAngle(forward, newSpring, spr_relativeStart, spr_relativeEnd);
	    newSpring *= relativeRate;
	}

	ForcePushComponent(forceNames, forceVectors, forcePriorities, SPR_FC, newSpring, spr_priority);

	if (spr_writeAffectors) {
		Spa("st_i_sprAffect", ptnum, spr_count);
		SetVectorArrayAttrib("spr_pos", ptnum, spr_pos);
	    SetVectorArrayAttrib("spr_col", ptnum, spr_col);
	    SetFloatArrayAttrib("spr_alf", ptnum, spr_alf);
	}

}
