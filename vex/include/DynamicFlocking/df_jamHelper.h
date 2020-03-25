#define JAMAFFECTORS "jam_affectorIds"
#define JAMSELFPOSITION  "jam_selfPosition"
#define JAMAFFECTORPOSITION  "jam_affectorPosition"
#define JAMINITALLENGTH "jam_initialLength"

vector CalculateDiffForce( 
float elasticity, forceStrength, stretchExponent; 
int maxAffectors, myPtNum;
vector myPos, selfPositions[], affectorPositions[];
int affectedBy[];
float initialLength[];
vector debug1, debug2
) {

//calculate forces
int nofAffectors = len(affectedBy);

int affectorId;
float diff, currentLen, stretch, initLen, jamMax;
vector myOffset, affectorOffset, affectorPos, affectorForce, myInitPos, affectorInitPos ;
vector accum = {0,0,0};

for(int i=0; i < nofAffectors; i++) {
    
    affectorId = affectedBy[i];   
    myInitPos = selfPositions[i];
    affectorInitPos = affectorPositions[i];
    initLen = initialLength[i];
    
    jamMax = initLen * elasticity;
    vector affectorCurrentPos = point(geoself(), "P", affectorId);
    
    affectorPos = affectorCurrentPos - affectorInitPos;    
    
    vector rawForce = affectorPos * -0.1 ;
    
    accum += rawForce;
}


if (nofAffectors == 0) nofAffectors=1;
accum = accum / float(nofAffectors);


accum *= forceStrength;
vector jamForce = accum;

/*
float jx = RoundTo(jamForce.x,5);
float jy = RoundTo(jamForce.y,5);
float jz = RoundTo(jamForce.z,5);

printf("%d\t%d\t%d\n",jx,jy,jz);
*/

return jamForce;

}






vector CalculateForce( 
float elasticity, forceStrength, stretchExponent; 
int maxAffectors, myPtNum;
vector myPos, selfPositions[], affectorPositions[];
int affectedBy[];
float initialLength[];
vector debug1, debug2
) {

//calculate forces
int nofAffectors = len(affectedBy);

int affectorId;
float diff, currentLen, stretch, initLen, jamMax;
vector myOffset, affectorOffset, affectorPos, affectorForce, myInitPos, affectorInitPos ;
vector accum = {0,0,0};

for(int i=0; i < nofAffectors; i++) {
    
    affectorId = affectedBy[i];   
    myInitPos = selfPositions[i];
    affectorInitPos = affectorPositions[i];
    initLen = initialLength[i];
    
    jamMax = initLen * elasticity;

    myOffset = myInitPos - myPos;
    
    affectorPos = point(geoself(), "P", affectorId);
    
    affectorOffset = affectorPos + myOffset;
    vector affectorDiff = affectorOffset - affectorInitPos ;
    debug1 = affectorDiff;
    
    vector newAffector = affectorOffset - myInitPos;
    vector oldAffecttor = normalize(newAffector) * initLen;
    currentLen = length(affectorOffset - myInitPos);

    //ExponentialWeightByMidPoint(pos.x, start, end, midPoint, exponent);
    float weight = ExponentialWeightByMidPoint(currentLen, 0, jamMax, initLen, stretchExponent);
 
    //printf("pt:%d\tcurr:%d\tinit:%d\tjmax:%d\tstr:%d\n", myPtNum, RoundTo(currentLen,3),RoundTo(initLen,3), RoundTo(jamMax,3),  RoundTo(stretch,3));
    
    affectorForce = newAffector - oldAffecttor;
    
    
    
    vector rawForce = affectorForce  * weight;
    accum += rawForce;
}

/*
if (nofAffectors == 0) nofAffectors=1;
accum = accum / float(nofAffectors);
*/

accum *= forceStrength;
vector jamForce = accum;

/*
float jx = RoundTo(jamForce.x,5);
float jy = RoundTo(jamForce.y,5);
float jz = RoundTo(jamForce.z,5);

printf("%d\t%d\t%d\n",jx,jy,jz);
*/

return jamForce;

}



void InitAffectors(float searchRadius, elasticity, forceStrength, stretchExponent; int maxAffectors, myPtNum; vector myPos, selfPositions[], affectorPositions[]; int affectedBy[]; float initialLength[]) {
    
int oPtNum, nofAffectors, affectorId;
vector oP, myInitPos, affectorInitPos;
float oDistance, oPscale, oPRatio, initLen, jamMax;

int maxPoints = npoints(geoself());

float pSearch = searchRadius * elasticity * 2;
int handle = pcopen(geoself(),"P", myPos, pSearch, maxPoints+1);

while (pciterate(handle))
{  
    pcimport(handle, "point.number", oPtNum);
    if (oPtNum == myPtNum) continue;

    pcimport(handle, "P", oP);
    pcimport(handle, "point.distance", oDistance);
    
    // withing search radius - collect new affectors
    if (oDistance <= searchRadius) {

        nofAffectors = len(affectedBy);
        if (maxAffectors > nofAffectors && !IsIntInArray(oPtNum, affectedBy) ) {
            //printf("jam collected:%d dist:%d\n", oPtNum, oDistance);
            push(affectedBy, oPtNum);
            push(selfPositions, myPos);
            push(affectorPositions, oP);
            push(initialLength, oDistance);
        }
    }
    
   

}
pcclose(handle);

}


