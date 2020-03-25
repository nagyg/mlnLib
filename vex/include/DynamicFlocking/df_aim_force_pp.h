



if ((forceControlBits & AIM_BIT) > 0) {

	float aim_strength = chf("../aim_strength");
	float aim_effectMin = chf("../aim_effectmin");
	float aim_effectMax = chf("../aim_effectmax");
	float aim_closeRangeMin = chf("../aim_closerangemin");
	float aim_closeRangeMax = chf("../aim_closerangemax");
	float aim_relativeStart = chf("../aim_relativestart");
	float aim_relativeEnd = chf("../aim_relativeend");
	float aim_closeBias = chf("../aim_closebias");
	float aim_tolerance = chf("../aim_tolerance");
	float aim_priorityMax = chf("../aim_prioritymax");
	float aim_farMin = chf("../aim_farmin");
	float aim_farMax = chf("../aim_farmax");
	int aim_writeStates = chi("../aim_writestates");


	if(aim_closeRangeMax < aim_closeRangeMin || aim_farMax < aim_farMin || aim_relativeEnd < aim_relativeStart ){ 
    	printf("abnormal ranges in aim.\n");
    } else {

    vector aimPoint = point(aimPointsPath, "P", aimId);
    //printf("pt:%d aimId:%d aimPos:%p\n", ptnum, aimId, aimPoint);

	vector aimVec = aimPoint - P;
	float aimLen = length(aimVec);
	aimVec = normalize(aimVec);

	int inCloseMin =0;
	int inClosing =0;
	int inFarRange =0;
	int isArrived = 0;
	int inCloseMax = 0;
	if (aimLen <= aim_tolerance) isArrived =1;
	if (aimLen <= aim_closeRangeMin) inCloseMin =1;
	if (aimLen > aim_closeRangeMin && aimLen <= aim_closeRangeMax) inClosing =1;
	if (aimLen >= aim_farMin) inFarRange =1;
	if (aimLen <= aim_closeRangeMax) inCloseMax =1;

	float mult = fit01(pRatio, aim_effectMin, aim_effectMax);  
	float linearClosing = clamp( aimLen / (aim_closeRangeMax - aim_closeRangeMin) ,0,1);
	float smoothClosing = BiasedSmooth(aimLen, aim_closeRangeMin, aim_closeRangeMax, aim_closeBias);
	float priority = fit01(BiasedSmooth(aimLen, aim_farMin, aim_farMax, 1) , 1, aim_priorityMax);
	      
	float angleRatio = AngleRatio(forward, aimVec, aim_relativeStart, aim_relativeEnd);
	if (aim_writeStates) {
		Spa("aimAngleRatio", ptnum, angleRatio);	
	}

	vector aimDir = aimVec * aim_strength * mult;     
	vector aimTurn = aimVec - forward;
	vector aimMixed = lerp(aimTurn, aimDir, angleRatio);

	vector overBreak = {0,0,0};
	vector aimResult = {0,0,0};

	if (isArrived || inCloseMin) {
	    vector brake = -1 * oldVelo;
	    aimResult = brake;
	}

	float farMult = 1;
	int isFar = 0;
	if (aimLen > aim_farMin) {
	    isFar = 1;
	    farMult =  priority;
	}


	if (inClosing) {
	        
	        float overLen = oldVeloLen - aimLen;
	        if (overLen <0) overLen =0;
	        overBreak = -1 * normalize(oldVelo) * overLen;
	        
	        //braking while approaching the aim point      
	        vector step = oldVelo * TimeInc;
	        float stepLen = length(step) ;
	        float nofSteps = aimLen / stepLen;
	        vector brake = -1 * oldVelo / nofSteps ;
	        brake += overBreak;          

	        vector aimComp = aimMixed * smoothClosing;
	        vector brakeComp = brake * (1- linearClosing);
	            
	        vector aimClosing = aimComp + brakeComp;
	        
	        //dont overpull...
	        float maxLen = clamp( length(aimClosing), 0, aimLen);
	      
	        aimResult = normalize(aimClosing) * maxLen;   
	  

	} else { 
	       
	    aimResult = aimMixed;
	}

	ForcePushComponent(forceNames, forceVectors, forcePriorities, AIM_FC, aimResult, priority);

	if (aim_writeStates) {
		Spa("st_f_aimCloseMult", ptnum, linearClosing);

		Spa("st_i_aimInCloseMin", ptnum, inCloseMin);
		Spa("st_i_aimInCloseMax", ptnum, inCloseMax);
		Spa("st_i_aimInClosing", ptnum, inClosing);
		Spa("st_i_aimInFarRange", ptnum, inFarRange);
		Spa("st_i_aimIsArrived", ptnum, isArrived);

		Spa("st_i_aimIsFar", ptnum, isFar);
		Spa("st_f_aimFarMult", ptnum, farMult);
	}

	//meaningful ranges
    }
// aim enabled
}