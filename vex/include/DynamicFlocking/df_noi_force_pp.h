
if ((forceControlBits & NOI_BIT) > 0) {

float noi_amplitudeXMin = chf("../noi_xamplitudemin");
float noi_amplitudeYMin = chf("../noi_yamplitudemin");
float noi_amplitudeZMin = chf("../noi_zamplitudemin");
float noi_amplitudeXMax = chf("../noi_xamplitudemax");
float noi_amplitudeYMax = chf("../noi_yamplitudemax");
float noi_amplitudeZMax = chf("../noi_zamplitudemax");
float noi_frequencyXMin = chf("../noi_xfrequencymin");
float noi_frequencyYMin = chf("../noi_yfrequencymin");
float noi_frequencyZMin = chf("../noi_zfrequencymin");
float noi_frequencyXMax = chf("../noi_xfrequencymax");
float noi_frequencyYMax = chf("../noi_yfrequencymax");
float noi_frequencyZMax = chf("../noi_zfrequencymax");
int noi_isAgentSpace = chi("../noi_isagentspace");
int noi_relativeVelocity = 	chi("../noi_relativevelocity");
float noi_relativeStart = 	chf("../noi_relativestart");
float noi_relativeEnd = 	chf("../noi_relativeend");
float noi_priority = 		chf("../noi_priority");


float pAmplitudeX = fit01(pRatio, noi_amplitudeXMin, noi_amplitudeXMax);
float pAmplitudeY = fit01(pRatio, noi_amplitudeYMin, noi_amplitudeYMax);
float pAmplitudeZ = fit01(pRatio, noi_amplitudeZMin, noi_amplitudeZMax);

float pFrequencyX = fit01(pRatio, noi_frequencyXMin, noi_frequencyXMax);
float pFrequencyY = fit01(pRatio, noi_frequencyYMin, noi_frequencyYMax);
float pFrequencyZ = fit01(pRatio, noi_frequencyZMin, noi_frequencyZMax);

float offsetX = (ptnum * 400) + 100;
float offsetY = (ptnum * 400) + 200;
float offsetZ = (ptnum * 400) + 300;

vector sp = set(Time,0,0);

float xNoise = NonPeriodicNoise(sp, offsetX, pFrequencyX, pAmplitudeX);
float yNoise = NonPeriodicNoise(sp, offsetY, pFrequencyY, pAmplitudeY);
float zNoise = NonPeriodicNoise(sp, offsetZ, pFrequencyZ, pAmplitudeZ);

vector newNoise = set(xNoise, yNoise, zNoise);

if (noi_isAgentSpace) {
    vector tw = Agent2WorldVector(ptnum, newNoise);
    newNoise = tw;
    }
          
if (noi_relativeVelocity) {
    float relativeFactor = FilterByAngle(forward, newNoise, noi_relativeStart, noi_relativeEnd );
    newNoise *= relativeFactor;
    }

ForcePushComponent(forceNames, forceVectors, forcePriorities, NOI_FC, newNoise, noi_priority);   
//noi enabled
}