
float GetSpringFactor(float pointDistance, pushClose, pushFar, pullClose, pullMid, pullFar, pushForce, pullForce; int hasSpring) {
    
    float relLen, relPos;
    hasSpring = 0;
    
    if (pointDistance > pullFar) return 0;
    
    hasSpring =1;

    if (pointDistance <= pushClose) return pushForce;
    if (pointDistance > pushClose && pointDistance < pushFar)
    {
        relLen = pushFar - pushClose;
        relPos = pointDistance - pushClose;
        return  pushForce * smooth(0,1, 1 - clamp(relPos / relLen, 0, 1));
        
    }
    
    // between pushFar and pullClose it is 0    
    if (pointDistance >= pushFar && pointDistance <= pullClose) return 0;

    if (pointDistance > pullClose && pointDistance < pullMid) {
        relLen = pullMid - pullClose;
        relPos = pointDistance - pullClose;
        return pullForce * ( -1 * smooth(0,1, clamp(relPos / relLen, 0, 1) ) );
               
        }
        

    if (pointDistance >= pullMid && pointDistance <= pullFar) {
        
        relLen = pullFar - pullMid;
        relPos = pointDistance - pullMid;
        return  pullForce * (-1* smooth(0,1,(1 - clamp(relPos / relLen, 0, 1))));        
        
        }

    return 0;
    }


float CalculateSpringForce(float pointDistance, pushClose, pushFar, pullClose, pullMid, pullFar, pushForce, pullForce; int debugMode; vector pointA, pointB; vector pos[], col[]; float alf[]) {
    
    int hasSpring = 0;
    float rawSpring = GetSpringFactor(pointDistance, pushClose, pushFar, pullClose, pullMid, pullFar, pushForce, pullForce, hasSpring);
    
    if (debugMode) {
        
        int isPull = 0;
        float ratio = 0.0;
        if (rawSpring < 0) {
            // pull case
            
            ratio = clamp( abs(rawSpring) / pullForce, 0,1 );
            isPull = 1;            

            } else {
            //push case
                
            ratio = clamp( rawSpring / pushForce , 0,1 );
                }


        vector kolor = { 0,0,0};
        if (hasSpring) {
            
            // has force            
            
            kolor.b = 0.1;

            if (isPull) {
                kolor.r = ratio;                
                kolor.g = 0.0;
                } else {
                 kolor.r = 0.0;
                 kolor.g = ratio;   
                }

            push(pos, pointA);
            push(col, kolor);
            push(alf, 0.3);
            
            push(pos, pointB);
            push(col, {-1,0,0});
            push(alf, 1);
        }                
    }

    return rawSpring;

    }

