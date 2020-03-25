
if ((forceControlBits & PNT_BIT) > 0) {

float pnt_closeRangeMin = 	chf("../pnt_closerangemin");
float pnt_closeRangeMax = 	chf("../pnt_closerangemax");
float pnt_strength = 		chf("../pnt_strength");
float pnt_effectMin = 		chf("../pnt_effectmin");
float pnt_effectMax = 		chf("../pnt_effectmax");
int pnt_relativeVelocity = 	chi("../pnt_relativevelocity");
float pnt_relativeStart = 	chf("../pnt_relativestart");
float pnt_relativeEnd = 	chf("../pnt_relativeend");
float pnt_priorityMax = 	chf("../pnt_prioritymax");
float pnt_farMin = 			chf("../pnt_farmin");
float pnt_farMax = 			chf("../pnt_farmax");
string pnt_geo = 			chs("../pnt_geo");

string pntFP = opfullpath(pnt_geo);
string pntGeoPath = concat("op:", pntFP);
int nofPoints = npoints(pntGeoPath);
//printf("%s %d\n", pntGeoPath, nofPoints);

if (nofPoints <= 0) {
    	printf("ERROR! No points were found on input #1. Skipping df_pnt\n");
    } else {

		float pMult = fit01(pRatio, pnt_effectMin, pnt_effectMax);

		vector closestPoint = minpos(pntGeoPath, P);

		vector pntVec = closestPoint - P;
		float pntLen = length(pntVec) - pscale;

		float priority = fit01( BiasedSmooth(pntLen, pnt_farMin, pnt_farMax, 1), 1, pnt_priorityMax);

		//decay within closerange
		float closeRangeDamp = clamp(fit(pntLen, pnt_closeRangeMin, pnt_closeRangeMax, 0,1) ,0,1);

		vector newPnt =  normalize(pntVec) * pMult * pnt_strength * closeRangeDamp;
	    
		if (pnt_relativeVelocity) {
		    float relativeFactor = FilterByAngle(forward, newPnt, pnt_relativeStart, pnt_relativeEnd );
		    newPnt *= relativeFactor;
		}
	   
		ForcePushComponent(forceNames, forceVectors, forcePriorities, PNT_FC, newPnt, priority);   

	// points found in geo
	}


//enable pnt
}