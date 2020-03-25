

if ((forceControlBits & VIS_BIT) > 0 && ptnum != oPtnum && realDist <= vis_forceRadius) {

	//the other point has a multiplier 
    float oEffectMult = fit01(oPRatio, vis_sizePowerMin, vis_sizePowerMax);	

    float angleFactor = 1.0;
    if (vis_limitWithDir) {
        vector toOther = oP - P;
        angleFactor = AngleRatio(forward, toOther, vis_dirStart, vis_dirEnd);       
        }
    
    //vision
    if (vis_limitWithDir && angleFactor <= 0) {} else {
	    //printf("af:%d\n",angleFactor);
	         
	    // 0 ... 1  0:far 1:close
	    float distanceFactor = 1 - clamp( realDist / vis_forceRadius, 0,1);      
	    distanceFactor = pow(distanceFactor,vis_rangeExp) ; 
	    
	    // collect
	    vis_accum += oV * distanceFactor * oEffectMult * vis_velocityFraction * angleFactor;
	    vis_count ++;
	    
	    if (vis_writeAffectors) {
	        push(vis_affectedBy, oPtnum);
	    }

	}


// vis enabled and not myself and within range
}