#include "Helpers/mathHelper.h"
#include "Helpers/stringHelper.h"
#include "Helpers/pointAttribHelper.h"
#include "Helpers/primAttribHelper.h"
#include "Helpers/arrayAttribHelper.h"
#include "Helpers/polyLineHelper.h"
#include "Helpers/intersectResult.h"

#include "DynamicFlocking/df_obstacleAvoidanceHelper.h"
#include "DynamicFlocking/df_groundHelper.h"
#include "DynamicFlocking/df_springHelper.h"


#define FORCEVALUEATTRNAME "df"
#define FORCELABELATTRNAME "lbl"
#define FORCEVALUES "forcevalues"
#define FORCELABELS "forcelabels"
#define FORCESELECTION "forceselection"
#define FORCEPRIORITIES "forcepriorities"
#define AFFECTORPOSTFIX "affectorIds"

#define AIM_FC "aim"
#define AVG_FC "avg"
#define COH_FC "coh"
#define PUSH_FC "push"
#define VIS_FC "vis"
#define ALN_FC "aln"
#define POL_FC "pol"
#define SPR_FC "spr"
#define OBS_FC "obs"
#define GND_FC "gnd"
#define PNT_FC "pnt"
#define NOI_FC "noi"
#define DRG_FC "drg"
#define JAM_FC "jam"

#define AIM_BIT 1
#define AVG_BIT 2
#define COH_BIT 4
#define PUSH_BIT 8
#define VIS_BIT 16
#define ALN_BIT 32
#define POL_BIT 64
#define SPR_BIT 128
#define OBS_BIT 256
#define GND_BIT 512
#define PNT_BIT 1024
#define NOI_BIT 2048
#define DRG_BIT 4096
#define JAM_BIT 8192

int GetForceControlBits(string forcesString) {

    int result = 0;
    foreach (string forceName; split(forcesString)){
        //printf("%s\n", forceName);

        if (match(AIM_FC, forceName)) {
            result |= AIM_BIT;
        }

        if (match(AVG_FC, forceName)) {
            result |= AVG_BIT;
        }

        if (match(COH_FC, forceName)) {
            result |= COH_BIT;
        }

        if (match(PUSH_FC, forceName)) {
            result |= PUSH_BIT;
        }

        if (match(VIS_FC, forceName)) {
            result |= VIS_BIT;
        }

        if (match(ALN_FC, forceName)) {
            result |= ALN_BIT;
        }

        if (match(POL_FC, forceName)) {
        result |= POL_BIT;
        }

        if (match(SPR_FC, forceName)) {
        result |= SPR_BIT;
        }

        if (match(OBS_FC, forceName)) {
        result |= OBS_BIT;
        }

        if (match(GND_FC, forceName)) {
        result |= GND_BIT;
        }

        if (match(PNT_FC, forceName)) {
        result |= PNT_BIT;
        }

        if (match(NOI_FC, forceName)) {
        result |= NOI_BIT;
        }

        if (match(DRG_FC, forceName)) {
        result |= DRG_BIT;
        }

        if (match(JAM_FC, forceName)) {
        result |= JAM_BIT;
        }

    }

    return result;
}




string ForceName(string fname) {
    string result;
    result = concat(FORCEVALUEATTRNAME, "_", fname);
    return result;
}

string ForceLabel(string fname) {
    string result;
    result = concat(FORCEVALUEATTRNAME, "_", fname, "_",FORCELABELATTRNAME);
    return result;
}

string AffectorName(string fname) {
    string result;
    result = concat(fname, "_", AFFECTORPOSTFIX);
    return result;
}


void ForceSubmitComponent(string componentName; int pointNumber; vector value; float priority) {
    
    string forceName = ForceName(componentName);
        
    vector forces[] = GetVectorArrayAttrib(FORCEVALUES, pointNumber);
    string labels[] = GetStringArrayAttrib(FORCELABELS, pointNumber);
    float priorities[] = GetFloatArrayAttrib(FORCEPRIORITIES, pointNumber);

    push(forces, value);
    push(labels, componentName);
    push(priorities, priority);
    
    // debug and normal usage -> df_forceName attrib
    Spa(forceName, pointNumber, value);
    
    // mind calculation
    SetVectorArrayAttrib(FORCEVALUES, pointNumber, forces);
    SetStringArrayAttrib(FORCELABELS, pointNumber, labels);
    SetFloatArrayAttrib(FORCEPRIORITIES, pointNumber, priorities);

    }


