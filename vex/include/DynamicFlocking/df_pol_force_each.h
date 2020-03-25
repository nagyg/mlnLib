
if ((forceControlBits & POL_BIT) > 0 && ptnum != oPtnum && realDist <= pol_forceRadius) {
      
    float angleFactor = 1;
    if (pol_limitWithDir) {          
        vector toNear = oP - P;    
        angleFactor = AngleRatio(forward, toNear, pol_directionLimitLow, pol_directionLimitHigh );
    }
    //i dont see this point
    if (angleFactor > 0) {

		float sizeMult = fit01(oPRatio, pol_sizePowerMin, pol_sizePowerMax);
         
    	float distanceRatio = 1 - clamp( realDist / pol_forceRadius, 0,1);      
    	distanceRatio = pow(distanceRatio, pol_rangeExp); 
    
    	pol_accum +=  angleFactor * distanceRatio * sizeMult;
    
    	pol_count ++; 
    	if (pol_writeAffectors) {
        	push(pol_affectedBy, oPtnum);
    	}

    //visible point
    }


//pol enabled, not myself, within radius
}

