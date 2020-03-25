#include "Helpers/mathHelper.h"
#include "Helpers/pointAttribHelper.h"
#include "Helpers/arrayAttribHelper.h"

#define PROGRESSBAR_BLOCKSIZE 10
#define PROGRESSBAR_NOFBLOCKS 6

#define STATE_LINE 1
#define STATE_LEAF 2
#define STATE_GOAL 3
#define STATE_NO_JUNCTION 4
#define STATE_ROOT 5
#define STATE_JUNCTION 6
#define STATE_DEAD_END 7
#define STATE_BEVEL_FIRST 8
#define STATE_BEVEL_SECOND 9
#define STATE_BEVEL_TRI 10
#define STATE_START 11
#define STATE_GIVEUP 12


#define A_TARGETIDS "targetids"
#define A_GOAL "goal"
#define A_GOAL_COUNT "goalcount"
#define A_START "start"
#define A_STATE "state"
#define A_LINEID "lineid"
#define A_ORDERI "orderi"
#define A_ORDERF "orderf"
#define A_ORDERN "ordern"
#define A_VERBOSITY "verbosity"
#define A_TONEXT "tonext"
#define A_SELECTOR_MAX_PTS "smaxp"
#define A_RANKI "ranki"
#define A_RANKN "rankn"
#define A_MAXORDER "maxorder"
#define A_UNUSED "unused"
#define A_ORDER_FIRST "orderfirst"
#define A_ORDER_LAST "orderlast"
#define A_STATENAME "statename"
#define A_UP "up"
#define A_UV "uv"
#define A_DISTTONEXT "disttonext"
#define A_BEVEL "bevel"
#define GRP_START "start"
#define GRP_GOAL "goal"
#define A_ISGOALLINE "goalline"
#define A_WAY_GUID "wayguid"
#define A_SELECTOR_GUID "selectorGuid"
#define A_GUID_SHORTPATH "shortPathGuid"
#define A_PREVPOS "prevpos"
#define A_NEXTPOS "nextpos"
#define A_DISTALONGCURVE "dalct"
#define A_DALC "dalc"

