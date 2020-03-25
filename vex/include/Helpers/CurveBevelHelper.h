
// Bevels a position array
vector[] CalculateCurveBevel(vector source[]; float dotThreshold, bevelScale, neutralZone, bevelExp, cornerOffset; int removeCorner, limited, doFirst, doLast; vector directions[]) {
    int oId, prevIdx,nextIdx, isLast;
    vector curPos, prevPos, nextPos, vTo,vFrom, bL, bR, pMid, crnPos, crnDir;
    float dotVal, bDistPrev, bDistNext, bRate, vToLen, vFromLen;

    vector nullv = set(0.,0.,0.);

    //printf("%d %d\n", doFirst, doLast);
    vector resultPositions[] ={};
      
    float nZone = clamp(0.499 -neutralZone, 0., 0.499);
    int nofBp = len(source);
    for(int i=0; i < nofBp; i++) {
        
        isLast = (i == nofBp-1) ? 1 : 0;
        prevIdx = (i == 0) ? 1 : i-1;
        nextIdx = isLast ? nofBp-2 : i+1;

        prevPos = source[prevIdx]; 
        curPos = source[i];
        nextPos = source[nextIdx];

        vTo = curPos - prevPos;
        vToLen = length(vTo);
        vTo = normalize(vTo);

        vFrom = nextPos - curPos;
        vFromLen = length(vFrom);
        vFrom = normalize(vFrom);

        
        if (isLast || i ==0 ) {
            
            if (i == 0) {
                if (doFirst ) {
                    dotVal =-1;
                } else {
                    dotVal = 2; // no bevel
                }
            } else {

                if (doLast ) {
                    dotVal =-1;
                } else {
                    dotVal = 2; // no bevel
                }
                vFrom *= -1; // flip if last 
            }
            
        } else {
            dotVal = dot(vTo, vFrom);
            bRate = pow(fit(dotVal, dotThreshold, -1, 0.,1.), bevelExp);
        }
        
         

        if (limited){
            float t = min(vFromLen * nZone, vToLen * nZone );
            t = min(bevelScale, t);
            bDistPrev = t;
            bDistNext = t;
             
        } else {
            bDistPrev = lerp( vToLen *nZone * bevelScale, 0.0, bRate);
            bDistNext = lerp( vFromLen *nZone * bevelScale, 0.0, bRate);
        }

        if (dotVal < dotThreshold) {
            // high dir change, bevel this

            // bevel left
            bL = curPos - (vTo * bDistPrev);

            // bevel right
            bR = curPos + (vFrom * bDistNext);

            pMid = (bL + bR) / 2.;
            crnDir = cornerOffset * (pMid - curPos);
            crnPos = curPos + crnDir;

            // add points
            if (i > 0 || ( isLast && doLast)) {
                push(resultPositions, bL); // left bevel point
                push(directions, -1 * vTo); // points away from corner
            }

            if (  i == 0 || isLast || !removeCorner) { 
                push(resultPositions, crnPos); // orig pos - corner
                push(directions, nullv); // keep the nofPt / array len
            }
            

            if ( !isLast || (i==0 && doFirst)) {
                push(resultPositions, bR); // right bevel point
                push(directions, vFrom); // points away from corner
            }

        } else {
            // do single point
            push(resultPositions, curPos);
            push(directions, nullv);
        }
    }

    return resultPositions;
}

vector[] CalculateCurveBevel(vector source[]; float dotThreshold, bevelScale, neutralZone, bevelExp, cornerOffset; int removeCorner, limited, doFirst, doLast) {

    vector directions[] ={};
    return  CalculateCurveBevel(source, dotThreshold, bevelScale, neutralZone, bevelExp, cornerOffset, removeCorner, limited, doFirst, doLast, directions) ;
}


// reads P from source input and returns as array
vector[] GetPositionsAsArray(int sourceInput){
    int nofPts = npoints(sourceInput);
    vector pos[] = {};
    resize(pos,nofPts);

    for(int i=0; i< nofPts; i++){
        vector op = point(sourceInput,"P",i);
        pos[i] = op;
    }
    return pos;
}