void ForceSubmitComponent(string componentName; int pointNumber; vector value) {
    
    ForceSubmitComponent(componentName, pointNumber, value, 1);
}

void ForceSubmitComponentArray(string forceNames[]; vector forceVectors[]; float forcePriorities[]; int pointNumber) {

    SetVectorArrayAttrib(FORCEVALUES, pointNumber, forceVectors);
    SetStringArrayAttrib(FORCELABELS, pointNumber, forceNames);
    SetFloatArrayAttrib(FORCEPRIORITIES, pointNumber, forcePriorities);

}

void ForceCreateDebugAttribs(string forceNames[]; vector forceVectors[]; int pointNumber) {

    int nofForces = len(forceNames);

    for(int i=0; i < nofForces; i++) {
        string fName = forceNames[i];
        vector fValue = forceVectors[i];
        string attribName = ForceName(fName);
        Spa(attribName, pointNumber, fValue);
    }

}





void ForcePushComponent(string forceNames[]; vector forceVectors[]; float forcePriorities[]; string forceName; vector forceVector; float priority) {

    push(forceNames, forceName);
    push(forceVectors, forceVector);
    push(forcePriorities, priority);

}


vector ForceSumComponents(int pointNumber) {
    vector accum = {0,0,0};

    vector forces[] = GetVectorArrayAttrib(FORCEVALUES, pointNumber);
        
    vector cf;
    int nofForces = len(forces);
    
    for(int i=0; i < nofForces; i++ ) {
           cf = forces[i];
           accum += cf;
        
    }
    
    return accum;
}


vector ForcePrioritizeComponents(int pointNumber; float threshold; int gotselected[]) {
    
    int verbose = 0;
    vector accum = {0,0,0};

    vector forces[] = GetVectorArrayAttrib(FORCEVALUES, pointNumber);
    float priorities[] = GetFloatArrayAttrib(FORCEPRIORITIES, pointNumber);
    string labels[] = GetStringArrayAttrib(FORCELABELS, pointNumber);
            
    float maxPriority = -1;
    foreach(float priority; priorities) {
        if (priority > maxPriority) maxPriority = priority;
    }
    
    if (verbose) {
        printf("max priority:%d\n", maxPriority);
        }

    int nofForces = len(forces);
    float normalizedPrios[];
    for(int i=0; i < nofForces; i++ ) {
        float np = priorities[i] / maxPriority;
        push(normalizedPrios, np);
        
        if (verbose) {
            string lbl = labels[i];
            float op = priorities[i];
            printf("%s %d -> %d\n", lbl, RoundTo(op,2), RoundTo(np,2) );
            }
    }  

    
    resize(gotselected, nofForces);
    vector cForce;   
    float cPriority;
    for(int i=0; i < nofForces; i++ ) {
        cForce = forces[i];
        cPriority = normalizedPrios[i];
        
        if (cPriority < threshold) {
            if (verbose) {
                string lbl = labels[i];
                printf("%s skipped %d\n", lbl, cPriority );
                }
            continue;
            }           
              
        if (verbose){
            string lbl = labels[i];
            printf("%s choosen %d\n", lbl, cPriority );           
            }

        accum += cForce * cPriority;
        gotselected[i] = 1;
        
    }
    
    return accum;
}




vector ForceGetByName(string componentName; int pointNumber ) {
    
    vector one = {0,0,0};
        
    vector forces[] = GetVectorArrayAttrib(FORCEVALUES, pointNumber);
    string labels[] = GetStringArrayAttrib(FORCELABELS, pointNumber);
        
    int nofLabels = len(labels);
    string cf;
    for(int i=0; i < nofLabels; i++ ) {
           
        cf = labels[i];
        if (cf == componentName) {
            one = forces[i];               
            break;
        }    
    }
    
    return one;
}


void ForceClear(int pointNumber) {
    
    ClearVectorArrayAttrib(FORCEVALUES, pointNumber);
    ClearStringArrayAttrib(FORCELABELS, pointNumber);
    ClearFloatArrayAttrib(FORCEPRIORITIES, pointNumber);
}



