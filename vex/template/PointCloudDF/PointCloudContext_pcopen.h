   
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
                     
        realDistance = oDistance - pscale - oPscale;
        realDistance = clamp(oDistance,0, pcRadius);
        
    // user code for each iteration comes here
 