
//--- detail attrib stuff
int DetailCreateInt(string attribName ) {

    if(!hasdetailattrib(geoself(), attribName)) {
        int defValue = 0;
        adddetailattrib(geoself(), attribName, defValue, '');
        return 1;
    }
    return 0;
}

int DetailCreateFloat(string attribName ) {

    if(!hasdetailattrib(geoself(), attribName)) {
        float defValue = 0;
        adddetailattrib(geoself(), attribName, defValue, '');
        return 1;
    }
    return 0;
}

int DetailCreateString(string attribName ) {

    if(!hasdetailattrib(geoself(), attribName)) {
        string defValue = "";
        adddetailattrib(geoself(), attribName, defValue, '');
        return 1;
    }
    return 0;
}


int DetailCreateFloatArrayAttrib(string attributeName) {

    if (! hasdetailattrib(geoself(), attributeName )) {
        float defValue[];  
        adddetailattrib(geoself(), attributeName, defValue, '');
        return 1;
    }
    return 0;    
}


int DetailCreateIntArrayAttrib(string attributeName) {

    if (! hasdetailattrib(geoself(), attributeName )) {
        int defValue[];  
        adddetailattrib(geoself(), attributeName, defValue, '');
        return 1;
    }
    return 0;    
}

int DetailCreateStringArrayAttrib(string attributeName) {

    if (! hasdetailattrib(geoself(), attributeName )) {
        string defValue[];  
        adddetailattrib(geoself(), attributeName, defValue, '');
        return 1;
    }
    return 0;    
}

float[] DetailGetFloatArrayAttrib(string attributeName) {
 
    float result[];    
    int success=-1;
    result =  detailattrib(geoself(),  attributeName, 0, success);

    if (!success) DetailCreateFloatArrayAttrib(attributeName);
    return result;
}

int[] DetailGetIntArrayAttrib(string attributeName; int inputId) {
 
    int result[];    
    int success=-1;
    result =  detailattrib(inputId,  attributeName, 0, success);

    if (!success) DetailCreateIntArrayAttrib(attributeName);
    return result;
}


int[] DetailGetIntArrayAttrib(string attributeName) {
 
    return DetailGetIntArrayAttrib(attributeName, geoself());
}



string[] DetailGetStringArrayAttrib(string attributeName) {
 
    string result[];    
    int success=-1;
    result =  detailattrib(geoself(),  attributeName, 0, success);

    if (!success) DetailCreateStringArrayAttrib(attributeName);
                
    return result;

}

int DetailSetInt(string attributeName; int value) {
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, value, '');   
        setdetailattrib(geoself(), attributeName, value, "set"); 
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, value, "set");
    }
    return 0;  
}

int DetailSetFloat(string attributeName; float value) {
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, value, '');  
        setdetailattrib(geoself(), attributeName, value, "set");  
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, value, "set");
    }
    return 0;  
}

int DetailSetString(string attributeName; string value) {
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, value, '');   
        setdetailattrib(geoself(), attributeName, value, "set"); 
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, value, "set");
    }
    return 0;  
}

int DetailSetVector(string attributeName; vector value) {
    
    float compz[] = {};
    push(compz,value.x);
    push(compz,value.y);
    push(compz,value.z);

    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, compz, '');    
        setdetailattrib(geoself(), attributeName, compz, "set");
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, compz, "set");
    }
    return 0;  
}


int DetailGetInt(string attributeName) {
    int result = 0;    
    int success=-1;
    result =  detailattrib(geoself(),  attributeName, 0, success);
    if (!success) DetailCreateInt(attributeName);
    return result;
}

float DetailGetFloat(string attributeName) {
    float result = 0;    
    int success=-1;
    result =  detailattrib(geoself(),  attributeName, 0, success);
    if (!success) DetailCreateFloat( attributeName);
    return result;
}

string DetailGetString(string attributeName) {
    string result = "";    
    int success=-1;
    result =  detailattrib(geoself(),  attributeName, 0, success);
    if (!success) DetailCreateString( attributeName);
    return result;
}

vector DetailGetVector(string attributeName) {
    int success=-1;
    float fa[] =  detailattrib(geoself(),  attributeName, 0, success);
    if (!success) DetailCreateFloatArrayAttrib( attributeName);
    return set(fa[0],fa[1],fa[2]);
}



int DetailSetStringArrayAttrib(string attributeName; string stringArray[]) {
    
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, stringArray, '');  
        setdetailattrib(geoself(), attributeName, stringArray, "set");  
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, stringArray, "set");
    }
    
    return 0;   
}

int DetailSetFloatArrayAttrib(string attributeName; float floatArray[]) {
    
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, floatArray, '');  
        setdetailattrib(geoself(), attributeName, floatArray, "set");  
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, floatArray, "set");
    }
    
    return 0;
}


int DetailSetIntArrayAttrib(string attributeName; int intArray[]) {
    
    if (!hasdetailattrib(geoself(), attributeName )) {
        adddetailattrib(geoself(), attributeName, intArray, '');  
        setdetailattrib(geoself(), attributeName, intArray, "set");  
        return 1;
    } else {
        setdetailattrib(geoself(), attributeName, intArray, "set");
    }
    
    return 0;
}

int Dsi(string attributeName; int value) {
    return DetailSetInt(attributeName, value);
}

int Dsf(string attributeName; float value) {
    return DetailSetFloat(attributeName, value);
}

int Dss(string attributeName; string value) {
    return DetailSetString(attributeName, value);
}

int Dsv(string attributeName; vector value) {
    return DetailSetVector(attributeName, value);
}

int Dgi(string attributeName) {
    return DetailGetInt(attributeName);
}

float Dgf(string attributeName) {
    return DetailGetFloat(attributeName);
}

string Dgs(string attributeName) {
    return DetailGetString(attributeName);
}

vector Dgv(string attributeName) {
    return DetailGetVector(attributeName);
}


int Dssa(string attributeName; string stringArray[]) {
    return DetailSetStringArrayAttrib(attributeName, stringArray);
}

int Dsfa(string attributeName; float floatArray[]){
    return DetailSetFloatArrayAttrib(attributeName, floatArray);
}

int Dsia(string attributeName; int intArray[]) {
    return DetailSetIntArrayAttrib(attributeName, intArray);
}

int Dsva(string attributeName; vector vectorArray[]) {

    float fArray[] = serialize(vectorArray);
    return DetailSetFloatArrayAttrib(attributeName, fArray);
}

float[] Dgfa(string attributeName ) {
    return DetailGetFloatArrayAttrib(attributeName);
}

int[] Dgia(string attributeName; int inputId) {
    return DetailGetIntArrayAttrib(attributeName, inputId);
}

int[] Dgia(string attributeName) {
    return DetailGetIntArrayAttrib(attributeName);
}

vector[] Dgva(string attributeName) {
    float fz[] = DetailGetFloatArrayAttrib(attributeName);
    vector vz[] = unserialize(fz);
    return vz;
}

string[] Dgsa(string attributeName) {
    return DetailGetStringArrayAttrib(attributeName);
}
