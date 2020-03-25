
IntersectResult GetObstacleAvoid(string obstacleGeo; vector sampleOrigin; vector sampleVector )
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
    
    iRes.sampledNormal = n;
    iRes.uu = uu;
    iRes.vv = vv;
    
    n = normalize(n);

    // this handles the case when we are sampling from inside the geo
    if (GetNormalizedDot(n,sampleVector) > 0) {
        iRes.wasIntersect =0;
        return iRes;
        }

    // vector "inside" obstacle
    vector inside = (sampleOrigin+sampleVector) - intersectPos;
    iRes.inside = inside;          

    // project on primitive
    vector resultForce =  n * abs(dot(inside,n));
    iRes.resultForce = resultForce;
    return iRes;

}



vector SampleObstacles(string obstacleGeo; vector sampleOrigin, sampleVector; int sampleAmount; float jitterSize; int nofHits; float geoDistance; int debugMode; vector pos[], col[]; float alf[]  ) {
 
    nofHits = 0;
    IntersectResult iRes;
         
    if (sampleAmount <= 1) {
        
        iRes = GetObstacleAvoid(obstacleGeo, sampleOrigin, sampleVector);
        if (debugMode) {
            iRes->SaveObstacleSample( pos, col, alf);
            }        
        
        nofHits += iRes.wasIntersect;
        geoDistance = iRes->GeoDistance();
        return iRes.resultForce;
    }

    int nofMisses = 0;
    float gdAccum = 0;
    vector samples[] = GetJitteredSamples(sampleOrigin, sampleVector, sampleAmount, jitterSize);
      
    vector subSample = {0,0,0};
    vector hitAccum = { 0,0,0};
    vector missAccum = { 0,0,0 };
    vector originEnd = sampleOrigin + sampleVector;

    for (int i=0; i< sampleAmount; i++) {
            
        subSample = samples[i];
                   
        iRes = GetObstacleAvoid(obstacleGeo, sampleOrigin, subSample);
        
        if (iRes.wasIntersect) {
            nofHits += iRes.wasIntersect;
            //hitAccum += iRes.resultForce;
            gdAccum += iRes->GeoDistance();

            vector offs = {0,0,1};
            vector sampleEnd = iRes.intersection + iRes.resultForce;
            float lenObsForce = length(iRes.resultForce);
            vector finalForce = normalize(sampleEnd - originEnd) * lenObsForce;

            hitAccum += finalForce;

            push(pos, iRes.intersection);
            push(col, {0.5,1,0.1});
            push(alf, 0.9);
            
            push(pos, iRes.intersection + finalForce);
            push(col, {-1,0,0} );
            push(alf, 1);

        }
        else {

            nofMisses ++;     
            missAccum += subSample;
        }

        if (debugMode) {
            iRes->SaveObstacleSample( pos, col, alf);
        }        
    }


    //printf("hits:%d\tmiss:%d\n", nofHits, nofMisses);
     vector missCorrection = {0,0,0};
    if (missAccum != {0,0,0}){

            if (nofMisses == 0) nofMisses = 1;
            vector missAvg = missAccum / float(nofMisses);

            vector missAvgPos = sampleOrigin + missAvg;
            missCorrection = missAvgPos - originEnd;

            push(pos, sampleOrigin);
            push(col, {1,0.3,0.2});
            push(alf, 1);
            
            push(pos, missAvgPos);
            push(col, {0,0.1,1} );
            push(alf, 1);

            push(pos, sampleOrigin + missAvg + missCorrection);
            push(col, {-1,0,0} );
            push(alf, 1);

    }

    
    int div = nofHits;
    if (div ==0){
        div =1;
        missCorrection = {0,0,0};
        }

    geoDistance = gdAccum / div;
    hitAccum = hitAccum / float(div);

    vector result = hitAccum + missCorrection;

    return result;
    
}
