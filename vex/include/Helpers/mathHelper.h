#define AXIS_X {1,0,0}
#define AXIS_Y {0,1,0}
#define AXIS_Z {0,0,1}
#define NULLV {0,0,0}
#define kX 0
#define kY 1
#define kZ 2

 

int CheckNanOrInf(float value; int ptinfo) {
        
/*
int isfinite(float x)
Returns 1 if the given value is a normal, finite, number.
Returns 0 if it is a NAN or infinite. Note that VEX generally will not produce these: 3/0 is defined as 0, for example.

isnan
Returns 1 if the given value is not a number.
Returns 0 if it is a normal number or infinite.
*/

    int result = 0;
    if (isnan(value) == 1) {
        printf("pt:%d NAN\n",ptinfo);
        result=1;
    }
    
    if (isfinite(value) == 0){
        printf("pt:%d infinite\n",ptinfo);
        result=1;
    }

    return result;
}



float RoundTo(float in; int nofDigits) {
    
    float tempF = in * pow(10,nofDigits);
    tempF = rint(tempF);
    return tempF / pow(10,nofDigits);
    
    }


float GetRadiansBetween(vector a, b) {

    return acos(clamp(dot(normalize(a),normalize(b)) , -1,1 ));
}




// -1 ... 1  180deg ... 0deg
float GetNormalizedDot(vector vecA, vecB) {
    return dot(normalize(vecA), normalize(vecB));
    }

// 0 ... 1  90deg ... 0deg
float Get01Dot(vector vecA, vecB) {
    return clamp( GetNormalizedDot(vecA, vecB),0,1 );
    }
    
// 0 ... 1 180 .... 0deg
float GetFullDot(vector vecA, vecB) {
    return  fit( GetNormalizedDot(vecA, vecB),-1,1, 0,1);
    }

// start-end: -1 ... 1
float DotRatio(vector vecA, vecB; float start, end) {
    return  fit( clamp( GetNormalizedDot(vecA, vecB), start, end ), start, end, 0,1);
    }

float AngleRatio(vector vecA, vecB; float start, end) {
    
    // 0 ... 3.14 linear
    float rads = GetRadiansBetween(vecA,vecB);
    float fitt = fit(rads, 0,M_PI, 1,-1 );
    return  smooth(0,1, fit( clamp( fitt, start, end ), start, end, 0,1));
    
}

float AngleRatio(vector vecA, vecB) {
    return AngleRatio(vecA, vecB, -1 , 1);
    }

float FilterByAngle(vector vecA, vecB; float start, end) {
    return 1 - AngleRatio(vecA, vecB, start, end);
    }


vector AvgValuesFromVectorArray(vector vectors[]) {
    
    int count = 0;
    int nofV = len(vectors);
    vector accum, curr;
    accum = {0,0,0};
    for (int i=0; i<nofV; i++ ) {
         curr = vectors[i];
         if (curr == {0,0,0}) continue;
         accum += curr; 
         count ++;      
        }
        
    if (count == 0) count=1;

    vector result = accum / count;
    
    return result;

    }


float AvgValuesFromFloatArray(float values[]) {
    
    int count = 0;
    int nofV = len(values);
    float accum, curr;
    accum = 0;
    for (int i=0; i<nofV; i++ ) {
         curr = values[i];
         if (curr == 0) continue;
         accum += curr; 
         count ++;      
        }
        
    if (count == 0) count=1;

    float result = accum / count;
    
    return result;

    }


// close..far -> 0..1
float BiasedSmooth(float pos, close, far, bias) {
    float res;
    res = smooth(0,1, clamp(fit(pos, close, far,0,1) ,0,1), bias);
    return res;
    }




float SnapToSubStep(float currentTime; int sourceSubSteps; int fps) {
    
    float inc = (1.0 / float(sourceSubSteps)) / float(fps);
        
    int multLow = floor(currentTime / inc);
    int multHigh = multLow + 1;
    
    float timeLow = floor(1000* multLow * inc) / 1000.0;
    float timeHigh = floor(1000* multHigh * inc) / 1000.0;

    float diffLow = floor(1000 *  currentTime - 1000* timeLow) / 1000.0;
    float diffHigh = floor(1000 * timeHigh -  1000* currentTime) / 1000.0;
    

    float result = timeHigh;

    if (diffLow < diffHigh) {
        result = timeLow;
        }
/*
    printf("t:%d\t\tinc:%d\t\tmLow:%d\t\tmHigh:%d\t\ttLow:%d\t\ttHigh:%d\t\tres:%d\n",
    currentTime, inc, multLow, multHigh, timeLow, timeHigh, result
    );
*/
    return result;

    }