void ReValidateAffectors(float searchRadius, elasticity, forceStrength, stretchExponent; int maxAffectors, myPtNum; vector myPos, selfPositions[], affectorPositions[]; int affectedBy[]; float initialLength[]) {
    
    
int oPtNum, nofAffectors, affectorId;
vector oP, myInitPos, affectorInitPos;
float oDistance, oPscale, oPRatio, initLen, jamMax;

int validAffectorIds[];
vector validSelfPos[], validAffectorPos[];
float validInitLens[];

int maxPoints = npoints(geoself());

float pSearch = searchRadius * elasticity * 2;
int handle = pcopen(geoself(),"P", myPos, pSearch, maxPoints+1);

while (pciterate(handle))
{  
    pcimport(handle, "point.number", oPtNum);
    if (oPtNum == myPtNum) continue;

    pcimport(handle, "P", oP);
    pcimport(handle, "point.distance", oDistance);
    
    // withing search radius - collect new affectors
    if (oDistance <= searchRadius) {

        nofAffectors = len(affectedBy);
        if (maxAffectors > nofAffectors && !IsIntInArray(oPtNum, affectedBy) ) {
            //printf("jam collected:%d dist:%d\n", oPtNum, oDistance);
            push(affectedBy, oPtNum);
            push(selfPositions, myPos);
            push(affectorPositions, oP);
            push(initialLength, oDistance);
        }
    }
    
    // validate affectors - remove the borken ones
    nofAffectors = len(affectedBy);

    for(int i=0; i < nofAffectors; i++) {
        
        affectorId = affectedBy[i];
        if (affectorId != oPtNum) continue;
        initLen = initialLength[i];
        jamMax = initLen * elasticity;
             
        if (oDistance > jamMax){
            //printf("jam removed:%d dist:%d\n", affectorId, oDistance);
            continue;
        }
        
        myInitPos = selfPositions[i];
        affectorInitPos = affectorPositions[i];
        
        
        push(validAffectorIds, affectorId);
        push(validSelfPos, myInitPos);
        push(validAffectorPos, affectorInitPos);
        push(validInitLens, initLen);
    }

}
pcclose(handle);

affectedBy = validAffectorIds;
selfPositions = validSelfPos;
affectorPositions = validAffectorPos;
initialLength = validInitLens;
   
    
}


vector jamSolver_combo(float searchRadius, elasticity, forceStrength, stretchExponent; int maxAffectors, myPtNum; vector myPos) {
    
    
int oPtNum, nofAffectors, affectorId;
vector oP, myInitPos, affectorInitPos;
float oDistance, oPscale, oPRatio, initLen, jamMax;

string affectorsName;
affectorsName = AffectorName("jam");

int affectedBy[];
affectedBy = GetIntArrayAttrib(affectorsName, myPtNum);

vector selfPositions[], affectorPositions[];
selfPositions = GetVectorArrayAttrib(JAMSELFPOSITION, myPtNum);
affectorPositions = GetVectorArrayAttrib(JAMAFFECTORPOSITION, myPtNum);

float initialLength[];
initialLength = GetFloatArrayAttrib(JAMINITALLENGTH, myPtNum);

ReValidateAffectors(searchRadius, elasticity, forceStrength, stretchExponent, maxAffectors, myPtNum, myPos, selfPositions, affectorPositions, affectedBy,  initialLength) ;

vector debug1, debug2;
debug1 = {0,0,0};
debug2 = {0,0,0};

vector jamForce =
CalculateForce( elasticity, forceStrength, stretchExponent, maxAffectors, myPtNum, myPos, selfPositions, affectorPositions, affectedBy,  initialLength, debug1, debug2) ;

SetIntArrayAttrib(affectorsName, myPtNum, affectedBy);
SetVectorArrayAttrib(JAMSELFPOSITION, myPtNum, selfPositions);
SetVectorArrayAttrib(JAMAFFECTORPOSITION, myPtNum, affectorPositions);
SetFloatArrayAttrib(JAMINITALLENGTH, myPtNum, initialLength);

return jamForce;
  
    
}




