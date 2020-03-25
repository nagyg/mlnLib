

if ((forceControlBits & JAM_BIT) > 0) {
	

	if (jam_revalidate) {
		jam_affectedBy = jam_validAffectorIds;
		jam_selfPositions = jam_validSelfPos;
		jam_affectorPositions = jam_validAffectorPos;
		jam_initialLength = jam_validInitLens;

		SetIntArrayAttrib(JAMAFFECTORS, ptnum, jam_affectedBy);
		SetVectorArrayAttrib(JAMSELFPOSITION, ptnum, jam_selfPositions);
		SetVectorArrayAttrib(JAMAFFECTORPOSITION, ptnum, jam_affectorPositions);
		SetFloatArrayAttrib(JAMINITALLENGTH, ptnum, jam_initialLength);
	}

	//calculate forces
	int nofAffectors = len(jam_affectedBy);

	vector accum = {0,0,0};

	for(int i=0; i < nofAffectors; i++) {
	    
	    int affectorId = jam_affectedBy[i];   
	    vector myInitPos = jam_selfPositions[i];
	    vector affectorInitPos = jam_affectorPositions[i];
	    float initLen = jam_initialLength[i];
	    
	    float jamMax = initLen * jam_elasticity;

	    vector myOffset = myInitPos - P;
	    
	    vector affectorPos = point(geoself(), "P", affectorId);
	    
	    vector affectorOffset = affectorPos + myOffset;
	    vector affectorDiff = affectorOffset - affectorInitPos ;
	        
	    vector newAffector = affectorOffset - myInitPos;
	    vector oldAffecttor = normalize(newAffector) * initLen;
	    float currentLen = length(affectorOffset - myInitPos);

	    //ExponentialWeightByMidPoint(pos.x, start, end, midPoint, exponent);
	    float weight = ExponentialWeightByMidPoint(currentLen, 0, jamMax, initLen, jam_stretchExponent);
	 
	    //printf("pt:%d\tcurr:%d\tinit:%d\tjmax:%d\tstr:%d\n", myPtNum, RoundTo(currentLen,3),RoundTo(initLen,3), RoundTo(jamMax,3),  RoundTo(stretch,3));
	    
	    vector affectorForce = newAffector - oldAffecttor;
	      
	    vector rawForce = affectorForce  * weight;
	    accum += rawForce;
	}

	accum *= jam_forceStrength;

	ForcePushComponent(forceNames, forceVectors, forcePriorities, JAM_FC, accum, jam_priority);

//jam enabled
}