vector[] GetJitteredSamples(vector sampleOrigin, sampleVector; int sampleAmount; float jitterSize) {
    
    vector result[];
    matrix3 xToDir = dihedral(AXIS_X, normalize(sampleVector));
    
    float effectRadius = length(sampleVector);
    float seed = sampleOrigin.x + sampleOrigin.y + sampleOrigin.z + sampleVector.x + sampleVector.y + sampleVector.z + 0.3137;
    vector subSample = AXIS_X;
    float randValue = 0;
    
    for (int i=0; i< sampleAmount; i++) {
            
        subSample = AXIS_X;
        
        randValue = ((rand(seed) * 2) -1) * jitterSize;
        seed += 0.1123;
        
        subSample.y = randValue;
        
        randValue = ((rand(seed) * 2) -1) * jitterSize;
        seed += 0.7129;
        subSample.z = randValue;

        subSample *= xToDir;
        subSample = normalize(subSample) * effectRadius;
        
        push(result, subSample);
    }

    return result;    
}


float NonPeriodicNoise(vector in; float offset, frequency, amplitude) {
   
    vector mi = set( offset + (in.x * frequency), 0, 0);
    return amplitude * (noise(mi) -0.5);
}

int MakeIntPair(int i,j) {
    int upper = j * int( pow(2,16));
    int result = i | upper;
    return result;
}

int IsPairInArray(int a,b, arrayV[]) {
    
    int pair1 = MakeIntPair(a,b);
    int pair2 = MakeIntPair(b,a);

    foreach(int val; arrayV) {
        if (val == pair1 || val == pair2) return 1;
    }
    
    return 0;
}
 

int IsIntInArray(int a; int arrayV[]) {

    return find(arrayV, a) >= 0;
}

int IsStringInArray(string s; string arrayV[] ) {

    return find(arrayV, s) >= 0;
}


float SmoothWeightByMidPoint(float pos, start, end, midPoint) {
    
    pos = clamp(pos,start, end);


    if (pos < midPoint){
        
        return 1 - BiasedSmooth(pos, start, midPoint, 1);   
    }  
    
    return BiasedSmooth(pos, midPoint, end, 1);   
   
}

float LinearWeightByMidPoint(float pos, start, end, midPoint) {
    
    pos = clamp(pos,start, end);

    if (pos < midPoint){
        
        return fit(pos, start, midPoint, 1,0);
    }
    
    return fit(pos, midPoint, end , 0,1);

}

float ExponentialWeightByMidPoint(float pos, start, end, midPoint, exponent) {
 
    pos = clamp(pos,start, end);

    if (pos < midPoint){
        
        return pow(fit(pos, start, midPoint, 1,0), exponent);
    }
    
    return pow(fit(pos, midPoint, end , 0,1), exponent);

   
}



float BiasedHyp(float inputVal, valueLeft, valueRight, biasX, biasY, exponentLeft, exponentRight ; int mirrorXLeft, mirrorYLeft, mirrorXRight, mirrorYRight ) {

    // printf("inVal:%d\texpL:%d\texpR:%d\tbiasX:%d\tbiasY:%d\tvalL:%d\tvalR:%d\n",inputVal, exponentLeft, exponentRight, biasX, biasY, valueLeft, valueRight);

    float result = 0;
    int wasMirror = 0;
    float expFinal = 0;
    float hyp = 0;
    float offs = 0;

    if (inputVal < biasX) {
        // this side | biasX
        expFinal = exponentLeft;
        if (mirrorYLeft)  expFinal = 1.0 / exponentLeft;

        hyp = inputVal;
        if (mirrorXLeft==0) {
        	hyp =  pow( (inputVal/biasX),   expFinal) ;
        } else {
            hyp =  pow( 1- (inputVal/biasX),  expFinal) ;
            wasMirror ^= 1;
        }
        
        offs = hyp;
		offs *= (biasY -valueLeft); // ySize

        if (wasMirror ==0) {
        	offs += valueLeft; // default
        } else {
            offs *= -1; // mirrored
        	offs += biasY;
        }
        result =   offs;
    } else {
        //  bias | this side

        expFinal = exponentRight;
        if (mirrorYRight) expFinal = 1.0 / exponentRight;
        
        hyp = inputVal;
        if (mirrorXRight==0) {
            hyp =  pow( (inputVal - biasX)/(1-biasX),  expFinal) ;
        } else {
            hyp =  pow(1 - (inputVal - biasX)/(1-biasX),  expFinal) ;
            wasMirror ^= 1;         
        }

        offs = hyp;
        offs *= (biasY -valueRight); // ySize

        if (wasMirror ==0) {
            // default
            offs *= -1; 
            offs += biasY;
        } else {
            // mirrored
            offs += valueRight; 
            
        }


        result = offs  ;
        
    }
    
    return result;

}


