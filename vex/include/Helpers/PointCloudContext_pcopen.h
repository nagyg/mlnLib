   
    //open all
    int hnd = pcopen(geoself(), "P", P, pcRadius, numpt+1);

    //as generic as possible
    
    ptCount = 0;
    while (pciterate(hnd))
    {
        pcimport(hnd, "point.number", oPtnum);
        pcimport(hnd, "point.distance", oDistance);    
        pcimport(hnd, "P", oP);
        pcimport(hnd, "pscale", oPscale);
        pcimport(hnd, "pRatio", oPRatio);
        pcimport(hnd, "sVel", oV);  

        realDist = oDistance - pscale - oPscale;
        realDist = clamp(realDist,0, pcRadius);

        // *** your code for each iteration comes here
 