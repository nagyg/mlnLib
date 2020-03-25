
float jam_searchRadius = 	chf("../jam_searchradius");
float jam_elasticity = 		chf("../jam_elasticity");
int jam_maxAffectors = 		chi("../jam_affectorsmax");
float jam_forceStrength = 	chf("../jam_forcestrength");
float jam_stretchExponent = chf("../jam_stretchexponent");
float jam_priority =		chf("../jam_priority");
int jam_revalidate = 		chi("../jam_revalidate");



	int jam_affectedBy[];
	vector jam_selfPositions[], jam_affectorPositions[];
	float jam_initialLength[];

	int jam_validAffectorIds[];
	vector jam_validSelfPos[], jam_validAffectorPos[];
	float jam_validInitLens[];
	

if ((forceControlBits & JAM_BIT) > 0) {

	jam_affectedBy = GetIntArrayAttrib(JAMAFFECTORS, ptnum);
	jam_selfPositions = GetVectorArrayAttrib(JAMSELFPOSITION, ptnum);
	jam_affectorPositions = GetVectorArrayAttrib(JAMAFFECTORPOSITION, ptnum);
	jam_initialLength = GetFloatArrayAttrib(JAMINITALLENGTH, ptnum);

}	