float CalculateNormalizedTime(float currentFrame; int startFrame; int endFrame) {

	// endframe is inclusive => endframe substeps will be beyond 1.0 => +1
	float res;
	res= fit(currentFrame, startFrame, endFrame+1, 0.0, 1.0);
	return res;
}

// $RFSTART ... $RFEND  => 0 ... 1
float GetNormalizedTime() {
    int succ;
    float result = detailattrib(geoself(),"nTime",0,succ);
    
    if (!succ) {
        printf("No nTime detail attrib can be found!\n");
        }
    
    return result;

    }


void GetAxisInfo(vector p1, p2, p3, p4; float diffs[]; int order[]; float averages[])
{
    float xComps[]={};
    push(xComps,p1.x);
    push(xComps,p2.x);
    push(xComps,p3.x);
    push(xComps,p4.x);

    float yComps[]={};
    push(yComps,p1.y);
    push(yComps,p2.y);
    push(yComps,p3.y);
    push(yComps,p4.y);

    float zComps[]={};
    push(zComps,p1.z);
    push(zComps,p2.z);
    push(zComps,p3.z);
    push(zComps,p4.z);

    xComps = sort(xComps);
    yComps = sort(yComps);
    zComps = sort(zComps);

    float xDiff = abs(xComps[0] - xComps[len(xComps)-1]);
    float yDiff = abs(yComps[0] - yComps[len(yComps)-1]);
    float zDiff = abs(zComps[0] - zComps[len(zComps)-1]);

    push(diffs,xDiff);
    push(diffs,yDiff);
    push(diffs,zDiff);

    order = argsort(diffs);
    push(averages, avg(xComps));
    push(averages, avg(yComps));
    push(averages, avg(zComps));

    //printf("\n\n\n\n\n\n\n\nin:\n");
    //printf("%d %d %d %d\n",p1,p2,p3,p4 );
    //printf("diffs:%d\n", diffs);
    //printf("order:%d\n", order);

}


int GetMaxAxis(vector p1, p2, p3, p4)
{
    float diffs[] = {};
    int order[] ={};
    float averages[] = {};
    GetAxisInfo(p1,p2,p3,p4,diffs,order,averages);

    if (order[2] == kX) return kX;
    if (order[2] == kY) return kY;
    return kZ;
}


int GetMinAxis(vector p1, p2, p3, p4)
{
    float diffs[] = {};
    int order[] ={};
    float averages[] = {};
    GetAxisInfo(p1,p2,p3,p4,diffs,order,averages);

    if (order[0] == kX) return kX;
    if (order[0] == kY) return kY;
    return kZ;
}


void GetMinAxisInfo(vector p1, p2, p3, p4; int minAxis; float average)
{
    float diffs[] = {};
    int order[] ={};
    float averages[] = {};
    GetAxisInfo(p1,p2,p3,p4,diffs,order,averages);

    if (order[0] == kX) {
        minAxis = kX; 
        average = averages[kX];
    } else if (order[0] == kY) {
        minAxis = kY;
        average = averages[kY];
    } else {
        minAxis = kZ;
        average = averages[kZ];
    }
}



int IntersectLineSegments( vector p0, p1, p2, p3, intersectPoint ) {

    float s10_x = p1[0] - p0[0];
    float s10_y = p1[1] - p0[1];
    float s32_x = p3[0] - p2[0];
    float s32_y = p3[1] - p2[1];

    float denom = s10_x * s32_y - s32_x * s10_y;

    if (denom == 0) return 0; 

    int denom_is_positive = denom > 0;

    float s02_x = p0[0] - p2[0];
    float s02_y = p0[1] - p2[1];

    float s_numer = s10_x * s02_y - s10_y * s02_x;

    if ((s_numer < 0) == denom_is_positive) { 
        return 0; // no collision 
    }

    float t_numer = s32_x * s02_y - s32_y * s02_x;

    if ((t_numer < 0) == denom_is_positive) {
        return 0; // no collision
    }

    if ( ((s_numer > denom) == denom_is_positive) || ((t_numer > denom) == denom_is_positive) ) {
        return 0; // no collision
    }


    //  collision detected

    float t = t_numer / denom;

    intersectPoint = set( p0[0] + (t * s10_x), p0[1] + (t * s10_y)  ,0);

    return 1;
} 

