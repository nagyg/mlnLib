#include "Helpers\mathHelper.h"
#include "Helpers\pointAttribHelper.h"

//--------------- KAMU
float TehNoise(vector sampledPos){
    
    int turb = 7;
    float rough = 0.701;
    float atten = 0.817;
    float nsize = 0.052;
    vector offset = {4.6, 0, 0};
    vector sp = (sampledPos +offset) * nsize;
    return anoise(sp, turb,rough,atten);
}
//--------------- KAMU


// samples a vector and a float attrib from a pointcloud
int PcBiasedSample(vector sP; string vectorAttrName, floatAttrName; float searchRange; int maxPts; float distBias, nearFactor, 
    cloudRateLo, cloudRateHi, mixOutside; int verboseMode;
    vector resultVector; float resultFloat, resultRate)
{

    float oDistance,  biasedRate, rawBias, sampledFltVal;
    vector oP, sampledVecVal, toSp;
    
    float biasedScalar = 0;
    float sumBiasedRates = 0;
    vector biasedPos = {0,0,0};
    vector cloudGrad = {0,0,0}; // a vector that points outside of the cloud. point distribution dependent.

    float nearRange = searchRange * nearFactor;

    int handle = pcopen(1,"P", sP, searchRange, maxPts );
    int count=0;
    while (pciterate(handle))
    {  
        pcimport(handle, "point.distance", oDistance);
        pcimport(handle, "P", oP);
        pcimport(handle, vectorAttrName, sampledVecVal);
        pcimport(handle, floatAttrName, sampledFltVal);

        rawBias = BiasedSmooth(oDistance, nearRange, searchRange, distBias);
        biasedRate = 1. - rawBias;
        
        toSp = rawBias * (sP - oP);
        cloudGrad += toSp;
        
        biasedPos += sampledVecVal * biasedRate; // this will be in noise space
        biasedScalar += sampledFltVal * biasedRate;
        
        sumBiasedRates += biasedRate;
        count++;
    }
    pcclose(handle);
    if (count <= 0) return 0;

    if (sumBiasedRates > 0){
        biasedPos = biasedPos  / sumBiasedRates;
        resultFloat = biasedScalar / sumBiasedRates;

        // this one sums vectors. ideally they negate each other, so the resulting length is "small".
        // when the sampled point is outside of the cloud the vectors are starting to point more into one direction making the vector longer.
        // max length is the worst case: all the point pcopen found are pointing towards the sampled point. most outside of the cloud.
        float cloudGradMaxLen = searchRange * count;
        float cloudRate =  smooth(0.,1., length(cloudGrad) / cloudGradMaxLen  );
        cloudRate = fit(cloudRate, cloudRateLo , cloudRateHi, 0.,1.);
        resultRate = cloudRate;

        // this operation makes the resulting value independent of the number of points it sampled
        // kinda normalizes it
        cloudGrad /= float(count); 
        cloudGrad *= cloudRate * mixOutside;

        resultVector = biasedPos + cloudGrad;

        if (verboseMode) {
            int npId = addpoint(geoself(), resultVector  );
            Spa("Cd",npId,set( resultRate,resultFloat, 0.05 )); 
            Spa("ncloud",npId,  cloudGrad ); 
            Spa("gfx",npId,1); // graphics
            if (rand(sP.x) < 0.051) printf("%d ",count);
        }
    }

    return 1;
}



