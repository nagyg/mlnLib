cvex CVexPointCloud
(
    int ptnum = 0;
    int id = -1;
    int numpt = 0;
    
    float Time = 0;
    float TimeInc = 0;
    float Frame = 0;
    float age = 0;
    float life = 0;
    
    vector v = {0,0,0};
    vector force = {0,0,0};
    vector uv = {0,0,0};
    vector N = {0,0,0};
    
    string OpInput1 = "";
    string OpInput2 = "";
    string OpInput3 = "";
    string OpInput4 = "";
    
    float pscale = 1;
    float pRatio = 1;
    
    export vector P = { 0, 0, 0 };
    export vector Cd = { 1, 1, 1 };

    export vector forward = {0,0,0};
    export vector sVel = {0,0,0};
    export float speed =0;

    export vector t_v = { 0,0,0 };

)
{

    int writeDebugAttribs = chi("../writedebugattribs");

    int oPtnum, ptCount, success;
    float oDistance, oPscale, oPRatio, realDist;
    vector oP, oV, bbMin, bbMax;
        
    getbbox(geoself(), bbMin, bbMax);
    float pcRadius = 2 * length(bbMin - bbMax);
    
    vector oldVelo = sVel;
    float oldVeloLen = length(oldVelo);

    string forceNames[];
    vector forceVectors[];
    float forcePriorities[];

    int subSteps = GetSubSteps();

    string forcesToProcess = chs("../forcestoprocess");
    int forceControlBits = GetForceControlBits(forcesToProcess);
    //printf("controlbits %d\n", forceControlBits);

    // user init comes here