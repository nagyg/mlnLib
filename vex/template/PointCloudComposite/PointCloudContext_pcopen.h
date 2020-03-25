   
    //open all
    int hnd = pcopen(geoself(), "P", P, pcRadius, numpt+1);

    //as generic as possible
    
    ptCount = 0;
    while (pciterate(hnd))
    {
        pcimport(hnd, "point.number", oPtnum);
        pcimport(hnd, "point.distance", oDistance);    
        pcimport(hnd, "P", oP);
    
        // *** your code for each iteration comes here
 