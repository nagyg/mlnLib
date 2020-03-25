


if ((forceControlBits & PUSH_BIT) > 0 && ptnum != oPtnum && realDist <= push_forceRadius) {


		if (push_writeAffectors) {
        	push(push_affectedBy, oPtnum);
    	}

    	vector fromNear = P - oP;
       
    	float distanceRatio = 1 - clamp( realDist / push_forceRadius,0,1);
    	distanceRatio = pow(distanceRatio, push_rangeExp);

    
    	push_accum += normalize(fromNear) * distanceRatio;
    	push_count ++;

// not myself
// within radius
//push enabled
}	