cvex CVexContextPoints
(
    int ptnum = 0;
    float Time =0;
    float TimeInc =0;
    float Frame =0;
    int id=0;
    int numpt=0;
    float pRatio = 0;
    float pscale = 0;
    float speed = 0;
    vector forward = {0,0,0};
    vector heading = {0,0,0};
    vector v ={0,0,0};
    vector force ={0,0,0};
    float age =0;
    float life =0;
    vector uv = {0,0,0};
    vector N = {0,0,0};
    string OpInput1 ="";
    string OpInput2 ="";
    string OpInput3 ="";
    string OpInput4 ="";
    export vector P = { 0, 0, 0 };
    export vector Cd = { 1, 1, 1 };      
)
{
    int oPtnum, ptCount;
    float oDistance, oPscale, oPRatio, realDistance;
    vector oP;
    
    vector bbMin, bbMax;
    getbbox(geoself(), bbMin, bbMax);
    float pcRadius = 2 * length(bbMin - bbMax);
    
    // user init comes here