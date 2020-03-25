
if ((forceControlBits & GND_BIT) > 0) {
float largeNumber = 10000;

float gnd_effectMin = 		chf("../gnd_effectmin");
float gnd_effectMax = 		chf("../gnd_effectmax");
float gnd_forceStrength = 	chf("../gnd_forcestrength");
float gnd_jitterSize = 		chf("../gnd_jittersize");
int gnd_sampleAmount = 		chi("../gnd_sampleamount");
float gnd_pushClose = 		chf("../gnd_pushclose");
float gnd_pushFar = 		chf("../gnd_pushfar");
float gnd_pullClose = 		chf("../gnd_pullclose");
float gnd_pullFar = 		chf("../gnd_pullfar");
float gnd_pushStrength = 	chf("../gnd_pushstrength");
float gnd_pullStrength = 	chf("../gnd_pullstrength");
int gnd_relativeVelocity = 	chi("../gnd_relativevelocity");
float gnd_relativeStart = 	chf("../gnd_relativestart");
float gnd_relativeEnd = 	chf("../gnd_relativeend");
float gnd_priorityMax = 	chf("../gnd_prioritymax");
float gnd_geoCloseMin = 	chf("../gnd_geoclosemin");
float gnd_geoCloseMax = 	chf("../gnd_geoclosemax");
string gnd_geo = 			chs("../gnd_geo");
int gnd_writeAffectors =	chi("../gnd_writeaffectors");

string groundGeoPath = concat("op:", opfullpath(gnd_geo));


vector gnd_pos[], gnd_col[];
float gnd_alf[];

float pMult = fit01(pRatio, gnd_effectMin, gnd_effectMax);

vector down = {0,-1,0};
vector horizDir = normalize( set(forward.x, 0, forward.z) );
vector sampleVec = ((horizDir + down) / 2) * largeNumber;

int nofHits = 0;
vector sampledNormal = SampleGround( groundGeoPath, P, sampleVec, gnd_sampleAmount, gnd_jitterSize, nofHits , gnd_writeAffectors,  gnd_pos, gnd_col, gnd_alf);

float priority = 1;
vector newGnd = {0,0,0};
if (nofHits) {
    nofHits = 0;
    vector sampleToSurface = -1 * sampledNormal * largeNumber;
    float surfDistance = DistanceToSurface(groundGeoPath, P, sampleToSurface, nofHits, gnd_writeAffectors, gnd_pos, gnd_col, gnd_alf);
    
    if (nofHits) {
        newGnd = CalculateGroundForce(sampledNormal, surfDistance, gnd_pushStrength, gnd_pullStrength, gnd_pushClose, gnd_pushFar, gnd_pullClose, gnd_pullFar );  
        priority = fit01(  BiasedSmooth(surfDistance, gnd_geoCloseMin , gnd_geoCloseMax, 1) ,  gnd_priorityMax ,1 );
    }   
}

vector resultGnd  = newGnd * pMult * gnd_forceStrength;

if (gnd_relativeVelocity) {
    float relativeFactor = FilterByAngle(forward, resultGnd, gnd_relativeStart, gnd_relativeEnd);
    resultGnd *= relativeFactor;
    }

ForcePushComponent(forceNames, forceVectors, forcePriorities, GND_FC, resultGnd , priority);

if (gnd_writeAffectors) {
    SetVectorArrayAttrib("gnd_pos", ptnum, gnd_pos);
    SetVectorArrayAttrib("gnd_col", ptnum, gnd_col);
    SetFloatArrayAttrib("gnd_alf", ptnum, gnd_alf);

}

//ground enabled
}