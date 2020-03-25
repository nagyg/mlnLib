
if ((forceControlBits & ALN_BIT) > 0 && ptnum != oPtnum && realDist <= aln_forceRadius) {

   
    //the other point has a multiplier 
    float oEffectMult = fit01(oPRatio, aln_sizePowerMin, aln_sizePowerMax);

    vector toOther = oP - P;
    float angleFactor =1.0;
    if (aln_limitWithDir) {                 
        angleFactor =  AngleRatio(forward, toOther, aln_directionLimitLow, aln_directionLimitHigh  ) ;
    } 

    // vision - do i see this point?
    if (angleFactor > 0) {
	         
	    // 0 ... 1  0:far 1:close
	    float distanceFactor = 1 - clamp( realDist / aln_forceRadius, 0,1);      
	    distanceFactor = pow(distanceFactor, aln_rangeExp); 
	    
	    // collect
	    aln_accum += oV * angleFactor * distanceFactor * oEffectMult;
	    aln_count ++; 
	    
	    if (aln_writeAffectors) {
	        push(aln_affectedBy, oPtnum);
	    }  

	// point was visible	    
	}


//aln enabled, not myself, within radius
}