int IntersectLineSegmentsMinimalAxis( vector p1, p2, p3, p4, intersection ) {
     
    vector s1,e1,s2,e2, ip;
    int res, minAxis;
    float compAvg;

    GetMinAxisInfo(p1,p2,p3,p4, minAxis, compAvg);

    if (minAxis == kX) { //x

        //printf("x axis\n");

        s1 = set(p1.y, p1.z,0);
        e1 = set(p2.y, p2.z,0);
        
        s2 = set(p3.y, p3.z,0);
        e2 = set(p4.y, p4.z,0);


    } else if (minAxis == kY) { //y
        
        //printf("y axis\n");

        s1 = set(p1.x, p1.z,0);
        e1 = set(p2.x, p2.z,0);
        
        s2 = set(p3.x, p3.z,0);
        e2 = set(p4.x, p4.z,0);

    } else { //z
        
        //printf("z axis\n");

        s1 = set(p1.x, p1.y,0);
        e1 = set(p2.x, p2.y,0);
        
        s2 = set(p3.x, p3.y,0);
        e2 = set(p4.x, p4.y,0);
    }
 
    res = IntersectLineSegments(  s1,e1,s2,e2, ip ) ;
    if (!res) return 0;

    if (minAxis == kX) { //x
   
        intersection = set(compAvg,ip.x,ip.y);

    } else if (minAxis == kY) { //y

        intersection = set(ip.x,compAvg,ip.y);

    } else { //z
         intersection = set(ip.x,ip.y,compAvg);
    }

    return 1;
}

int GetNearestPointOnSameGeo(vector p; float radius; vector nearest) {

    int nearps[];
    nearps =nearpoints(geoself(), p, radius, 2);
    if (len(nearps) >1) {
        nearest = point(geoself(),"P",nearps[1]);
        return 1;
    }

    return 0;
}

int GetNearPoints(int pointId, nofPoints; float radius; int nearIds[]){

    vector p = point(geoself(),"P",pointId);
    int nearpts[];
    nearpts =  nearpoints(geoself(), p, radius, nofPoints+1);
    if (len(nearpts) >1) {
        nearIds = nearpts[1:];
        return 1;
    }
    return 0;
}

int RndValueFromArray(int arr[]) {

    int nofVals = len(arr);
    if(nofVals == 0) return 0;
    if(nofVals == 1) return arr[0];
    float rndVal =nrandom("qstrat");
    int rndIndex =int(floor(nofVals * rndVal));
    return arr[rndIndex];
} 

int RndValueFromArray(int arr[]; float seed) {

    int nofVals = len(arr);
    if(nofVals == 0) return 0;
    if(nofVals == 1) return arr[0];
    float rndVal =rand(seed);
    int rndIndex =int(floor(nofVals * rndVal));
    return arr[rndIndex];
} 

int RndIntInclusive(int inclusiveUpper; float seed) {
    
    int res = int(floor(random(seed) * (inclusiveUpper+1) ));
    return res;
}

int RndIntInclusive(int inclusiveUpper) {
    int res = int(floor(nrandom("qstrat") * (inclusiveUpper+1)));
    return res;
}

int IsTheSame(vector a, b) {
    float l = length(a-b);
    return l <= M_TOLERANCE;
}

vector RelativeAdd(vector source, velocity; float mult) {

    vector nS = normalize(source);
    float dV = dot(nS, normalize(velocity));
    dV = fit(dV, -1.0, 0.0, 1.0, 0.0);
    return source * dV * mult;
}


vector4 SummonOrient(vector v_normal, v_direction){
        
    matrix3 m = maketransform( v_direction, v_normal );
    vector4 q = quaternion(m);
    return q;
}


float RelativeLuminance(float r,   g,   b){
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
}

float RelLuma(vector rgb){
    return RelativeLuminance(rgb.r,rgb.g,rgb.b);
}
