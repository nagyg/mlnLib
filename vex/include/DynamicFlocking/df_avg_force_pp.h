

if ((forceControlBits & AVG_BIT) > 0) {

	float avg_closeRangeMin = chf("../avg_closerangemin");
	float avg_closeRangeMax = chf("../avg_closerangemax");
	float avg_strength = chf("../avg_strength");
	float avg_effectMin = chf("../avg_effectmin");
	float avg_effectMax = chf("../avg_effectmax");
	float avg_relativeStart = chf("../avg_relativestart");
	float avg_relativeEnd = chf("../avg_relativeend");
	float avg_priorityMax = chf("../avg_prioritymax");
	float avg_farMin = chf("../avg_farmin");
	float avg_farMax = chf("../avg_farmax");
	int avg_relativeVelocity = chi("../avg_relativevelocity");
	int avg_writeStates = chi("../avg_writestates");

	vector avgPoint = detailattrib(geoself(), "avgPos",0, success);

	if (!success) {
	printf("avgPos detailattrib was not found!\n");
	} else {

	if (avg_relativeEnd < avg_relativeStart || avg_closeRangeMax < avg_closeRangeMin || avg_farMax < avg_farMin) {
	printf("abnormal ranges in avg.\n");
	} else {


	vector avgVec = avgPoint - P;
	float avgLen = length(avgVec) - pscale;
	float priority = fit01(BiasedSmooth(avgLen, avg_farMin, avg_farMax, 1) , 1, avg_priorityMax);

	//decay within closerange
	float closeRangeDamp = clamp(fit(avgLen, avg_closeRangeMin, avg_closeRangeMax, 0,1) ,0,1);
	//printf("avg closeRangeDamp: %d\n",closeRangeDamp);    

	float mult = fit01(pRatio, avg_effectMin, avg_effectMax);
	vector avgResult =  normalize(avgVec) * mult * avg_strength * closeRangeDamp;
	          
	vector newAvg = avgResult;

	if (avg_relativeVelocity) {
	    float relativeFactor = FilterByAngle(forward, newAvg, avg_relativeStart, avg_relativeEnd );
	    
	    //printf("avg relativeFactor: %d\n",relativeFactor);
	    
	    newAvg *= relativeFactor;
	    }

	ForcePushComponent(forceNames, forceVectors, forcePriorities, AVG_FC, newAvg, priority);

	float farMult = 1;
	int isFar = 0;
	if (avgLen > avg_farMin) {
	    isFar = 1;
	    farMult = priority;
	}
	    
	float closeMult = 1;
	int avgClose = 0;
	if (avgLen < avg_closeRangeMax) {
	    avgClose = 1;
	    closeMult = clamp( avgLen / avg_closeRangeMax,0,1);
	}

	if (avg_writeStates) {
		Spa("st_i_avgIsClose", ptnum, avgClose);
		Spa("st_i_avgIsFar", ptnum, isFar);
		Spa("st_f_avgCloseMult", ptnum, closeMult);
		Spa("st_f_avgFarMult", ptnum, farMult);
	}


//meaningful ranges	
}
// was avg pos	
}
//avg enabled
}




