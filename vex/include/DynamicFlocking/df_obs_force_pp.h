
if ((forceControlBits & OBS_BIT) > 0) {

float obs_effectMin = 		chf("../obs_effectmin");
float obs_effectMax = 		chf("../obs_effectmax");
float obs_forceRadius = 	chf("../obs_forceradius");
float obs_forceStrength = 	chf("../obs_forcestrength");
float obs_jitterSize = 		chf("../obs_jittersize");
int obs_sampleAmount = 		chi("../obs_sampleamount");
float obs_fade = 			chf("../obs_fade");
int obs_bufferSize = 		chi("../obs_buffersize");
float obs_speedExp = 		chf("../obs_speedexp");
int obs_relativeVelocity = 	chi("../obs_relativevelocity");
float obs_relativeStart = 	chf("../obs_relativestart");
float obs_relativeEnd = 	chf("../obs_relativeend");
float obs_priorityMax = 	chf("../obs_prioritymax");
float obs_geoCloseMin = 	chf("../obs_geoclosemin");
float obs_geoCloseMax = 	chf("../obs_geoclosemax");
int obs_writeAffectors =	chi("../obs_writeaffectors");
string obs_geo =			chs("../obs_geo");
int obs_useBuffering =		chi("../obs_usebuffering");
int obs_clampacceleration =	chi("../obs_clampacceleration");

if (obs_writeAffectors) {
	ClearVectorArrayAttrib("obs_pos", ptnum);
	ClearVectorArrayAttrib("obs_col", ptnum);
	ClearFloatArrayAttrib ("obs_alf", ptnum);
}

vector obs_pos[], obs_col[]; 
float obs_alf[];

string obsFP = opfullpath(obs_geo);
string obsGeoPath = concat("op:", obsFP);

float fadeReal = (1 - obs_fade) / float(subSteps);

obs_bufferSize *= subSteps;
float speedScale = pow(speed, obs_speedExp );
speedScale = clamp(speedScale, 1,10);
float pMult = fit01(pRatio, obs_effectMin, obs_effectMax);

float geoDistance = 0;
int nofHits = 0;
vector testVector = forward * obs_forceRadius * speedScale;
vector newObs = SampleObstacles(obsGeoPath, P, testVector, obs_sampleAmount, obs_jitterSize, nofHits, geoDistance, obs_writeAffectors, obs_pos, obs_col, obs_alf);
newObs = newObs * pMult * obs_forceStrength;


if (obs_clampacceleration) {
	vector localObs = World2AgentVector(ptnum, newObs);
	localObs[0] = 0;
	newObs = Agent2WorldVector(ptnum, localObs);
}



vector resultObs = {0,0,0};
if (obs_useBuffering) {
	if (nofHits > 0) {
	    resultObs = AddBufferedVectorValue("obs_buf", ptnum, newObs, obs_bufferSize);   
	} else {
	    resultObs = DegrageBufferedVectorValue("obs_buf", ptnum, fadeReal);  
	}
} else {
	resultObs = newObs;
}

t_v = resultObs;
//resultObs = resultObs - oldVelo;


if (obs_relativeVelocity) {
    float relativeFactor = FilterByAngle(forward, resultObs, obs_relativeStart, obs_relativeEnd);
    resultObs *= relativeFactor;
}
    
float priority = 1;
if (geoDistance > 0) {   
    priority = fit01(  BiasedSmooth(geoDistance, obs_geoCloseMin , obs_geoCloseMax, 1) ,  obs_priorityMax ,1 );  
}

ForcePushComponent(forceNames, forceVectors, forcePriorities, OBS_FC, resultObs , priority);

if (obs_writeAffectors) {
    SetVectorArrayAttrib("obs_pos",ptnum, obs_pos);
    SetVectorArrayAttrib("obs_col",ptnum, obs_col);
    SetFloatArrayAttrib("obs_alf",ptnum, obs_alf);
    Spa("st_i_obsNofHits", ptnum, nofHits);
}



//obs enabled
}