int GetAimId(int ptn) {
    
    return GetOrDefaultPointAttribute("aimId",ptn,0);
}


vector GetRawVelocity(int ptn) {
    
    return GetOrDefaultPointAttribute("v",ptn, AXIS_X);

}

void SetRawVelocity(int ptn; vector rawVelo) {
    
    SetAndCreatePointAttribute("v", ptn, rawVelo);
}

void SetUp( int ptn; vector newUp) {
    
   SetAndCreatePointAttribute("up", ptn, normalize(newUp));
}



void SetSpeed(int ptn; float newSpeed) { 
    
    SetAndCreatePointAttribute("speed", ptn, newSpeed);
    
    }

void SetForward(int ptn; vector newForward) {
    
    newForward = normalize(newForward);

    SetAndCreatePointAttribute("forward", ptn, newForward);

    }

vector GetUp(int ptn) {
    
    return GetOrDefaultPointAttribute("up", ptn, AXIS_Y);

    }

vector GetStrictUp(int ptn) {
    
    return GetOrDefaultPointAttribute("sUp", ptn, AXIS_Y);

    }

void SetStrictUp(int ptn; vector newValue) {
    
    newValue = normalize(newValue);
    SetAndCreatePointAttribute("sUp",ptn, newValue);

    }


vector GetForward(int ptn) {
    
    return GetOrDefaultPointAttribute("forward", ptn, AXIS_X );
  
}

float GetSpeed(int ptn) {
  
    return GetOrDefaultPointAttribute("speed", ptn, 0.0);

}




matrix3 AgentRotationMatrix(int ptn) {
    
    return dihedral(AXIS_X,GetForward(ptn));
}

matrix3 AgentRotationMatrixToVector(int ptn; vector direction) {
    
    return dihedral(AXIS_X,direction);
}

vector World2AgentVector(int ptn; vector in) {
        // get rotation matrix
        return invert(AgentRotationMatrix(ptn)) * in;
    }

vector Agent2WorldVector(int ptn; vector in) {
    
        return AgentRotationMatrix(ptn) * in;
    }

vector World2CustomVector(int ptn; vector in; vector direction) {
        // get rotation matrix
        return invert(AgentRotationMatrixToVector(ptn, direction)) * in;
    }


vector CalculateAvgOfPoints() {
    
    int nofPoints;
    nofPoints = npoints(geoself());
    vector accu = {0,0,0};

    vector pointPos;

    for(int i=0; i<nofPoints; i++)
    {
        pointPos = point(geoself(), "P", i);
        accu += pointPos;
    }
    
    vector result = accu / (float)nofPoints;

    return result;
    
    }



int GetSubSteps() {
  int subSteps;
  subSteps = detail(geoself(), "subSteps",0);
  if (subSteps <=0) subSteps=1;
  return subSteps;
}



//---------------------------------------------------------------------

vector testAddAttrib(string attributeName; int pointNumber; vector defaultValue) {
    
    
    vector result ={2,2,2};
       
     int success=-1;
     result =  pointattrib(geoself(),  attributeName, pointNumber, success);    
 
    if (!success)
    {
        //success = 0  original result get overwritten with default {2,2,2} => {0,0,0}
        printf("nem sikerult!\n");
        printf("succ:%d res:%g\n",success,result);
        
        //int addpointattrib(int geohandle, string name, int defvalue, string typeinfo='')

        int addres = addpointattrib(geoself(), attributeName, defaultValue, '');
        printf("add result: %d\n", addres);        


        // int setpointattrib(int geohandle, string name, int pt, vector value, string mode="set")
        int setres = setpointattrib(geoself(), attributeName, pointNumber, defaultValue, "set");
        printf("set result: %d %s\n", setres, attributeName);

    }
   else
   {
        //success = 1
        printf   ("sikerult! %d %g \n", success,result);
       
   }
   
    return result;

}

//-----------

void SubmitAffectorPoints(string forceName; int pointNumber; int affectorIds[]) {
    string attribName = AffectorName(forceName);
    SetIntArrayAttrib(attribName, pointNumber, affectorIds);
}


