
float spr_forceStrength = 		chf("../spr_forcestrength");
float spr_sizePowerMin = 		chf("../spr_sizepowermin");
float spr_sizePowerMax = 		chf("../spr_sizepowermax");
int spr_limitWithDir = 			chi("../spr_limitwithdirection");
float spr_directionLimitLow = 	chf("../spr_directionlimitlow");
float spr_directionLimitHigh = 	chf("../spr_directionlimithigh");
float spr_crowdExponent = 		chf("../spr_crowdexponent");
float spr_pushMin = 			chf("../spr_pushmin");
float spr_pullMin = 			chf("../spr_pullmin");
float spr_pushMax = 			chf("../spr_pushmax");
float spr_pullMax = 			chf("../spr_pullmax");
float spr_pushCloseMin = 		chf("../spr_pushclosemin");
float spr_pushFarMin = 			chf("../spr_pushfarmin");
float spr_pullCloseMin = 		chf("../spr_pullclosemin");
float spr_pullMidMin = 			chf("../spr_pullmidmin");
float spr_pullFarMin = 			chf("../spr_pullfarmin");
float spr_pushCloseMax = 		chf("../spr_pushclosemax");
float spr_pushFarMax = 			chf("../spr_pushfarmax");
float spr_pullCloseMax = 		chf("../spr_pullclosemax");
float spr_pullMidMax = 			chf("../spr_pullmidmax");
float spr_pullFarMax = 			chf("../spr_pullfarmax");
int spr_relativeVelocity = 		chi("../spr_relativevelocity");
float spr_relativeStart = 		chf("../spr_relativestart");
float spr_relativeEnd = 		chf("../spr_relativeend");
float spr_priority = 			chf("../spr_priority");
int spr_writeAffectors = 		chi("../spr_writeaffectors");

if (spr_writeAffectors) {
ClearVectorArrayAttrib("spr_pos", ptnum);
ClearVectorArrayAttrib("spr_col", ptnum);
ClearFloatArrayAttrib("spr_alf", ptnum);
}

float spr_mPush = fit01(pRatio, spr_pushMin, spr_pushMax);
float spr_mPull = fit01(pRatio, spr_pullMin, spr_pullMax);
float spr_mPushClose = fit01(pRatio, spr_pushCloseMin, spr_pushCloseMax);
float spr_mPushFar = fit01(pRatio, spr_pushFarMin, spr_pushFarMax);
float spr_mPullClose = fit01(pRatio, spr_pullCloseMin, spr_pullCloseMax);
float spr_mPullMid = fit01(pRatio, spr_pullMidMin, spr_pullMidMax);
float spr_mPullFar = fit01(pRatio, spr_pullFarMin, spr_pullFarMax);

vector spr_pos[], spr_col[]; float spr_alf[]; //debug display
vector spr_accum = { 0,0,0 };
int spr_count =0;