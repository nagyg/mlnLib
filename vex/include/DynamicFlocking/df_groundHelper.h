
IntersectResult GetGroundSample(string obstacleGeo; vector sampleOrigin; vector sampleVector )
{
    
    IntersectResult iRes;
    iRes->Init(sampleOrigin, sampleVector);
    
    vector result = {0,0,0};
    vector intersectPos;
    float uu,vv;

    // test for intersection
    int primnum = intersect(obstacleGeo, sampleOrigin, sampleVector, intersectPos, uu,vv);
   
    if (primnum == -1) {
        return iRes;
    }

    iRes.wasIntersect = 1;
    iRes.primIdx = primnum;
    iRes.intersection = intersectPos;
    
    vector n = prim_normal(obstacleGeo, primnum, uu, vv);
    n = normalize(n);    

    iRes.sampledNormal = n;
    iRes.uu = uu;
    iRes.vv = vv;
    
  
    // this handles the case when we are sampling from inside the geo
    
    if (GetNormalizedDot(n,sampleVector) > 0) {
        iRes.wasIntersect =0;
        return iRes;
        }
    
    
    //fake stuff
    iRes.resultForce = n;
    return iRes;

}




vector SampleGround(string groundGeo; vector sampleOrigin, sampleVector; int sampleAmount; float jitterSize; int nofHits, debugMode; vector pos[], col[]; float alf[]) {
 
    nofHits = 0;
    IntersectResult iRes;
           
    if (sampleAmount <= 1) {
        
        iRes = GetGroundSample(groundGeo, sampleOrigin, sampleVector);
        if (debugMode) {
            iRes->SaveGroundSample( pos, col, alf);
            }        
        
        nofHits += iRes.wasIntersect;

        return iRes.sampledNormal;
    }

    vector samples[] = GetJitteredSamples(sampleOrigin, sampleVector, sampleAmount, jitterSize);
       
    vector subSample = {0,0,0};
    vector accum = { 0,0,0};
    
    for (int i=0; i< sampleAmount; i++) {
            
         subSample = samples[i];
            
        iRes = GetGroundSample(groundGeo, sampleOrigin, subSample);
        
        accum += iRes.resultForce;
        nofHits += iRes.wasIntersect;
        
        if (debugMode) {
            iRes->SaveGroundSample( pos, col, alf);
            }        
    }
    
    int div = nofHits;
    if (div == 0) div =1;
    
    accum = accum / float(div);
    
    return accum;
    
}




float DistanceToSurface(string groundGeo; vector sampleOrigin, sampleVector; int nofHits, debugMode; vector pos[], col[]; float alf[];) {
     
    nofHits = 0;
    IntersectResult iRes;       
            
    iRes = GetGroundSample(groundGeo, sampleOrigin, sampleVector);
    nofHits += iRes.wasIntersect; 
    
    float surfDist = 0;
    if (nofHits > 0) {
        surfDist = distance(sampleOrigin, iRes.intersection);        
    }   

    if (debugMode) {
        iRes->SaveSurfaceSample( pos, col, alf);
    }        
    
    return surfDist;

}


float GetGroundFactor(float pointDistance, pushStrength, pullStrength, pushClose, pushFar, pullClose, pullFar) {
    
    float relLen, relPos;
    
    if (pointDistance <= pushClose) return pushStrength;
    if (pointDistance > pushClose && pointDistance <= pushFar) {
        relLen = pushFar - pushClose;
        relPos = pointDistance - pushClose;    
        
        return  pushStrength * smooth(0,1, 1 - clamp(relPos / relLen, 0, 1));
    }

    // between pushFar and pullClose it is 0    
    if (pointDistance > pushFar && pointDistance <= pullClose) return 0;
    
    if (pointDistance > pullClose && pointDistance <= pullFar) {
        relLen = pullFar - pullClose;
        relPos = pointDistance - pullClose;
        return pullStrength * ( -1 * smooth(0,1, clamp(relPos / relLen, 0, 1) ));   
    }

    //pointDistance > pullFar
    return  -1 * pullStrength ;         

}


vector CalculateGroundForce(vector sampledNormal; float surfDistance, pushStrength, pullStrength, pushClose, pushFar, pullClose, pullFar ) {
    
        float groundFactor = GetGroundFactor(surfDistance, pushStrength, pullStrength, pushClose, pushFar, pullClose, pullFar);
        return normalize(sampledNormal) * groundFactor;
}

