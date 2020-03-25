
// SET int, float, string, vector - "detail", "point", "prim", "vertex"
// own geo

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; int value ) {
    

    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        int defaultValue = 0;
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; float value ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        float defaultValue = 0;
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; string value ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        string defaultValue = "";
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; vector value ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        vector defaultValue = {0,0,0};
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; int value[] ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        int defaultValue[];
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}


void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; float value[] ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        float defaultValue[];
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}


void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; string value[] ) {
    
    if (!hasattrib(geoself(), attribClass, attribName)) 
    {
        string defaultValue[];
        addattrib(geoself(), attribClass, attribName, defaultValue);
    }
    
    setattrib(geoself(), attribClass, attribName, elementId, vtxofprim, value, "set");
}

void SetAndCreateAttribute(string attribClass, attribName; int elementId, vtxofprim; vector value[] ) {
    
    float serFloats[] = serialize(value);
    SetAndCreateAttribute(attribClass, attribName, elementId, vtxofprim, serFloats );
}

//-------------------------------

// GET int, float, string, vector - "detail", "point", "prim", "vertex"
// own geo

int GetOrDefaultAttribute(string attribClass, attribName; int elementId; int defaultValue ) {
    
    int result = 0; //will be overwritten immediately with default if we query the attrib
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
    
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

float GetOrDefaultAttribute(string attribClass, attribName; int elementId; float defaultValue ) {
    
    float result = 0; 
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

string GetOrDefaultAttribute(string attribClass, attribName; int elementId; string defaultValue ) {
    
    string result = ""; 
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

vector GetOrDefaultAttribute(string attribClass, attribName; int elementId; vector defaultValue ) {
    
    vector result = {0,0,0}; 
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}



//--------------------------------------------------------------

// SET - Point attrib directly

void SetAndCreatePointAttribute(string attributeName; int pointNumber; vector4 value) {

    if (!haspointattrib(geoself(), attributeName))
    {
        addpointattrib(geoself(), attributeName, {0,0,0,1}, 'quaternion');
    }

    setpointattrib(geoself(), attributeName, pointNumber, value, "set");
}


void SetAndCreatePointAttribute(string attributeName; int pointNumber; vector value) {   
    
    if (!haspointattrib(geoself(), attributeName)) 
    {
        addpointattrib(geoself(), attributeName, {0,0,0}, '');
    }
    
    setpointattrib(geoself(), attributeName, pointNumber, value, "set");
}


void SetAndCreatePointAttribute(string attributeName; int pointNumber; float value) {
    
    
    if (!haspointattrib(geoself(), attributeName)) 
    {
        addpointattrib(geoself(), attributeName, 0.0, '');
    }
    
    setpointattrib(geoself(), attributeName, pointNumber, value, "set");
}

void SetAndCreatePointAttribute(string attributeName; int pointNumber; int value) {
        
    if (!haspointattrib(geoself(), attributeName)) 
    {
        addpointattrib(geoself(), attributeName, 0, '');
    }
    
    setpointattrib(geoself(), attributeName, pointNumber, value, "set");
}

void SetAndCreatePointAttribute(string attributeName; int pointNumber; string value) {
    
    if (!haspointattrib(geoself(), attributeName)) 
    {
        addpointattrib(geoself(), attributeName, "", '');
    }
    
    setpointattrib(geoself(), attributeName, pointNumber, value, "set");
}


void Spa(string attribName; int pointNumber; vector4 value) {
    SetAndCreatePointAttribute(attribName, pointNumber, value);
}

void Spa(string attributeName; int pointNumber; vector value) {
    SetAndCreatePointAttribute(attributeName, pointNumber,  value);
    }
    
void Spa(string attributeName; int pointNumber; float value ) {
    SetAndCreatePointAttribute(attributeName, pointNumber,  value);
    }

void Spa(string attributeName; int pointNumber; int value ) {
    SetAndCreatePointAttribute(attributeName, pointNumber,  value);
    }

void Spa(string attributeName; int pointNumber; string value ) {
    SetAndCreatePointAttribute(attributeName, pointNumber,  value);
    }

// -------------------------------------------

// GET - Point attrib directly


vector GetOrDefaultPointAttribute(string attributeName; int pointNumber; vector defaultValue ) {
    
    vector result ={0,0,0}; //will be overwritten immediately with default if we query the attrib
    
    int success=-1;
    result =  pointattrib(geoself(),  attributeName, pointNumber, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attributeName, pointNumber, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

float GetOrDefaultPointAttribute(string attributeName; int pointNumber; float defaultValue ) {
    
    float result =0.0;
    
    int success=-1;
    result =  pointattrib(geoself(),  attributeName, pointNumber, success);    
 
    if (!success)
    {
        SetAndCreatePointAttribute(attributeName, pointNumber, defaultValue);
        result = defaultValue;        
    }
  
    return result;    
}

int GetOrDefaultPointAttribute(string attributeName; int pointNumber; int defaultValue ) {
    
    int result =0;
    
    int success=-1;
    result =  pointattrib(geoself(),  attributeName, pointNumber, success);    
 
    if (!success)
    {
        SetAndCreatePointAttribute(attributeName, pointNumber, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

string GetOrDefaultPointAttribute(string attributeName; int pointNumber; string defaultValue ) {
    
    string result ="";
    
    int success = -1;
    result = pointattrib(geoself(),  attributeName, pointNumber, success);    
 
    if (!success)
    {
        SetAndCreatePointAttribute(attributeName, pointNumber, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}


int[] GetOrDefaultAttribute(string attribClass, attribName; int elementId; int defaultValue[] ) {
    
    int result[] = {};
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
    
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

float[] GetOrDefaultAttribute(string attribClass, attribName; int elementId; float defaultValue[] ) {
    
    float result[] = {};
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

string[] GetOrDefaultAttribute(string attribClass, attribName; int elementId; string defaultValue[]) {
    
    string result[] = {}; 
    
    int success = -1;
    result = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defaultValue);
        result = defaultValue;
    }
  
    return result;    
}

vector[] GetOrDefaultAttribute(string attribClass, attribName; int elementId; vector defaultValue[]) {
    
    vector result[] = {}; 
    
    int success = -1;
    float serFloats[] = getattrib(geoself(), attribClass, attribName, elementId, success); 
 
    if (!success)
    {
        float defFloats[];
        defFloats = serialize(defaultValue);
        SetAndCreatePointAttribute(attribClass, attribName, elementId, 0, defFloats);
        serFloats = defFloats;
    }
    
    result = unserialize(serFloats);

    return result;    
}

vector Gpa(string attributeName; int pointNumber; vector defaultValue ) {
    return GetOrDefaultPointAttribute(attributeName,pointNumber,defaultValue);
    }
    
float Gpa(string attributeName; int pointNumber; float defaultValue ) {
    return GetOrDefaultPointAttribute(attributeName,pointNumber,defaultValue);
    }
    
int Gpa(string attributeName; int pointNumber; int defaultValue ) {
    return GetOrDefaultPointAttribute(attributeName,pointNumber,defaultValue);
    }

string Gpa(string attributeName; int pointNumber; string defaultValue ) {
    return GetOrDefaultPointAttribute(attributeName,pointNumber,defaultValue);
}

int ParentPrimId(int pointNumber, inputId ) {
    int parr[] = pointprims(inputId, pointNumber);
    if (len(parr) < 1) return -1;
    return parr[0];
}

int ParentPrimId(int pointNumber) {
    return ParentPrimId( pointNumber, geoself());
}

int HasAllPointAttribs(int inputId; string attribNames[]; int aTypes[], aSizes[]; string aTypeInfos[]){
    
    string cAttr;
    int nofAttribs = len(attribNames);
    if (nofAttribs <=0) return 1; // ok
    resize(aTypes, nofAttribs);
    resize(aSizes, nofAttribs);
    resize(aTypeInfos, nofAttribs);

    string missing = "";
    int missCount = 0;
    for(int i=0; i < nofAttribs; i++) {
        cAttr = attribNames[i];
        //printf("%s\t\ttype:%d size:%d %s\n", cAttr, pointattribtype(inputId, cAttr), pointattribsize(inputId, cAttr), pointattribtypeinfo(inputId, cAttr));
        if (!haspointattrib(inputId, cAttr)) {
            missCount ++;
            missing = sprintf("%s [%s]", missing, cAttr);
            aTypes[i] = -1;
            aSizes[i] = -1;
            aTypeInfos[i] = "n/a";
        }
        aTypes[i] = pointattribtype(inputId, cAttr);
        aSizes[i] = pointattribsize(inputId, cAttr);
        aTypeInfos[i] = pointattribtypeinfo(inputId, cAttr);
    }

    if (missCount <= 0) {
        return 1; // all good
    }

    printf("input:%d missing attribs:%s\n", inputId, missing);
    return 0;
}
