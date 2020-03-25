
if ((forceControlBits & DRG_BIT) > 0) {

float drg_dragStr = 		chf("../drg_dragstr");
float drg_dragExponent = 	chf("../drg_dragexponent");
float drg_effectMin = 		chf("../drg_effectmin");
float drg_effectMax = 		chf("../drg_effectmax");
float drg_priority = 		chf("../drg_priority");

float speedLen = length(oldVelo);
float pMult = fit01(pRatio, drg_effectMin, drg_effectMax);
float df = pow(speedLen, drg_dragExponent) * drg_dragStr * pMult;
vector dragV = normalize(oldVelo) * -1 * df;

ForcePushComponent(forceNames, forceVectors, forcePriorities, DRG_FC, dragV, drg_priority);

//drag enabled
}