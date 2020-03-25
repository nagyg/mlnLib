

if ((forceControlBits & SPR_BIT) > 0 && ptnum != oPtnum && realDist <= spr_mPullFar) {

   
    //the other point has a multiplier 
    float oEffectMult = fit01(oPRatio, spr_sizePowerMin, spr_sizePowerMax);
    
    float angleFactor =1.0;
    if (spr_limitWithDir) { 
        vector toOther = oP - P;                
        angleFactor =  AngleRatio(forward, toOther, spr_directionLimitLow, spr_directionLimitHigh  ) ;
    } 
    
    // vision - do i see this point?
    if (angleFactor > 0) {

	    //calculate one spring
	    float rawSpring = CalculateSpringForce(oDistance, spr_mPushClose, spr_mPushFar, spr_mPullClose, spr_mPullMid, spr_mPullFar, spr_mPush, spr_mPull, spr_writeAffectors, P, oP, spr_pos, spr_col, spr_alf);
	  
	    vector fromOther = P - oP;
	    vector oneSpring = normalize(fromOther) * rawSpring;
	    
	    // collect
	    spr_accum += oneSpring * angleFactor * oEffectMult;
	    spr_count ++; 
	}

// spr enabled, not myself, within radius
}
      