struct WayFinder {

float smallNumber = 0.0001;

//parameters
int verbosity = 1; 
int inputSeed = 1337;
int seed = 1337;
int checkIntersecting = 1; // intersection check on/off
int checkHeading = 1; // heading check enable
float headingThreshold = 0.9; // extra check pre drawing
int maxJunctionSelectAttempts = 320; 
int junctionSelectMode = 1;
float uvMargin = 0.02;
int doBevel = 0;
int triPointBevel = 1;
float bevelSize = 10;
float bevelSizeAbs = 0.1;
float bevelCornerOffset = 0.0;
float bevelDotThreshold = 1.0;

//runtime stuff
string wayGuid = "";
int goalCount = 0;
int rndCount =0;
int currentStartPointId = -1;
int segmentsTotal = 0;
int nofNew = 0;
int lastCreatedPointId =0;
int nofPrimitivesCreated = 0;
int goalReached = 0;
int wayOrder = 0;
int lineId = 0; // counting up per primitive
int maxOrder =0; // ever created maximum of order
int selectorMaxPoints; // nof target points used by the selector
int nofSourcePoints = 0; // source points from input 1
int progressBar = 0;
vector startVectors[] ={}; // the line segments start ...
vector endVectors[] = {}; //  ... end points
 
 
int lineJcs[] = {}; // the source pointIds used as junction

int sourceOrders[] = {}; // order lookup for source points
int sourceStartIds[] = {};
int sourceGoalIds[] = {};


// created geometry
int createdOrders[] ={}; 
int createdIds[] ={}; // pointIds of the lines
int createdSourceIds[] = {}; // created pointId => used source pointId
int createdRanks[] = {};
int createdLineIds[] = {};
int createdStates[] = {};
vector createdToNext[] = {};
vector createdPositions[] = {};

int lookUpStartOrGoal[] ={};
int lookUpIsUsed[] ={};
vector lookUpPosition[] ={};

//derived geo
float normalizedRanks[] = {}; 
vector createdUvs[] = {};

//line stuff
int createdPimitives[] = {};
int maxRankPerLine[] = {};
int createdOrderMin[] = {};	
int createdOrderMax[] = {};

//stats per line
int last_nofIntersects;
int last_nofHeadings;
int last_nofBadJunction;



//     #########              
//    ####                    
//     ########               
//           ####             
//    ####   ####             
//    #########


void SaveToDetail(){
    printf("saving to detail\n");
    Dsi("verbosity",verbosity);
    
    Dsi("seed",seed);
    Dsi("inputSeed", inputSeed);
    Dsi("checkIntersecting",checkIntersecting);
    Dsi("checkHeading",checkHeading);
    Dsf("headingThreshold",headingThreshold);
    Dsi("maxJunctionSelectAttempts",maxJunctionSelectAttempts);
    Dsi("junctionSelectMode",junctionSelectMode);
    Dsf("uvMargin",uvMargin);
   

    Dsia("sourceStartIds", sourceStartIds);
    Dsia("sourceGoalIds", sourceGoalIds);
   

    //runtime stuff
    Dss(A_WAY_GUID, wayGuid);
    Dsi(A_GOAL_COUNT, goalCount);
    Dsi("rndCount",rndCount);
    Dsi("segmentsTotal",segmentsTotal);
    Dsi("nofNew",nofNew);
    Dsi("nofPrimitivesCreated",nofPrimitivesCreated);
    Dsi("lineId",lineId);
    Dsi(A_MAXORDER,maxOrder);
    Dsi("selectorMaxPoints",selectorMaxPoints);
    Dsi("nofSourcePoints",nofSourcePoints);
    

    // created geometry
    Dsia("createdOrders",createdOrders);
    Dsia("createdIds",createdIds);
    Dsia("createdSourceIds",createdSourceIds);
    Dsia("createdRanks",createdRanks);
    Dsia("createdLineIds",createdLineIds);
    Dsia("createdStates",createdStates);
    Dsva("createdPositions",createdPositions);
    Dsia("sourceOrders", sourceOrders);
    Dsva("createdToNext", createdToNext);

    //derived geo
    Dsfa("normalizedRanks",normalizedRanks);
    Dsva("createdUvs",createdUvs);

    //line stuff
    Dsia("createdPimitives",createdPimitives);
    Dsia("maxRankPerLine",maxRankPerLine);
    Dsia("createdOrderMin",createdOrderMin);
    Dsia("createdOrderMax",createdOrderMax);

    //bevel stuff
    Dsi("doBevel",doBevel);
    Dsi("triPointBevel",triPointBevel);
    Dsf("bevelSize",bevelSize);
    Dsf("bevelSizeAbs",bevelSizeAbs);
    Dsf("bevelCornerOffset",bevelCornerOffset);
    Dsf("bevelDotThreshold", bevelDotThreshold);
    printf("save done\n");
}



void LoadFromDetail() {
    printf("Loading from detail\n");
    verbosity =Dgi("verbosity");
     
    seed =Dgi("seed");
    inputSeed = Dgi("inputSeed");
    checkIntersecting =Dgi("checkIntersecting");
    checkHeading =Dgi("checkHeading");
    headingThreshold =Dgf("headingThreshold");
    maxJunctionSelectAttempts =Dgi("maxJunctionSelectAttempts");
    junctionSelectMode =Dgi("junctionSelectMode");
    uvMargin =Dgf("uvMargin");
    

    sourceStartIds = Dgia("sourceStartIds");
    sourceGoalIds = Dgia("sourceGoalIds");

    //runtime stuff
    wayGuid = Dgs(A_WAY_GUID);
    goalCount =Dgi(A_GOAL_COUNT);
    rndCount =Dgi("rndCount");
    segmentsTotal =Dgi("segmentsTotal");
    nofNew =Dgi("nofNew");
    nofPrimitivesCreated =Dgi("nofPrimitivesCreated");
    lineId =Dgi("lineId");
    maxOrder =Dgi(A_MAXORDER); // this one is used later
    selectorMaxPoints = Dgi("selectorMaxPoints");
    nofSourcePoints =Dgi("nofSourcePoints");
   

    // created geometry
    createdOrders = Dgia("createdOrders");
    createdIds = Dgia("createdIds");
    createdSourceIds = Dgia("createdSourceIds");
    createdRanks = Dgia("createdRanks");
    createdLineIds = Dgia("createdLineIds");
    createdStates = Dgia("createdStates");
    createdPositions  =Dgva("createdPositions");
    sourceOrders = Dgia("sourceOrders");
    createdToNext = Dgva("createdToNext");

    //derived geo
    normalizedRanks = Dgfa("normalizedRanks");
    createdUvs =Dgva("createdUvs");

    //line stuff
    createdPimitives = Dgia("createdPimitives");
    maxRankPerLine = Dgia("maxRankPerLine");
    createdOrderMin = Dgia("createdOrderMin");
    createdOrderMax = Dgia("createdOrderMax");

        //bevel stuff
    doBevel = Dgi("doBevel");
    triPointBevel = Dgi("triPointBevel");
    bevelSize = Dgf("bevelSize");
    bevelSizeAbs = Dgf("bevelSizeAbs");
    bevelCornerOffset =Dgf("bevelCornerOffset");
    bevelDotThreshold =Dgf("bevelDotThreshold");
    printf("Load done.\n");
}


string GetNodeName() {
    string fullPath = opfullpath("..");
    string dirName, nodeName;
    splitpath(fullPath, dirName, nodeName);
    return nodeName;
}

int SeedByNodeName() {

    int sum = 0;
    string myName = this->GetNodeName();
    for(int i=0;i<len(myName);i++) {
        sum += ord(myName[i]);
    }
    return sum;
}

void MakeGuid() {
    string selGuid = detail(1,A_SELECTOR_GUID);
    int ht = int(rint(headingThreshold * 10000.0));
    int bsa = int(rint(bevelSizeAbs*10000.0));
    int bco = int(rint(bevelCornerOffset*10000.0));
    int bdt = int(rint(bevelDotThreshold*10000.0));

    string shPart = Dgs(A_GUID_SHORTPATH);

    string nn = this->GetNodeName();
    string bevelPart = "";
    
    if (doBevel) {
       bevelPart = sprintf("_%dz%do%dt%d", triPointBevel, bsa,bco, bdt);
    }
    wayGuid = sprintf("%s_e%s_s%dh%di%dt%da%dj%d%s%s",nn, selGuid, inputSeed,checkHeading,checkIntersecting, ht,maxJunctionSelectAttempts, junctionSelectMode, bevelPart, shPart);
    Dss(A_WAY_GUID, wayGuid);
}

void SetVerbosity(int isVerbose) {
    //printf("Verbose:%s\n", isVerbose ? "Yes" : "No" );
    verbosity = isVerbose;
}

// gets generic stuff from the detail - SELF
void SlimInitFromDetailSelf() {
    int vl = Dgi(A_VERBOSITY);
    this->SetVerbosity(vl);
    seed = Dgi("seed");
    inputSeed  = Dgi("inputSeed");
    //printf("Params from SELF:\tVerbosity: %s\tSeed: %d\n" , ( vl ? "Yes" : "No" ), seed );
}

// just a few session related things, not necessary to load and trash details
void SlimInitFromDetailInputOne() {
    int vl = detail(1,A_VERBOSITY,0);
    this->SetVerbosity(vl);
    seed = detail(1,"seed",0);
    inputSeed = detail(1,"inputSeed");
    //printf("Params from #1:\tVerbosity: %s\tSeed: %d\n" , ( vl ? "Yes" : "No" ), seed );
}
    

void ResetRandom(int seedIn)
{
    this.seed =seedIn;
    this.rndCount =0;
}


float NextRandom()
{
    rndCount++;
    return random(seed + rndCount);
}

int NofUsedPoints() {
    return len(createdSourceIds);
}

int RndLineId()
{
    int sl = this->NofUsedPoints();
    float rnd = this->NextRandom();
    int rndIndex = int(floor(sl) * rnd);
    return createdSourceIds[rndIndex];
}

int IsUsed(int idx ) {
    return lookUpIsUsed[idx] == 1;
    //return IsIntInArray(idx, createdSourceIds);
}

int RandomElement(int arr[])
{
	int sl = len(arr);
    float rnd = this->NextRandom();
    int rndIndex = int(floor(sl) * rnd);
    return arr[rndIndex];

}

int[] ShuffleIntArray(int source[])
{
    int sl = len(source);
    if (sl == 0) return source;
    int target[];
    int used[];
    resize(target,sl);
    resize(used,sl);
    int i =0;

    while(i < sl)
    {
        float rnd = this->NextRandom();
        int rndIndex = int(floor(sl) * rnd);
        if (used[rndIndex] == 0)
        {
            target[rndIndex] = source[i]; // copy
            used[rndIndex]=1; //mark it used
            i++;
        }
    }

    return target;
}


void ComplexityLevel(){

    printf("Calculating complexity\n");
    int ids[];
    int sum = 0;
   
    int nofPts = npoints(geoself());
    for(int i=0; i < nofPts; i++) {
         ids =GetIntArrayAttrib(A_TARGETIDS, i);
         sum += len(ids);
    }

    printf("Complexity:%d\n", sum);
}




void SetGoalAttribByGroup(int inputNum) {
    int one = 1;
    int sp[] = expandpointgroup(inputNum, GRP_GOAL);
    sourceGoalIds = this->ShuffleIntArray(sp);
    int nofGoals = len(sourceGoalIds);
    for(int i=0;i<nofGoals;i++){
        int goalId = sourceGoalIds[i];
        Spa(A_GOAL,goalId, one);
    }
}


void SetStartAttribByGroup(int inputNum) {
    int one = 1;
    int sp[] = this->ShuffleIntArray(expandpointgroup(inputNum, GRP_START));
    sourceStartIds = this->ShuffleIntArray(sp);
    int nofIds = len(sourceStartIds);
    for(int i=0;i<nofIds;i++){
        int startId = sourceStartIds[i];
        Spa(A_START,startId, one);
    }
}






//   ###### 
//    ####  
//    ####  
//    ####  
//    ####  
//  ########

void ConfigureWayFinder(int  maxJunctionSelectAttemptsIn, junctionSelectModeIn, checkHeadingIn, checkIntersectingIn; float headingThresholdIn, uvMarginIn  ){

	
    this.maxJunctionSelectAttempts = maxJunctionSelectAttemptsIn; 
    this.junctionSelectMode = junctionSelectModeIn;
    this.headingThreshold = headingThresholdIn;
    this.checkHeading = checkHeadingIn;
    this.checkIntersecting = checkIntersectingIn;
    this.uvMargin = uvMarginIn;
    
    this->SlimInitFromDetailInputOne(); // slim detail load

    this->SetStartAttribByGroup(1);
    this->SetGoalAttribByGroup(1);

    if (verbosity) printf("Start points:\t%d\t", len(sourceStartIds));
    if (verbosity) printf("Goal points:\t%d\n", len(sourceGoalIds));  
    nofSourcePoints = npoints(1);
    resize(sourceOrders,nofSourcePoints);
}

void ConfigureBevel (   int doBevelIn, triPointBevelIn; float bevelSizeAbsIn, bevelDotThresholdIn, bevelCornerOffsetIn ){
    this->LoadFromDetail();
    this.doBevel = doBevelIn;
    this.triPointBevel = triPointBevelIn;
    this.bevelSizeAbs = bevelSizeAbsIn;
    this.bevelDotThreshold = bevelDotThresholdIn;
    this.bevelCornerOffset = bevelCornerOffsetIn;
       
}


void CheckConsistency() {

    int ncOrders = len(createdOrders);
    int ncIds = len(createdIds);
    int ncSourceIds = len(createdSourceIds);
    int ncRanks = len(createdRanks);
    int ncLineIds = len(createdLineIds);
    int ncStates = len(createdStates);
    int ncToNexts = len(createdToNext);
    int ncPositions = len(createdPositions);

    
    int lnPrimsCreated = len(createdPimitives);
    int lnRankPerLine = len(maxRankPerLine);
    int lnOrderMin = len(createdOrderMin);
    int lnOrderMax = len(createdOrderMax);

    int boss = ncIds;
    int lnBoss = lnPrimsCreated;
    if (ncOrders != boss || ncSourceIds != boss || ncRanks != boss || ncLineIds != boss || ncStates != boss || ncToNexts != boss || ncPositions != boss ||
        lnRankPerLine != lnBoss || lnOrderMin != lnBoss || lnOrderMax != lnBoss  
        ) 
     {
        printf("\n-- !! CONSISTENCY ERROR !! -- \n");
        printf("ord:%d id:%d sou:%d rank:%d lid:%d st:%d nxt:%d pos:%d\n", ncOrders, ncIds, ncSourceIds, ncRanks, ncLineIds, ncStates, ncToNexts, ncPositions );
        printf("primCre:%d rankPerLine:%d ordMin:%d ordMax:%d\n", lnPrimsCreated, lnRankPerLine, lnOrderMin, lnOrderMax );
         
     } else {
        if (verbosity) printf("Consistency ok.\n");
     }
}



void PrintBevelConfig() {

    if (verbosity) printf("Bevel size:\t%d\n", bevelSize );
    if (verbosity) printf("Dot threshold:\t%d\t", bevelDotThreshold );
    if (verbosity) printf("Corner offset:\t%d\n", bevelCornerOffset ); 
}

void PrintConfig()
{
	if (verbosity) printf("Intersct check:\t%s\t",  (checkIntersecting ? "Yes" : "No") );
	if (verbosity) printf("Check heading:\t%s", (checkHeading ? "Yes" : "No") );
	if (verbosity) {
		if (checkHeading) printf("\tHeading thrshd:\t%d", headingThreshold);
	}
	if (verbosity) printf("\n");
	if (verbosity) printf("JnctSelAttmpts:\t%d\t", maxJunctionSelectAttempts);
	if (verbosity) printf("JnctSelMode:\t%s\t", (junctionSelectMode ? "Random" : "FirstValid") ); //0-fv 1-rnd
	if (verbosity) printf("Uv margin:\t%d\n", uvMargin);

	if (verbosity) printf("Input seed:\t%d\t", inputSeed);
    if (verbosity) printf("Runtime seed:\t%d\t", seed);

    string bt = "Off";
    if (doBevel && triPointBevel) {
        bt = "3Pnts";
    } else if (doBevel && !triPointBevel) {
        bt = "2Pnts";
    }
    if (verbosity) printf("Bevel type:\t%s\t", bt);
	

    if (doBevel) this->PrintBevelConfig();
}

void IncreaseWayOrder(){
    wayOrder++;
    maxOrder = ( wayOrder  >= maxOrder) ? wayOrder : maxOrder; //store maximum order 
}

int[] CreatedIdsByLineId(int lId){

    int result[] = {};
    for(int i=0;i<nofNew;i++){
        int cId = createdIds[i];
        int clId = createdLineIds[cId];
        
        if (clId == lId) push(result, cId);
    }   
    return result;
}
 

void SubmitJunction(int idx) {

	//if (IsIntInArray(idx, lineJcs)) {
	//	if (verbosity) printf(" *** TROUBLE *** junction:%d\n", idx);
	//	return;
	//}
	push(lineJcs, idx);
}



int IsOverlappingPoints(vector s1,e1,s2,e2)
{
    float dist;
    dist = distance(s1,s2);
    if (dist < smallNumber) return 1;
    dist = distance(s1,e2);
    if (dist < smallNumber) return 1;
    dist = distance(e1,s2);
    if (dist < smallNumber) return 1;
    dist = distance(e1,e2);
    if (dist < smallNumber) return 1;
    return 0;
}

int IsHeadingAligned(vector s1,e1,s2,e2)
{
	float d = dot(normalize(e1 - s1),normalize(e2 - s2));
	return (abs(d) > headingThreshold);
}






//     #########     ##########     ####
//    ####           ####           ####
//     ########      ########       ####
//           ####    ####           ####
//    ####   ####    ####           ####
//    #########      ###########    ###########


// the only catch is to filter out self point... 
void FindTargetsStraight(int pointId, maxPoints; float maxDistance ) {
     
    int nearz[] = {};
    int res = GetNearPoints(pointId, maxPoints,maxDistance, nearz);
    if (res) {
         
        int shuffledIds[] = this->ShuffleIntArray(nearz);

        SetIntArrayAttrib(A_TARGETIDS,pointId,shuffledIds);
        Spa(A_SELECTOR_MAX_PTS,pointId, maxPoints);

    }  else {
        if (verbosity) printf("no nearz\n");
    }

    string selGuid = sprintf("S%dm%d", int(rint(maxDistance * 1000.0)), maxPoints  );
    Dss(A_SELECTOR_GUID, selGuid);
}

// one min - one max - repeat 
// returns the resulting array like:
// [nearIds] [farIds] 
// practically in distance order
void FindTargetsOneMinOneMax(int pointId, maxPoints; float minDistance,  maxDistance ) {
    
    int ids[] = {};
    int oPtnum, oColl;
    float oDistance, matchDist;
    vector oP;
    float pcoRadius = maxDistance * 1.1;

    int numpt = npoints(geoself());
    vector pp = point(geoself(), "P", pointId);
     
    int hnd = pcopen(geoself(), "P", pp, pcoRadius, numpt+1);
    while (pciterate(hnd))
    {
        pcimport(hnd, "point.number", oPtnum);
        pcimport(hnd, "point.distance", oDistance);    

        if (oPtnum == pointId) continue; // skip myself
        if (oDistance < minDistance) continue;
        if (oDistance > maxDistance) continue;

        // ok, we found a point with the distance        
        push(ids, oPtnum);
        
    }
    pcclose(hnd);

    int nofPairs = len(ids);

    if (nofPairs <= 2) {
    	SetIntArrayAttrib(A_TARGETIDS,pointId,ids);
    	Spa(A_SELECTOR_MAX_PTS,pointId, maxPoints);
    	return;
    }
     
    int iLow = 0;
    int iHi = nofPairs -1;
    int lows[] = {};
    int his[] = {};

    for(int i=0;i<maxPoints;i++) {

    	if (i % 2 == 0) {
    		int cId = ids[iLow++];
    		if (IsIntInArray(cId,his))  break; // no points to work with
    		push(lows, cId);

    	} else  {
    		int cId = ids[iHi--];
    		if (IsIntInArray(cId,lows))  break; // no points to work with
    		push(his, cId);
    	}
    }

    his = reverse(his);
    append(lows,his); // [  near1, near2, ..... , farN-1, farN ]

    //if (verbosity) printf("%d\n", lows);
    // no shuffling here 
    SetIntArrayAttrib(A_TARGETIDS,pointId,lows);
    Spa(A_SELECTOR_MAX_PTS,pointId, maxPoints);

    string selGuid = sprintf("O%dm%d",  int(rint(maxDistance * 1000.0)), maxPoints );
    Dss(A_SELECTOR_GUID, selGuid);
}

void FindTargetsAtLeastN(int pointId,  atLeast , maxPoints; float minDistance, maxDistance) {
    
    int ids[] = {};
    int oPtnum ;
    float oDistance, matchDist;

    float pcoRadius = maxDistance * 1.5;
    int numpt = npoints(geoself());
    vector pp = point(geoself(), "P", pointId);
    
    int count =0;
    int hnd = pcopen(geoself(), "P", pp, pcoRadius, numpt+1);
    while (pciterate(hnd))
    {
        pcimport(hnd, "point.number", oPtnum);
        pcimport(hnd, "point.distance", oDistance);    

        if (oPtnum == pointId) continue; // skip myself
        if (oDistance < minDistance) continue;
        count++;
        if (count > atLeast && oDistance > maxDistance) continue;

        // ok, we found a point with the distance        
        push(ids, oPtnum);
        
    }
    pcclose(hnd);

    int closestFirst[] = ids[0:atLeast];
    int others[] = this->ShuffleIntArray(ids[atLeast:]);
    
    int nofOthers = len(others);
    int selected[] = {};
    int cId = -1;

    append(selected, closestFirst);

    for(int i=len(closestFirst);i<maxPoints;i++) {
    	if (i>= nofOthers) break;
    	push(selected, others[i]);
    }

    int shuffledIds[] = this->ShuffleIntArray(selected);
    SetIntArrayAttrib(A_TARGETIDS,pointId,shuffledIds);
    Spa(A_SELECTOR_MAX_PTS,pointId, maxPoints);
    string selGuid = sprintf("N%du%dm%d", atLeast, int(rint(maxDistance * 1000.0)), maxPoints );
    Dss(A_SELECTOR_GUID, selGuid);
}




void FindTargetsDotFilter(int pointId, maxPoints; float minDistance, maxDistance, dotThreshold )
{
    int ids[] = {};
    int oPtnum, oColl;
    float oDistance, matchDist;
    vector oP;
    float pcoRadius = maxDistance * 1.1;

    int numpt = npoints(geoself());
    vector pp = point(geoself(), "P", pointId);
     
    int hnd = pcopen(geoself(), "P", pp, pcoRadius, numpt+1);
    while (pciterate(hnd))
    {
        pcimport(hnd, "point.number", oPtnum);
        pcimport(hnd, "point.distance", oDistance);    
        pcimport(hnd, "P", oP);

        if (oPtnum == pointId) continue; // skip myself
        if (oDistance > maxDistance) continue;
        if (oDistance < minDistance) continue;

        // ok, we found a point with the distance        
        int nofIds = len(ids);        
        if (nofIds >= maxPoints) break;
        
        if (nofIds > 0) { 
            // got at least one target point
            int lastId = ids[nofIds-1];
            vector lastPos = point(geoself(), "P", lastId);
            vector lv = lastPos - pp;
            vector currPos = point(geoself(), "P", oPtnum);
            vector ov = oP - pp;
            float ddot = abs(dot(normalize(lv), normalize(ov)));

            if (ddot < dotThreshold) push(ids, oPtnum);

        } else {
            // first target point, just add
            push(ids, oPtnum);
        }   
    }
    pcclose(hnd);

    int shuffledIds[] = this->ShuffleIntArray(ids);
    SetIntArrayAttrib(A_TARGETIDS,pointId,shuffledIds);
    Spa(A_SELECTOR_MAX_PTS,pointId, maxPoints);

    string selGuid = sprintf("D%du%dm%d", int(rint(dotThreshold * 1000.0)), int(rint(maxDistance * 1000.0)), maxPoints );
    Dss(A_SELECTOR_GUID, selGuid);
}

//  returns 1 if all ok
int StoreIfValid(vector start, end ) {

    if (len(startVectors) == 0) { // no segments yet => just store
        push(startVectors,start);
        push(endVectors,end);
        return 1; // => true
    }

    vector intersectPoint = {0,0,0};
    vector s1,e1,s2,e2;
    s1 = start;
    e1 = end;

    //match against each segment we already have stored
    int nofv = len(startVectors);
    for(int i=0;i<nofv;i++) {
        s2 = startVectors[i];
        e2 = endVectors[i];

        if( this->IsOverlappingPoints(s1,e1,s2,e2))
        {
        	if (checkHeading && this->IsHeadingAligned(s1,e1,s2,e2)) {
                //if (verbosity) printf("heading check failed\n"); 
                last_nofHeadings++;
                return 0; // they are aligned too much into the same direction
            } else {
                continue; // this would fail for intersection check but it's not that
            }
        }
     
     	if (!checkIntersecting) continue;
        
        int res = IntersectLineSegmentsMinimalAxis(s1, e1, s2, e2, intersectPoint);
        if(res) {
            //if (verbosity) printf("intersect check failed\n");
            //if (verbosity) printf("%d %d %d %d\n",s1,e1,s2,e2);
            last_nofIntersects++;
            return 0; //intersecting with existing segment      => false
        }
    }

    //store the survivor
    push(startVectors,start);
    push(endVectors,end);
    return 1; // => true
}




int IsStartOrGoal(int idx) {
    return lookUpStartOrGoal[idx] > 0;
    //return IsIntInArray(idx,sourceStartIds) || IsIntInArray(idx, sourceGoalIds);
}

int IsGoal(int idx) {
    return (lookUpStartOrGoal[idx] & 0xf0) > 0;
    //return IsIntInArray(idx, sourceGoalIds);
}

int IsStart(int idx) {
    return (lookUpStartOrGoal[idx] & 0x0f) > 0;
    //return IsIntInArray(idx, sourceStartIds);
}

void CorrectState(int idx, state) {
    int s = state;
    if (idx == currentStartPointId)  s = STATE_START;
    Spa(A_STATE, idx, s);
    createdStates[idx] = s;
}

void AddState(int idx, state) {
    int s = state;
    if (idx == currentStartPointId)  s = STATE_START;
    Spa(A_STATE, idx, s);
    push(createdStates, s);
}



int AddSegmentVertex(int primId, originalPointId, rank; vector pos, vToNext; int state ) {

	int idx = addpoint(geoself(), pos);
    lastCreatedPointId  = idx;
	Spa(A_LINEID, idx,lineId);
	Spa(A_ORDERI, idx, wayOrder);
    Spa(A_TONEXT, idx, vToNext);
	Spa(A_RANKI, idx, rank);

    push(createdToNext, vToNext);
    push(createdLineIds, lineId);
    push(createdRanks, rank);
	push(createdIds, idx);
	push(createdOrders,wayOrder);
    push(createdSourceIds, originalPointId);
    push(createdPositions, pos);
    lookUpIsUsed[originalPointId] =1;

    sourceOrders[originalPointId] = wayOrder;
    this->AddState(idx, state );
	addvertex(geoself(), primId, idx );
	return idx;
}

int AddSegmentPrimitive(){
    lineId++;
	int pIdx=	addprim(geoself(), "polyline");
    push(createdPimitives, pIdx);
    setprimattrib(geoself(),A_LINEID, pIdx, lineId, "set");
    setprimattrib(geoself(),A_ORDER_FIRST, pIdx, wayOrder, "set"); //the line starting order
    push(createdOrderMin, wayOrder);
    nofPrimitivesCreated++;
    return pIdx;
}


void ResetStats(){
	
	last_nofIntersects =0;
	last_nofHeadings =0;
	last_nofBadJunction =0;
}

void PrintStats(){
	if (verbosity) printf("Stats:\n");
	if (verbosity) printf("Source points:\t%d\t", nofSourcePoints );
	if (verbosity) printf("Order reached:\t%d\t", maxOrder );
	if (verbosity) printf("Prims created:\t%d\n", nofPrimitivesCreated );
	if (verbosity) printf("Intrsct fails:\t%d\t", last_nofIntersects);
	if (verbosity) printf("Heading fails:\t%d\t", last_nofHeadings);
	if (verbosity) printf("Bad junctions:\t%d\n", last_nofBadJunction );
    if (verbosity) printf("Segmnts total:\t%d\n", segmentsTotal );
}

void Init(int startPointId){
    this->ResetStats();
    currentStartPointId = startPointId;
	lastCreatedPointId =0;
	progressBar = 0;
	goalReached = 0;
	
	selectorMaxPoints = point(1,A_SELECTOR_MAX_PTS,startPointId); // the number of max targets value used by the selector
    if (verbosity) printf("StartId:\t%d\tMax targets:\t%d\n", startPointId, selectorMaxPoints );
}

void LineDone(int lastPrimId, currentId, correctState, rank) {
    this->CorrectState(lastCreatedPointId, correctState);
    setprimattrib(geoself(),A_ORDER_LAST, lastPrimId, wayOrder, "set");
    push(createdOrderMax, wayOrder);
    this->SubmitJunction(currentId); // do not use this as a junction
    push(maxRankPerLine,rank);

    int isGoalPrim = 0;
    if (correctState == STATE_GOAL) isGoalPrim =1;
    setprimattrib(geoself(),A_ISGOALLINE, lastPrimId, isGoalPrim, "set");
}

void CreateStartOrGoalLookUp() {

    vector pos;
    int isStart, isGoal, res;
    int nofPt = npoints(1);
    resize(lookUpStartOrGoal, nofPt);
    resize(lookUpPosition, nofPt);
    resize(lookUpIsUsed, nofPt);
    for (int i=0; i < nofPt; i++){

        res =0;

        isStart = point(1,A_START, i);
        if (isStart) res |= 0x01;

        isGoal = point(1,A_GOAL,i);
        if (isGoal) res |= 0x10;

        lookUpStartOrGoal[i] = res;

        pos = point(1,"P",i);
        lookUpPosition[i] = pos;
    }
}

int IsOtherAStartPoint(int otherId) {
    return ((lookUpStartOrGoal[otherId] & 0x0f) > 0 );
}

vector LookUpPosition(int idx) {
    return lookUpPosition[idx];
}






//    #########   ####     ####
//    ####        ####     ####
//    #######     ####  #  ####
//    ####        #### ### ####
//    ####        ###### ######
//    ####        ####     ####





void FindMyWay(int startPointId ) {

	this->Init(startPointId);

    int currentId = startPointId;	
	int targetIds[] = {};
	int jcId = -1;
    int primId = -1;
    int nofT, otherId;
    
    int rank = 0;
     
    int foundTarget = 0;
    int foundJunction = 0;
    int success;
    int isDeadEnd = 0;
    int lastSelectedAsJunction = -1;
    int st; // work stuff

     
     
    while(1) {
        //if (verbosity) printf("processing:%d\n",currentId);

        int goal = this->IsGoal(currentId);
        if (goal == 1) {
            if (verbosity) printf("\n >>>> Arriving to goal at: %d\n", currentId);
            goalReached = 1;
            this->LineDone(primId, currentId,  STATE_GOAL, rank);
            break; // exit while >>
        }

        isDeadEnd = 0;
        targetIds = pointattrib(1, A_TARGETIDS, currentId, success); // get the target id array from the source point
        int nofTargets = len(targetIds);
        if (nofTargets <= 0 ) { // no targetIds => dead end
            //if (verbosity) printf("\n >>>> Dead end at:%d\n", currentId);
            isDeadEnd = 1;
            foundTarget = 0;
            // it will continue and select a new junction
        } else { // select otherId from the targetids[] attribute

            foundTarget = 0;
            for(int i=0; i<nofTargets; i++)
            {
                otherId = targetIds[i];
                if (this->IsOtherAStartPoint(otherId)) continue; // it is a start point, dont use it

                if (this->IsUsed(otherId)) { //if (verbosity) printf("currentId:%d otherId:%d was used.\n", currentId, otherId);
                    continue; }

                vector op = this->LookUpPosition(otherId);
                vector pp = this->LookUpPosition(currentId);
                if (!this->StoreIfValid(pp,op )) { //if (verbosity) printf("store refused:%d %d\n", currentId, otherId);
                    continue; }

                //if (verbosity) printf("valid found:%d\n",otherId,wayOrder);
                segmentsTotal++;
                 
                vector vToNext = op - pp;
                    
                if (primId < 0) { // starting a new prim => 2 points
                    
                    if (lineId > 0) { // skip first round - so we know: a line was finished
                        //if (rank >0) push(maxRankPerLine,rank); // any progress was made
                        rank = 0; // reset per line count
                    }

                  
                    primId = this->AddSegmentPrimitive();
                        
                    int newStartId = this->AddSegmentVertex(primId, currentId, rank++, pp, vToNext, STATE_ROOT ) ;
                    this->IncreaseWayOrder();
                    int newEndId = this->AddSegmentVertex(primId, otherId, rank, op, vToNext, STATE_LINE ) ;
                    
                     
                    if (lastSelectedAsJunction >=0) {
                        this->CorrectState(lastSelectedAsJunction, STATE_JUNCTION);
                    }

                } else { // continuing a line => 1 point

                    this->IncreaseWayOrder();
                    rank++;
                    Spa(A_TONEXT, lastCreatedPointId, vToNext);
                    createdToNext[len(createdToNext)-2] = vToNext; // reach back here also!
                    int newEndId =this->AddSegmentVertex(primId, otherId, rank,  op, vToNext, STATE_LINE ) ;
                }
                
                //maxOrder = ( wayOrder  >= maxOrder) ? wayOrder : maxOrder; //store maximum order 
                currentId = otherId; // make the step! => line
                foundTarget = 1;
                break;
            } // attempts to select a target
        } // has any targetId
        
        if (foundTarget==0) { //no proper target was found

            //if (verbosity) printf("lastPrimId:%d lastPoint:%d lastOrder:%d\n", primId, lastCreatedPointId, (wayOrder - 1));
            if (primId < 0)
            {
                // a proper junction got selected, but nowhere to go from the specified junction
                // no new primitive or point got created
                if(len(lineJcs) >= this->NofUsedPoints())
                {
                    if (verbosity) printf("\n >>>> All linepoints used up as junction\n");
                    this->CorrectState(lastCreatedPointId, STATE_GIVEUP);
                    break;
                }

                last_nofBadJunction++;
            }
            else
            { 
            	// at least one primitive got created 
                string prgChar = ".";
                if (rank > 5) prgChar = ",";
                if (rank > 10) prgChar = ":";
                if (rank > 15) prgChar = ";";
                if (rank > 20) prgChar = "|";
                if (rank > 45) prgChar = "&";
                if (rank > 95) prgChar = "#";

            	if (progressBar % PROGRESSBAR_BLOCKSIZE == 0) { 
            		 	if (verbosity) printf(" %s",prgChar);

            	} else if (progressBar % (PROGRESSBAR_BLOCKSIZE * PROGRESSBAR_NOFBLOCKS) == (PROGRESSBAR_BLOCKSIZE * PROGRESSBAR_NOFBLOCKS) -1) {
            		 	if (verbosity) printf("%s\n", prgChar);
            		 
            	} else {
            		 	if (verbosity) printf("%s", prgChar);
            	}
            	progressBar ++;
                
                // this is the end of a line primitive
                st = isDeadEnd ? STATE_DEAD_END : STATE_LEAF;
                this->LineDone(primId, currentId, st, rank);
            }

            primId = -1; // tigger new prim start
            
            // junction select modes start ----
            foundJunction=0;
            lastSelectedAsJunction = -1;
            if (junctionSelectMode == 1) {
                for(int i=0; i<maxJunctionSelectAttempts; i++) {
                    jcId = this->RndLineId();
                    if (this->IsStartOrGoal(jcId)) continue;
                    if(!IsIntInArray(jcId, lineJcs)) {
                        foundJunction =1;
                    	break;	
                    }
                }
            } else { // default - counting backwards until the first good one
                nofT = this->NofUsedPoints();
                for(int i=nofT-2;i>=0;i--) {

                    jcId = createdSourceIds[i];
                    if (this->IsStartOrGoal(jcId)) continue;
                    if(!IsIntInArray(jcId, lineJcs)) {
                    	foundJunction =1;
                    	break;	
                    }
                }
            } // junction select modes end ----
            
            if (foundJunction== 0) {
            	if (verbosity) printf("\n >>>> No proper junction was found using attempts: %d\n", maxJunctionSelectAttempts);
                this->CorrectState(lastCreatedPointId, STATE_NO_JUNCTION);
                break; // from while - exit
            }

            // find the created point at the junction
            nofT = len(createdIds);
            for(int i=nofT-1;i>=0;i--) {
                if (createdSourceIds[i] == jcId)
                {
                    lastSelectedAsJunction = createdIds[i];
                    break;
                }
            }
            
            this->SubmitJunction(jcId); // store the selected junction
            currentId = jcId; //make the step => junction
            wayOrder = sourceOrders[jcId]; // continue calculating from the junction root
               
        } // if closing - no target found handling

    } // main while
  
    //if (verbosity) printf("bye!\n");
}

// from input #1
void AddUnusedPoints(){
     
    printf("Adding unused points\n");
    Spa(A_UNUSED,0,0); // init the point attrib on the geometry, so split will work properly
    return;
    for(int i= 0;i<nofSourcePoints;i++){

        if (!this->IsUsed(i)){
            vector pp = point(1,"P",i);
            int pid = addpoint(geoself(), pp);
            Spa(A_UNUSED,pid,1);
        }
    }

    printf("Adding unused points done\n");
}

void GenerateUv(){
    printf("Generating Uv\n");
    
    float uvOffset = uvMargin;
    float uvScale = 1.0 - (uvMargin * 2);

    //if (verbosity) printf("maxOrder:%d uvMargin:%d uvOffset:%d uvScale:%d\n",maxOrder,uvMargin,uvOffset,uvScale );
    float calcOrder = float(maxOrder);

    resize(createdUvs, nofNew);
    for(int i=0;i<nofNew;i++){
        int nId = createdIds[i];
        int nOrder = createdOrders[i];
        float oNorm = nOrder / calcOrder;
        oNorm *= uvScale;
        oNorm += uvOffset;
        vector uv = set(oNorm,0.5,0);
        Spa("uv",nId,uv);
        createdUvs[i] = uv;
    }

    printf("Uv done\n");
}

void NormalizeRank() {
    printf("Normalize rank\n");
    resize(normalizedRanks, nofNew);
    for(int i=0;i<nofNew;i++){
        int cId = createdIds[i];
        int cRank = createdRanks[i];
        int cLineId = createdLineIds[i];
        int rankMax = maxRankPerLine[cLineId-1];
        float rankNorm = cRank / float(rankMax);
        Spa(A_RANKN, cId, rankNorm);
        normalizedRanks[i] = rankNorm;
    }
    printf("Rank done\n");
}

vector CalculateUp(vector vToNext){
    vector result,kross,cUp;
    vector upA = { 0,1,0 };
    vector upB = { 0,0,1 };
    cUp = upA;
    if ( (1.0- dot(cUp, vToNext)) < smallNumber ) cUp = upB;
    kross = cross(cUp, vToNext);
    return  -1* cross(kross, vToNext);
}

void GenerateUpOnRawLines() {
    printf("Calculate up\n");
    vector cToNext, resUp, nToNext;
    for(int i=0;i<nofNew;i++){
        int cId = createdIds[i];
        cToNext = createdToNext[i];
        nToNext = normalize(cToNext);
        resUp = this->CalculateUp(nToNext);
       
        Spa(A_UP,cId, resUp);
        Spa(A_DISTTONEXT,cId, length(cToNext));
    }
    printf("Up done\n");
}

void MakeFloatOrder() {
    printf("Float order\n");
    for(int i=0;i<nofNew;i++){
        int cId = createdIds[i];
        int cOrder = createdOrders[i];
        Spa(A_ORDERF, cId, float(cOrder));
    }
    printf("orderF done\n");
}

//----- bevel stuff

int bevelCurrentLineId =0;
vector bevelPositions[] = {};
int bevelStates[] = {};
float bevelOrders[] ={};
float bevelRankN[] ={};
vector bevelUvs[] ={};
vector bevelToNext[] ={};
vector bevelUp[] ={};
float bevelDistToNext[] ={};


void ResetBevelData()  {
    
    resize(bevelPositions,0);
    resize(bevelStates,0);
    resize(bevelOrders,0);
    resize(bevelRankN,0);
    resize(bevelUvs,0);
    resize( bevelToNext,0);
    resize( bevelUp,0);
    resize( bevelDistToNext,0);

}

void GenerateUpOnBevelLine() {

    vector cToNext, resUp, nToNext;
    int nofBevel = len(bevelPositions);
    resize(bevelUp, nofBevel);
    resize(bevelDistToNext, nofBevel);
    for(int i=0;i<nofBevel;i++){
         
        cToNext = bevelToNext[i];
        nToNext = normalize(cToNext);
        resUp = this->CalculateUp(nToNext);
        bevelUp[i] = resUp;
        bevelDistToNext[i] = length(cToNext);
    }
}


void CalculateBevelToNext() {

    vector a,b,tv;
    int nofBevel = len(bevelPositions);
    resize(bevelToNext, nofBevel);
    for (int i=0; i < nofBevel; i++) {

        if (i== (nofBevel-1)) { // first point
            a = bevelPositions[i];
            b = bevelPositions[i-1];
            tv = a - b;
        } else {
            a = bevelPositions[i];
            b = bevelPositions[i+1];
            tv = b - a;
        }

        bevelToNext[i] = tv;
    }
}


int DrawBeveledLine( ) {

    int priId = addprim(geoself(), "polyline");
    for(int i=0;i<len(bevelPositions);i++) {
        vector ppp = bevelPositions[i];
        int pid = addpoint(geoself(), ppp ) ;
        addvertex(geoself(), priId, pid);
        
        Spa(A_STATE,pid, bevelStates[i] );
        Spa(A_ORDERF,pid, bevelOrders[i] );
        Spa(A_RANKN,pid, bevelRankN[i]);
        Spa(A_LINEID,pid, bevelCurrentLineId);
        Spa(A_UV,pid, bevelUvs[i]);
        Spa(A_TONEXT,pid, bevelToNext[i]);
        Spa(A_UP,pid, bevelUp[i]);
        Spa(A_DISTTONEXT, pid,bevelDistToNext[i]);
    }
    setprimattrib(geoself(),A_BEVEL, priId, 1, "set"); // DEBUG
    return priId;
}



void EmitIdxOriginal(int idx) {
    
    if (idx < 0) return;

    push(bevelPositions, createdPositions[idx]);
    push(bevelStates, createdStates[idx]);
    push(bevelOrders, float(createdOrders[idx]));
    push(bevelRankN, normalizedRanks[idx]);
    push(bevelUvs, createdUvs[idx]);
     
}

void EmitBAsCorner(int a, b, c; vector beforeB, cornerP, afterB; float ratioA, ratioB) {

    int oA = createdOrders[a];
    int oB = createdOrders[b];
    int oC = createdOrders[c];
    float oBeforeB = oB - ratioA;
    float oAfterB = oB + ratioB;
    
    float rankB = normalizedRanks[b];
    float rankBeforeB = fit01((1.0 - ratioA), normalizedRanks[a], normalizedRanks[b] );
    float rankAfterB = fit01(ratioB, normalizedRanks[b], normalizedRanks[c] );

    vector uvB = createdUvs[b];
    vector uvBB = fit01((1.0 - ratioA), createdUvs[a], createdUvs[b] );
    vector uvAB = fit01(ratioB, createdUvs[b], createdUvs[c] );

    // Before B
    push(bevelPositions,beforeB);
    push(bevelStates, STATE_BEVEL_FIRST);
    push(bevelOrders, float(oBeforeB));
    push(bevelRankN, rankBeforeB);
    push(bevelUvs, uvBB);
 
    // Corner
    if (triPointBevel) {
        push(bevelPositions,cornerP);
        push(bevelStates, STATE_BEVEL_TRI);
        push(bevelOrders, float(oB));
        push(bevelRankN, rankB);
        push(bevelUvs, uvB);
    }

    // After B
    push(bevelPositions,afterB);
    push(bevelStates, STATE_BEVEL_SECOND);
    push(bevelOrders, oAfterB);
    push(bevelRankN, rankAfterB);
    push(bevelUvs, uvAB);
}

// segId == -2 is a special case
void ProcessCorner(int a, b,c, segId, emitC) {
         
    int stateA = createdStates[a];
    int stateB = createdStates[b];
    int stateC = createdStates[c];

    vector pa = createdPositions[a];
    vector pb = createdPositions[b];
    vector pc = createdPositions[c];
    
    vector vab = pb - pa;
    vector vbc = pc - pb;
    float dott = dot(normalize(vab), normalize(vbc)); // ez mehene eleve attribnak!

    int doCorner = 1;
    if (dott > bevelDotThreshold) doCorner=0; // filter be angle

    float da = length(vab);
    float db = length(vbc);
    float bevelDividerA = da * bevelSize; // compensate edge distance
    float bevelDividerB = db * bevelSize;
    float ratioA = 1.0 / bevelDividerA;
    float ratioB = 1.0 / bevelDividerB;

    vector afterA = (vab / bevelDividerA) + pa;
    vector beforeB =  pb - (vab / bevelDividerA);
    vector afterB = pb + (vbc / bevelDividerB);
    vector beforeC = pc - (vbc / bevelDividerB);

    vector midP = (beforeB + afterB ) / 2.0;
    vector cornerDefault = (midP + pb) / 2.0;
    vector cornerP = cornerDefault + ( bevelCornerOffset * (pb - cornerDefault ) );

    if (segId == 0 || segId == -2) { // two segments or starting segment

        this->EmitIdxOriginal(a);
        if (stateB == STATE_LINE && doCorner) { 
            this->EmitBAsCorner(a,b,c, beforeB,cornerP,afterB, ratioA, ratioB);
        } else {
            this->EmitIdxOriginal(b);
        }

    } else if (segId == -1) { // single segment - not used, emitted directly
    } else { 

        if (stateB == STATE_LINE && doCorner) {
            this->EmitBAsCorner(a,b,c,beforeB,cornerP,afterB, ratioA, ratioB);
        } else {
            this->EmitIdxOriginal(b);
        }
    }

    if (emitC) this->EmitIdxOriginal(c); // other than that emit c if needed
}

void CreateAttribsOnBevelPrimitive(int bevelPrId, origPrId) {
    setprimattrib(geoself(),A_LINEID, bevelPrId, origPrId+1, "set"); // DEBUG
    setprimattrib(geoself(), A_ORDER_FIRST, bevelPrId, createdOrderMin[origPrId], "set"); // DEBUG
    setprimattrib(geoself(), A_ORDER_LAST, bevelPrId, createdOrderMax[origPrId], "set"); // DEBUG
}






//    #########               
//    ####   ####             
//    #########               
//    ####   ####             
//    ####   ####             
//    #########




void BevelCreatedLines() {
   

    if (verbosity) printf("Bevel ");
    bevelSize = 1.0 / bevelSizeAbs;
    
    int nofPInLine, nofSegInLine,a,b,c;
    int nofLines = len(maxRankPerLine);
    int pointsInLine[];
    for(int cLId=1; cLId <= nofLines; cLId++ ) { // foreach line
        bevelCurrentLineId = cLId;
        this->ResetBevelData();
        
        nofSegInLine = maxRankPerLine[cLId-1];
        
        pointsInLine = this->CreatedIdsByLineId(cLId);
 
        if (nofSegInLine < 2) { // one segment
            // emit source as is
            a = pointsInLine[0];
            b = pointsInLine[1];
            this->EmitIdxOriginal(a);
            this->EmitIdxOriginal(b);

        } else if (nofSegInLine == 2) { // two segments 
            
            this->ProcessCorner(pointsInLine[0],pointsInLine[1],pointsInLine[2],-2,1); // process a,b,c and do last 
            
        } else { // more segments

            nofPInLine = len(pointsInLine);
            for(int segId=0; segId < nofSegInLine-1; segId++) { // foreach segment
                    
                a = pointsInLine[segId];
                b = pointsInLine[segId+1];
                c = pointsInLine[segId+2];

                if (segId == 0 ) { // first segment
                    this->ProcessCorner(a,b,c, segId,0);

                } else if (segId == (nofSegInLine-2) ) { // last segment
                    this->ProcessCorner(a,b,c, segId,1);

                } else { // line segment
                    this->ProcessCorner(a,b,c, segId,0);
                }
            } // foreach segments
        } // end more segments
         
        this->CalculateBevelToNext();
        this->GenerateUpOnBevelLine();
        int bevelPrId = this->DrawBeveledLine(); 
        this->CreateAttribsOnBevelPrimitive(bevelPrId, cLId-1);
        if (verbosity) printf(".");

    } // foreach line

    if (verbosity) printf("\n");
    doBevel = 1;

}








//     #########              
//    ####                 
//    ####                    
//    #### ######             
//    ####   ####             
//     ##########
//           ####


void FindMyWayGroup(){
    this->ResetRandom(inputSeed + 774503);  // selectors reset once in the beginning, waffinders before running a session
    this->PrintConfig();
	int wayCount = 0;
	goalCount = 0;
	int spts = 1;
	 
	this->ResetStats();
	this->CreateStartOrGoalLookUp();
	int nofStarters = len(sourceStartIds);
	for(int i=0;i<nofStarters;i++){
		int starterId = sourceStartIds[i];

		int oldPrimCount = nofPrimitivesCreated;
		int oldLCPId = lastCreatedPointId;
		this->FindMyWay(starterId);

		int nofPCre = nofPrimitivesCreated - oldPrimCount;
		int nofSCre = lastCreatedPointId - oldLCPId;

		if (nofPCre > 0) wayCount++;
		if (goalReached) goalCount++;
        printf("\n");
		this->PrintStats();
        printf("-----\n");
		if (verbosity) printf("Nof new prims:\t%d\tNof new segs:\t%d\t", nofPCre, nofSCre );
		if (verbosity) printf("Goal reached:\t%s\n", (goalReached ? "Yes" : "No") );
		if (verbosity) printf("Progress:\t%d/%d\tAnyPrim:\t%d/%d\tGoalReach:\t%d/%d\n", spts++, nofStarters, wayCount, nofStarters, goalCount, nofStarters );
        printf("\n");
	}
    nofNew = len(createdIds);
    this->CheckConsistency();
	this->PrintStats();
    this->GenerateUv();
    
    this->NormalizeRank();
    this->GenerateUpOnRawLines();
    this->MakeFloatOrder();
    this->SaveToDetail();
    if (verbosity) printf("Processing of ways done.\n");
}

// bevel init will load all
void BevelTheWay() {
    if (doBevel) this->BevelCreatedLines();
    
    this->SaveToDetail();
    //if (verbosity) printf("Bevel done.\n");
}


// finalize stuff
void FinishTheWay() {
    this->LoadFromDetail();
    this->AddUnusedPoints();
    //if (verbosity) printf("WayFinder done.\n");
    this->MakeGuid();
    this->SaveToDetail();
    if (verbosity) printf("Way finished\t%s\t%s\n", this->GetNodeName(), wayGuid);
}




// init the thing
void PrepareWay(int verbosityIn, seedIn) {
    verbosity = verbosityIn;
    seed = seedIn;
    inputSeed = seedIn;
    Dsi("verbosity", verbosity);
    Dsi("seed", seed);
    Dsi("inputSeed",inputSeed);
    this->SetGoalAttribByGroup(geoself());
    this->SetStartAttribByGroup(geoself());
    if (verbosity) printf("\n\nWayFinder prepared.\n");
}



void DrawPossibilities(){
    this->ComplexityLevel();
    printf("Draw possibilites\n");
    float dist, distMin, distMax, pairMin, pairMax;
    vector cPos, tPos;
    int numPts = npoints(geoself());
    int targetids[] = {};
    int nofTargetIds =0;
    int tIdx = 0;
    //if (verbosity) printf("nofp: %d\n", numPts);
    distMax = -1;
    distMin = 100000000;
    pairMin = 10000000;
    pairMax = -1;

    int targetPerPoint[] = {};
    float distances[] = {};

    int primId =0;
    
    for (int i=0;i<numPts;i++){
        
        targetids = GetIntArrayAttrib(A_TARGETIDS, i);
        nofTargetIds = len(targetids);
        if (nofTargetIds <= 0) continue;
        push(targetPerPoint, nofTargetIds);
        
        if (nofTargetIds < pairMin) pairMin = nofTargetIds;
        if (nofTargetIds > pairMax) pairMax = nofTargetIds;

        cPos = point(geoself(), "P", i);

        for(int j=0; j < nofTargetIds; j++) {
            tIdx = targetids[j]; 
            tPos = point(geoself(), "P", tIdx);  
            dist = length(tPos - cPos);
            if (dist < distMin ) distMin = dist;
            if (dist > distMax ) distMax = dist;
            push(distances, dist);

            primId= addprim(geoself(), "polyline", i, tIdx);
        }
    }

    float sumTpp = 0.0;
    for(int i=0;i<len(targetPerPoint); i++) {
        sumTpp += targetPerPoint[i];
    }

    if (verbosity) printf("Distance min: %d\tmax: %d\tavg: %d\n", distMin, distMax, avg(distances));
    if (verbosity) printf("TargetPp min: %d\tmax: %d\tavg: %d\n", pairMin, pairMax, avg(sumTpp / len(targetPerPoint) ));

}

void AddStateName(int pointId) {

    int state = point(geoself(),A_STATE,pointId);
    string stName = "";
    if (state == STATE_LINE) stName = "+";
    if (state == STATE_LEAF) stName = "#";
    if (state == STATE_GOAL) stName = "goal";
    if (state == STATE_NO_JUNCTION) stName = "0";
    if (state == STATE_ROOT) stName = "root";
    if (state == STATE_JUNCTION) stName = "Y";
    if (state == STATE_DEAD_END) stName = "dead";
    if (state == STATE_BEVEL_FIRST ) stName = "a";
    if (state == STATE_BEVEL_SECOND ) stName = "b";
    if (state == STATE_BEVEL_TRI ) stName ="C";
    if (state == STATE_START ) stName ="start";
    if (state == STATE_GIVEUP ) stName ="tired";
    Spa(A_STATENAME,pointId,stName);

}



void MergeShpTargets() {
     
    printf("Merged shp targets\n");
    int origpt, prevpt;
    int targetIds[];
    int nofShpPoints = npoints(1);
    if (nofShpPoints <= 0 ) return;
    for(int i=0; i < nofShpPoints; i++) {

        origpt = point(1,"origpt",i);
        prevpt = point(1,"prevpt",i);

        if (prevpt < 0) continue;

        targetIds = GetIntArrayAttrib(A_TARGETIDS, prevpt);
        if (!IsIntInArray(origpt, targetIds)){
            push(targetIds, origpt);
            SetIntArrayAttrib( A_TARGETIDS, prevpt, targetIds);
        } 
    }
}









//     #########     ####           #########
//    ####           ####           ####   ####
//     ########      #########      #########
//           ####    ####   ####    ####
//    ####   ####    ####   ####    ####
//    #########      ####   ####    ####


void AddNextNearest(int pointId, nofPtoAdd ) {
    
    // generate random cost for shortest path
    vector sz = getbbox_size(geoself());
    float radius = length(sz);
    
    int targets[] = GetIntArrayAttrib( A_TARGETIDS, pointId);
    
    int nofInput = npoints(geoself());
    int nearIds[];
    int amount = 1;
    int success = 0;
    int pid;
    int found = 0;
    while(1) {
        
        int nres = GetNearPoints(pointId, amount, radius, nearIds);
        //printf("id:%d amount:%d nearIds:%d\n", pointId, amount, nearIds);
        if (nres) {
            
            for (int i=amount-1;i<len(nearIds);i++) {
                //printf("i:%d  amount:%d\n",i, amount);
                pid = nearIds[i];
                if (!IsIntInArray(pid, targets)){
                    push(targets, pid);
                    found++;
                    break; // for
                } 
            }
            if (found >= nofPtoAdd) {
                SetIntArrayAttrib(A_TARGETIDS, pointId, targets);
                break; // while
            }
            
        } else {
            break; //no nears -> exit
        }

        amount++;
        if (amount >= nofInput) {
            printf("WARNING!\tAll input points got checked. check function:AddNextNonTargetAsTarget\tWARNING!\n");
            break;
        }

    } // end while
}

void MarkAsJunction(int sourcePointId) {

    int createdId;
    for(int i=0; i<len(createdIds); i++) {
        if (createdSourceIds[i] == sourcePointId) {
            createdId = createdIds[i];
            this->CorrectState(createdId, STATE_JUNCTION);
            break;
        }
    }
}



void AddShortesPathResult() {

    printf("adding shortest path result to ways\n");
     
    int a,b, isLast,isFirst,nofp, primId;
    vector aPos,bPos,vAb;
    int pid ,origPt, nofRawPoints, firstPathId;
    int usedOrigPoints[]={};
    int pathIds[] = {};
    vector pathPos[] = {};
    int rawPoints[] = {};
    vector origPos = {0,0,0};

    int nPrim = nprimitives(1);
    if (nPrim <= 0) {
        printf("WARNING! Error occured extending with shortest path. No primitives on input #1.\tWTF?\n");
        return;
    }

    if (verbosity) printf("%d shortest path(s).\n", nPrim);
    for (int cPrimId = 0; cPrimId < nPrim; cPrimId++ ) {

        int rank = 0;
        resize(pathIds,0);
        resize(pathPos,0);
        int geoReached = 0;
        
        rawPoints = primpoints(1,cPrimId );
        nofRawPoints = len(rawPoints);
        
        if (nofRawPoints <= 1) continue; // safety wtf

        for(int j = 0; j < nofRawPoints; j++) {

            pid = rawPoints[ (nofRawPoints-1) - j ]; // go reverse from goal 
            origPt = point(1,"origpt", pid); // get attrib by h
            origPos = point(1,"P", pid);

            push(pathIds, origPt);
            push(pathPos, origPos);

            if (this->IsUsed(origPt )) {
                geoReached = 1;
                break;
            } 

        } // end one path scan
         
        //if (verbosity) printf("Shortest path primId:%d points changed: %d -> %d\n", cPrimId, nofRawPoints, len(pathIds));

        // for drawing order
        pathIds = reverse(pathIds);
        pathPos = reverse(pathPos);

        //build geometry for shortest path

        firstPathId = pathIds[0];
        wayOrder = geoReached ? sourceOrders[firstPathId] : maxOrder;

        nofp = len(pathIds);
        if (nofp <= 1) continue;

        primId = this->AddSegmentPrimitive();

        for (int i=0; i< nofp-1; i++) {
            isLast = (i ==nofp-2);
            isFirst = (i==0);

            a = pathIds[i];
            b = pathIds[i+1];
            aPos = pathPos[i];
            bPos = pathPos[i+1];
            vAb = bPos- aPos;

            int pState = STATE_LINE;
            if (isFirst) {
                pState = STATE_ROOT;
                // mark point as junction on original
                this->MarkAsJunction(a);
            }

            // A and B was used
            if (IsIntInArray(a, usedOrigPoints) && IsIntInArray(b, usedOrigPoints)) continue;

            // A:not B:yes -> end stuff here
            if (!IsIntInArray(a, usedOrigPoints) && IsIntInArray(b, usedOrigPoints))  isLast=1;

            int p1 = this->AddSegmentVertex(primId, a, rank,  aPos, vAb, pState  );
            segmentsTotal++;
            
            rank++;
            this->IncreaseWayOrder();
            if (!IsIntInArray(a, usedOrigPoints)) push(usedOrigPoints, a);
                                    
            // add last
            if (isLast) {
                
                int p2 = this->AddSegmentVertex(primId, b, rank,  bPos, vAb, STATE_GOAL  );
                
                this->LineDone(primId, b, STATE_GOAL , rank);
                break;
            }

        } // one path
        printf(".");
    } // for all path prim
    printf("\nadding shp result done\n");
}

void AddShortestPathGuid() {
    string spGuid = "_P";
    Dss(A_GUID_SHORTPATH,spGuid);
}


// Try to fix the result with shortest path 
void SaveTheWay() {
    this->LoadFromDetail();
    this->AddShortesPathResult();
    nofNew = len(createdIds);
    this->CheckConsistency();
	this->PrintStats();
    this->GenerateUv();
    this->NormalizeRank();
    this->GenerateUpOnRawLines();
    this->MakeFloatOrder();
    this->AddShortestPathGuid();
    this->SaveToDetail();
}


int validateIntersect = 1;
int validStartIds[] = {};
int validEndIds[] = {};
vector validStartPos[] = {};
vector validEndPos[] = {};
int IsPairStored(int startId, endId) {

    int cs, ce;
    int nofv = len(validStartIds);
    for(int i=0; i < nofv; i++) {
        cs = validStartIds[i];
        ce = validEndIds[i];

        // yes stored, nono
        if ((cs == startId && ce == endId ) || (ce == startId && cs == endId ) ) {
            return 1;
        }
    }
    return 0; // not stored, ok!
}


int ValidateTargetPair(int startId, endId; vector startPos, endPos) {
    
    vector s1,e1,s2,e2;
    vector intersectPoint  = {0,0,0};
    int nofv = len(validStartPos);   
    int psId,peId;

    if (nofv <= 0 ) { // store
        push(validStartIds, startId);
        push(validEndIds, endId);
        push(validStartPos, startPos);
        push(validEndPos, endPos);
        return 1;
    }

    // s < - > e
    if (this->IsPairStored(startId, endId)) return 0; // not valid, already have it
    
    s1 = startPos;
    e1 = endPos;

    for(int i=0; i < nofv; i++) {
        s2 = validStartPos[i];
        e2 = validEndPos[i];
        psId = validStartIds[i];
        peId = validEndIds[i];
        
        // same start id, different end id - don't check intersect
        if (psId == startId && peId != endId) continue;
        if (psId != startId && peId == endId) continue; // and reverse
        if (psId == startId || peId == startId || psId == endId || peId == endId ) continue;

        //printf("\n %d - %d vs %d - %d\n", s1,e1,s2,e2 );
        //printf("intersect check on: %d->%d  vs  %d->%d  ", startId, endId, psId, peId );
        if (!validateIntersect) continue;

        int res = IntersectLineSegmentsMinimalAxis(s1, e1, s2, e2, intersectPoint);
        if(res) {
            //printf("intersecting\n");
            return 0; // not valid =>  intersecting
        }
        //printf("no intersect\n");

    }
    push(validStartIds, startId);
    push(validEndIds, endId);
    push(validStartPos, startPos);
    push(validEndPos, endPos);
    return 1;
}

void KeepNonCrossingOnPoint(int pointId) {
   
    vector startPos, endPos;
    int startId, endId, isValid, nofTargets;
    int targetIds[] = {};
    int filteredTargets[] = {};

    startId = pointId;
    targetIds = GetIntArrayAttrib(A_TARGETIDS, startId);
    nofTargets = len(targetIds);
    if (nofTargets <= 0) return;
    startPos = point(geoself(), "P", startId);

    resize(filteredTargets, 0);
    for(int j=0; j < nofTargets; j ++) {
        
        endId =  targetIds[j];
        endPos = point(geoself(), "P", endId);

        isValid = this->ValidateTargetPair(startId, endId, startPos, endPos);
        if (isValid) {
            push(filteredTargets , endId);
        }
    }

    SetIntArrayAttrib(A_TARGETIDS, startId, filteredTargets);
  
}



void KeepNonCrossingTargets(int validateIntersectIn) {
    this.validateIntersect = validateIntersectIn;
     int nofPt = npoints(geoself());
     if (nofPt <= 0) return;
     printf("KeepNonCrossing on points:%d\n", nofPt);
     float fraction, percent;
     int percentInt;
     int pCount = 0;

     int lastPercent =0;
    for(int i=0; i < nofPt; i++ ) {

        this->KeepNonCrossingOnPoint(i);
        pCount++;
        fraction = pCount / float(nofPt);
        percent = 100.0 * fraction;
        percentInt = int(rint(percent));
        if (percentInt % 10 == 0 && lastPercent != percentInt){
            printf("#");
            lastPercent = percentInt;
        } 
        if (pCount % 25 == 0) printf(".");
    }
    printf("\n");
}


void AddNeighboursAsTargets(int pointId) {
    
    int np[] = {};
    np = neighbours(geoself(), pointId);
    SetIntArrayAttrib(A_TARGETIDS, pointId, np);
}



void SetupSinglePrimForAdjacency(int primId; float totalDist){
    //printf("\n\n");
   
    float distanceSum = 0;
    float distances[] ={};
    float diffLen;
    vector diff;
    vector curPos = {0,0,0};
    vector prevPos = {0,0,0};
    vector nextPos = {0,0,0};
    vector toNext = {0,0,0};
    int curId, prevId, nextId;
    int pointIds[];
    pointIds = primpoints(geoself(), primId);
    int nofPoints = len(pointIds);
    if (nofPoints <= 1) return;

    int rank = 1;
    int iteration = 0;
    int isFirst, isLast;

    if (nofPoints == 2 ) {// two point segment case
        curId = pointIds[0];
        nextId = pointIds[1];
        curPos = point(geoself(),"P", curId);
        nextPos = point(geoself(),"P", nextId);
        
        diff = nextPos - curPos;
        diffLen = length(diff);
        

        Spa(A_RANKI, curId, -1); // start sign
        Spa(A_RANKN, curId, 0.0);
        Spa(A_PREVPOS, curId, curPos - diff); // start previous
        Spa(A_NEXTPOS, curId, nextPos); // start next
        Spa(A_DISTALONGCURVE, curId, totalDist);
        Spa(A_DISTTONEXT, curId, diffLen);
        Spa(A_TONEXT, curId, diff );

        distanceSum += diffLen;
        totalDist += diffLen;

        Spa(A_RANKI, nextId, -2); // end sign
        Spa(A_RANKN, nextId, 1.0);
        Spa(A_PREVPOS, nextId, curPos); // end previous
        Spa(A_NEXTPOS, nextId, nextPos + diff); // end next
        Spa(A_DISTALONGCURVE, nextId, totalDist);
        Spa(A_DISTTONEXT, nextId, 0.0);
        Spa(A_TONEXT, nextId, diff );
        return;
    }

     // more segments
    push(distances, distanceSum);
    //                       0 1 2  iteration
    // from 1 ... Npt-2    s . . . e
    for(int i=1; i < nofPoints-1; i++) {
        isFirst = (iteration == 0);
        isLast = (i == nofPoints-2);

        prevId = pointIds[i-1];
        curId = pointIds[i];
        nextId = pointIds[i+1];
        
        curPos = point(geoself(),"P", curId);
        prevPos = point(geoself(),"P", prevId);
        nextPos = point(geoself(),"P", nextId);
       
       
        if (isFirst) { // set info on point #0
            Spa(A_RANKI, prevId, -1); // signal line start  
            Spa(A_RANKN, prevId, 0.0 );
            diff =  prevPos - curPos;
            Spa(A_PREVPOS, prevId, prevPos + diff);
            Spa(A_NEXTPOS, prevId, curPos);
            Spa(A_DISTALONGCURVE, prevId, totalDist);
            Spa(A_DISTTONEXT, prevId, length(diff));
            Spa(A_TONEXT, prevId, curPos - prevPos );
        }
       
        diff = (curPos - prevPos);
        diffLen = length(diff);
        toNext = nextPos - curPos;
        distanceSum += diffLen;
        push(distances, distanceSum);
        totalDist += diffLen;

        Spa(A_RANKI, curId, rank);
        Spa(A_PREVPOS, curId, prevPos);
        Spa(A_NEXTPOS, curId, nextPos);
        Spa(A_DISTALONGCURVE, curId, totalDist);
        Spa(A_DISTTONEXT, curId, length(toNext));
        Spa(A_TONEXT, curId, toNext );

        //printf("iter:%d i:%d prev:%d curr:%d next:%d %s%s\n", iteration, i, prevId, curId, nextId, ( isFirst ? "First" : ""), ( isLast ? "Last" : "") );

       

        if (isLast) {
            
            Spa(A_RANKI, nextId, -2); // signal line end
            Spa(A_RANKN, nextId, 1.0 );
            Spa(A_PREVPOS, nextId, curPos);
            diff = nextPos - curPos;
            diffLen = length(diff);
            distanceSum += diffLen;
            totalDist += diffLen;
            push(distances, distanceSum);
            Spa(A_NEXTPOS, nextId, nextPos + diff);
            Spa(A_DISTALONGCURVE, nextId, totalDist);
            Spa(A_DISTTONEXT, nextId, 0.0);
            Spa(A_TONEXT, nextId, diff );

        }

        iteration++;    
        rank++;
    }
    // Apply rankN distance based;
    for(int i=1; i < nofPoints-1; i++) {
        Spa(A_RANKN,  pointIds[i], distances[i] / distanceSum);
    }
}


float SetupAdjacency() {
    printf("Generating adjacency\n");
    float totalDist = 0.0;
    int nofPrims = nprimitives(geoself());
    for (int i=0; i<nofPrims; i++ ) {
        this->SetupSinglePrimForAdjacency(i, totalDist);
    }
    printf("totalDistance along curve:%d\n", totalDist);
    return totalDist;
   

}

void NormalizeDalc(float totalDistance) {
    printf("Normalize dalc\n");
 // normalize dalc
    int nofPts = npoints(geoself());
    for (int i=0;i<nofPts; i++) {
        float dalcT = Gpa(A_DISTALONGCURVE,i,0.);
        float dalc = dalcT / totalDistance;
        Spa(A_DALC, i, dalc);
    }
    printf("Dalc normalized\n");
}





//    #########               
//    ####   ####             
//    #########               
//    ####   ####             
//    ####   ####             
//    ####   ####


void TrashTargets(int pointId; vector curPos; int  shortAmount,  longAmount ) {
     
    int tId, nofCurrent, ofs, idx;
    vector tPos;
    float dist;
    int ids[];
    int sortedIds[];
    int theSplit[];
    ids =GetIntArrayAttrib(A_TARGETIDS, pointId);
    int nofIds = len(ids);
    if (nofIds <= 1) return;
    
    float distances[] = {};
    resize(distances, nofIds);
    for (int i=0; i<nofIds; i++) {
        tId = ids[i];
        tPos = point(geoself(), "P", tId);
        dist = length(tPos - curPos);
        distances[i] = dist;
    }

    sortedIds = argsort(distances);

    if (shortAmount > 0) {
        nofCurrent = len(sortedIds);
        ofs = (shortAmount >= nofCurrent-1) ? nofCurrent-1 : shortAmount;
        theSplit = sortedIds[ofs:];
        sortedIds = theSplit;
    }

    if (longAmount > 0) {
        nofCurrent = len(sortedIds);
        ofs = (longAmount >= nofCurrent-1) ? nofCurrent-1 : longAmount;
        theSplit = sortedIds[:-ofs];
        sortedIds = theSplit;
    }

    nofCurrent = len(sortedIds);
    int resultIds[] = {};
    for(int i=0; i < nofCurrent; i++) {
        idx = sortedIds[i];
        tId = ids[idx];
        push(resultIds, tId);
    }

    SetIntArrayAttrib(A_TARGETIDS, pointId, resultIds);
    
}


}; //struct end
 