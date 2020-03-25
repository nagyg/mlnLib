

void SetAndCreatePrimAttribute(string attributeName; int primNumber; vector value) {
    if (!hasprimattrib(geoself(), attributeName)) 
    {
        addprimattrib(geoself(), attributeName, {0,0,0}, '');
    }
    
    setprimattrib(geoself(), attributeName, primNumber, value, "set");
}



void SetAndCreatePrimAttribute(string attributeName; int primNumber; int value) {
    if (!hasprimattrib(geoself(), attributeName)) 
    {
        addprimattrib(geoself(), attributeName, 0, '');
    }
    
    setprimattrib(geoself(), attributeName, primNumber, value, "set");
}


void SetAndCreatePrimAttribute(string attributeName; int primNumber; float value) {
    if (!hasprimattrib(geoself(), attributeName)) 
    {
        addprimattrib(geoself(), attributeName, 0.0, '');
    }
    
    setprimattrib(geoself(), attributeName, primNumber, value, "set");
}


