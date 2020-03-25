

void AddSegment(vector pointA, pointB, segmentColor; int pointAIndex, pointBIndex, primitiveIndex) {
        pointAIndex = addpoint(geoself(), pointA);
        pointBIndex = addpoint(geoself(), pointB);
        
        Spa("Cd", pointAIndex, segmentColor);
        Spa("Cd", pointBIndex, segmentColor);
                
        primitiveIndex = addprim(geoself(), "polyline");
        
        addvertex(geoself(), primitiveIndex, pointAIndex);
        addvertex(geoself(), primitiveIndex, pointBIndex);
    
    }



void DrawDiagLine(vector positions[], colors[]; float alphaStart, alphaEnd; vector endColorTint) {

    
    int nofPos = len(positions);
    int nofColors = len(colors);

    if (nofColors != nofPos){
        printf("position and color array length mismatch!\n");
        return;
    }
    
    if (nofPos == 0 || nofColors == 0) return;

    int pi1,pi2;    
    if (nofPos == 1) {
        pi1 = addpoint(geoself(), positions[0]);
        Spa("Cd", pi1, colors[0]);
        Spa("Alpha", pi1, alphaStart);
        return;
        }

    int primIdx;
    vector pp1, pp2;
    vector c1, c2;
    float a1, a2;
    
    a1 = alphaStart;
    a2 = alphaEnd;

    for(int i=0; i<nofPos-1;i++ ) {

        c1 = colors[i];       
        if (c1.r < 0) continue;        

        pp1 = positions[i];
        pp2 = positions[i+1];
        
        c2 = c1;
        c2 *= endColorTint;     
        
        pi1 = addpoint(geoself(), pp1);
        pi2 = addpoint(geoself(), pp2);
        
        Spa("Cd", pi1, c1);
        Spa("Alpha", pi1, a1);
        Spa("Cd", pi2, c2);
        Spa("Alpha", pi2, a2);
        
        primIdx = addprim(geoself(), "polyline");
        
        addvertex(geoself(), primIdx, pi1);
        addvertex(geoself(), primIdx, pi2);

        }
   
    }


void DrawDiagLine(vector positions[], colors[]) {
     DrawDiagLine(positions, colors, 1, 0.5, {1, 1, 1});
    }


void DrawPolyLines(vector positions[]; vector colors[]; float alphas[]) {

    int nofPos = len(positions);
    int nofColors = len(colors);
    int nofAlphas = len(alphas);

    if (nofColors != nofPos || nofAlphas != nofPos ){
        printf("DrawPolyLines: array length mismatch!\n");
        return;
    }
    
    if (nofPos == 0 || nofColors == 0 || nofAlphas == 0) return;

    int pi1,pi2;    
    if (nofPos == 1) {
        pi1 = addpoint(geoself(), positions[0]);
        Spa("Cd", pi1, colors[0]);
        Spa("Alpha", pi1, alphas[0]);
        return;
        }

    int primIdx;
    vector pp1, pp2, c1, c2;
    float a1, a2;
    
    for(int i=0; i< nofPos - 1; i++ ) {
        
        c1 = colors[i];
        if (c1.r < 0) continue;

        pp1 = positions[i];
        pp2 = positions[i+1];
        a1 = 1.0;
        a2 = alphas[i];
        
        c2 = c1;
        c1 *= a1;
        c2 *= a2;        

        pi1 = addpoint(geoself(), pp1);
        pi2 = addpoint(geoself(), pp2);
        
        Spa("Cd", pi1, c1);
        Spa("Alpha", pi1, a1);
        Spa("Cd", pi2, c2);
        Spa("Alpha", pi2, a2);
        
        primIdx = addprim(geoself(), "polyline");
        
        addvertex(geoself(), primIdx, pi1);
        addvertex(geoself(), primIdx, pi2);

        }
   
    }


int DrawPolyLine(vector positions[]; vector lineColor; int resultIds[]) {
    
    int nofPos = len(positions);
     
    if (nofPos == 0) return 0;

    int pidx;    
    
    if (nofPos == 1) {
        pidx = addpoint(geoself(), positions[0]);
        push(resultIds, pidx);
        Spa("Cd", pidx, lineColor);
        return pidx;
        }

    int primIdx;
    vector ppos;
        
    primIdx = addprim(geoself(), "polyline");
    
    for(int i=0; i< nofPos; i++ ) {
      
        ppos = positions[i];

        pidx = addpoint(geoself(), ppos);
        push(resultIds, pidx);       
        addvertex(geoself(), primIdx, pidx);

    }
   
    SetAndCreatePrimAttribute("Cd", primIdx, lineColor);
    return primIdx;
}


int DrawPolyLine(vector positions[]; vector lineColor) {
    int resultIds[] ={};
    return DrawPolyLine(positions,  lineColor, resultIds);
}






void DrawSinglePolyLine(vector positions[]; vector colors[]; float alphas[]) {

    int nofPos = len(positions);
    int nofColors = len(colors);
    int nofAlphas = len(alphas);

    if (nofColors != nofPos || nofAlphas != nofPos ){
        printf("DrawPolyLines: array length mismatch!\n");
        return;
    }
    
    if (nofPos == 0 || nofColors == 0 || nofAlphas == 0) return;


    int primIdx, pidx;
    vector pp, c;
    float a;

    primIdx = addprim(geoself(), "polyline");
    
    for(int i=0; i< nofPos ; i++ ) {
        
        c = colors[i];
        pp = positions[i];
        a = alphas[i];

        pidx = addpoint(geoself(), pp);
        
        Spa("Cd", pidx, c);
        Spa("Alpha", pidx, a);
        
        addvertex(geoself(), primIdx, pidx);

        }
   
    }

void DrawSinglePolyLine(vector positions[]; vector colors[]; float alphas[]; float widths[]) {

    int nofPos = len(positions);
    int nofColors = len(colors);
    int nofAlphas = len(alphas);
    int nofWidths = len(widths);

    if (nofColors != nofPos || nofAlphas != nofPos || nofColors != nofWidths ){
        printf("DrawPolyLines: array length mismatch!\n");
        return;
    }
    
    if (nofPos == 0 || nofColors == 0 || nofAlphas == 0) return;


    int primIdx, pidx;
    vector pp, c;
    float a,w;

    primIdx = addprim(geoself(), "polyline");
    
    for(int i=0; i< nofPos ; i++ ) {
        
        c = colors[i];
        pp = positions[i];
        a = alphas[i];
        w = widths[i];
        pidx = addpoint(geoself(), pp);
        
        Spa("Cd", pidx, c);
        Spa("Alpha", pidx, a);
        Spa("width",pidx,w);
        
        addvertex(geoself(), primIdx, pidx);

        }
   
    }
