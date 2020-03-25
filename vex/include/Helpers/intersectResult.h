
struct IntersectResult {
    
    vector sampleOrigin, sampleVector, intersection, sampledNormal, inside, resultForce;
    float uu, vv;
    int wasIntersect, primIdx;
    
void Init(vector sampleO, sampleV ) {
    
    wasIntersect = 0;
    sampleOrigin = sampleO;
    sampleVector = sampleV;    
    resultForce = {0,0,0};
    intersection = {0,0,0};
    sampledNormal = {0,0,0};
    inside = {0,0,0};
    uu = 0;
    vv = 0;
    primIdx = -1;
    
    }

float GeoDistance() {
    
    if (!wasIntersect) {
        return -1;   
    }
    vector gd = sampleOrigin - intersection;
    return length(gd);
    }

        
void SaveObstacleSample( vector pos[], col[]; float alf[]) {
      
    if (wasIntersect) {
        push(pos, sampleOrigin);
        push(col, {0.1,0.9,0.3});
        push(alf, 1);
        
        push(pos, intersection);
        push(col, {0.2,0.3,0.6});
        push(alf, 1);
        
        push(pos, sampleOrigin+sampleVector);
        push(col, {-1,0,0});
        push(alf, 1);
            
        push(pos, intersection);
        push(col, {0.95,0.2,0.6});
        push(alf, 1);

        push(pos, intersection+resultForce);
        push(col, {-1,0,0});
        push(alf, 1);
        
        } else {
            
        push(pos, sampleOrigin);
        push(col, {0.2,0.2,0.2});
        push(alf, 0);            
            
        push(pos, sampleOrigin+sampleVector);
        push(col, {-1,0,0});
        push(alf, 1);
        
        }
    
    }

void SaveGroundSample(vector pos[], col[]; float alf[]) {
      
    if (wasIntersect) {
        push(pos, sampleOrigin);
        push(col, {0.02,0.1,0.05});
        push(alf, 0);
        
        push(pos, intersection);
        push(col, {0.5,0.3,0.2});
        push(alf, 0);
        
        push(pos, intersection+resultForce);
        push(col, {-1,0,0});
        push(alf, 1);
        
        } else {
            
        push(pos, sampleOrigin);
        push(col, {0.3,0.5,1});
        push(alf, 0);            
            
        vector ns = normalize(sampleVector);
        push(pos, sampleOrigin +ns);
        push(col, {-1,0,0});
        push(alf, 1);
        
        }
    
    }

void SaveSurfaceSample(vector pos[], col[]; float alf[]) {
      
    if (wasIntersect) {
        push(pos, sampleOrigin);
        push(col, {0.02,1,0.05});
        push(alf, 1);
        
        push(pos, intersection);
        push(col, {0.5,0.3,0.2});
        push(alf, 0);
        
        } else {
            
        push(pos, sampleOrigin);
        push(col, {1,0.5,0.3});
        push(alf, 0);            
            
        vector ns = normalize(sampleVector);
        push(pos, sampleOrigin +ns);
        push(col, {-1,0,0});
        push(alf, 1);
        
        }
    
    }

//structend
}
