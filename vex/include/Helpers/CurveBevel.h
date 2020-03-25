#include "Helpers/pointAttribHelper.h"
#include "Helpers/primAttribHelper.h"
#include "Helpers/CurveBevelHelper.h"
#include "Helpers/polyLineHelper.h"

#define Verbose 0

struct CurveBevel {

int SourceInput, ApplyTwice, RemoveCorner, Limited, Iteration, WasNewPoint, MaxIterations, DoFirst, DoLast, AddBevelDir;
float DotThreshold, BevelScale, NeutralZone, BevelExp, CornerOffset, CurrentDotTh, IterationScale;


vector originalPos[] = {};
vector beveledPos[] = {};
vector directions[] = {};

void Init(int sourceInput; float dotThreshold, bevelScale, neutralZone, bevelExp, cornerOffset, iterationScale; int removeCorner, limited, maxIterations, doFirst, doLast, addBeveldir){
    
    DotThreshold = dotThreshold;
    BevelScale = bevelScale;
    RemoveCorner = removeCorner;
    NeutralZone = neutralZone;
    BevelExp = bevelExp;
    Limited = limited;
    CornerOffset = cornerOffset;
    SourceInput = sourceInput;
    IterationScale = iterationScale;
    MaxIterations = max(1,maxIterations);
    DoFirst = doFirst;
    DoLast = doLast;
    AddBevelDir = addBeveldir;

    originalPos = GetPositionsAsArray(SourceInput);
    CurrentDotTh = DotThreshold;
    if (Verbose) printf("%d points from source:%d ", len(originalPos), SourceInput);
}

void NextThreshold(){
    if (DotThreshold >= 1) return;
    float maxT = -1;
    CurrentDotTh = lerp(CurrentDotTh, maxT, IterationScale);
    if (Verbose) printf("nextT:%d\n", CurrentDotTh);
}

void RunOnce() {

   resize(directions,0); // clear
   beveledPos = CalculateCurveBevel(originalPos, CurrentDotTh, BevelScale, NeutralZone, BevelExp, CornerOffset, RemoveCorner, Limited,DoFirst, DoLast, directions);
   Iteration++;
   if (Verbose) printf("-> %d beveled ", len(beveledPos));
   int nofNewPts = len(beveledPos) - len(originalPos);
   if (Verbose) printf(" %d new pts\n", nofNewPts);
   WasNewPoint = (nofNewPts > 0);
   this->NextThreshold();
}

void Draw(){
    int resultIds[] ={};
    int freshPrimId = DrawPolyLine(this.beveledPos, {1, 1, 0.4}, resultIds);

    if (!AddBevelDir) return;

    if (len(directions) != len(beveledPos)) {
        printf("Wtf? array size mismatch dirs vs beveles. no dirs will be applied.\n");
        printf("pts:%d dirs:%d new primId:%d  nofResIds:%d\n", len(beveledPos), len(directions), freshPrimId, len(resultIds));
        return;
    }

    int nofDirs = len(directions);
    
    for(int i=0; i < nofDirs; i++ ){
        vector d = directions[i];
        int ptId = resultIds[i];
        Spa("beveldir",ptId,  d);
    }
}

void Run() {

    while (Iteration < MaxIterations) {
        
        this->RunOnce();
        if (!WasNewPoint) {
            //printf("no new points after %d iterations\n", Iteration);
            break;
        }
        originalPos = beveledPos;
    }

    this->Draw();
}


//struct end
}