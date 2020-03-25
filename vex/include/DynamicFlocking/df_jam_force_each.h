


if ((forceControlBits & JAM_BIT) > 0 && jam_revalidate == 1) {
	
		int nofAffectors;
		float grabRadius = (jam_searchRadius + jam_searchRadius + (jam_searchRadius * jam_elasticity)) /3;
		
	    // within search radius - collect new affectors
	    if (oDistance <= grabRadius) {

	        nofAffectors = len(jam_affectedBy);
	        if (jam_maxAffectors > nofAffectors && !IsIntInArray(oPtnum, jam_affectedBy) ) {
	            //printf("jam collected:%d dist:%d\n", oPtNum, oDistance);
	            push(jam_affectedBy, oPtnum);
	            push(jam_selfPositions, P);
	            push(jam_affectorPositions, oP);
	            push(jam_initialLength, oDistance);
	        }
	    }
	    
	    // validate affectors - remove the borken ones
	    nofAffectors = len(jam_affectedBy);

	    for(int i=0; i < nofAffectors; i++) {
	        
	        int affectorId = jam_affectedBy[i];
	        if (affectorId != oPtnum) continue;
	        float initLen = jam_initialLength[i];
	        float jamMax = initLen * jam_elasticity;
	             
	        if (oDistance > jamMax){
	            //printf("jam removed:%d dist:%d\n", affectorId, oDistance);
	            continue;
	        }
	        
	        vector myInitPos = jam_selfPositions[i];
	        vector affectorInitPos = jam_affectorPositions[i];
	        	        
	        push(jam_validAffectorIds, affectorId);
	        push(jam_validSelfPos, myInitPos);
	        push(jam_validAffectorPos, affectorInitPos);
	        push(jam_validInitLens, initLen);
	    }

//jam enabled, revalidate
}


