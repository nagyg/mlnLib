
int CreateIntArrayAttrib(string attributeName)
{

    if (!haspointattrib(geoself(), attributeName))
    {
        int defValue[];
        addpointattrib(geoself(), attributeName, defValue, '');
        return 1;
    }

    return 0;
}

int CreateFloatArrayAttrib(string attributeName)
{

    if (!haspointattrib(geoself(), attributeName))
    {
        float defValue[];
        addpointattrib(geoself(), attributeName, defValue, '');
        return 1;
    }

    return 0;
}

int CreateStringArrayAttrib(string attributeName)
{

    if (!haspointattrib(geoself(), attributeName))
    {
        string defValue[];
        addpointattrib(geoself(), attributeName, defValue, '');
        return 1;
    }

    return 0;
}

int CreateVectorArrayAttrib(string attributeName)
{
    // just for the sanity
    return CreateFloatArrayAttrib(attributeName);
}

int[] GetIntArrayAttrib(string attributeName; int pointNumber)
{

    int result[];
    int success = -1;
    result = pointattrib(geoself(), attributeName, pointNumber, success);

    if (!success)
        CreateIntArrayAttrib(attributeName);

    return result;
}

float[] GetFloatArrayAttrib(string attributeName; int pointNumber)
{

    float result[];
    int success = -1;
    result = pointattrib(geoself(), attributeName, pointNumber, success);

    if (!success)
        CreateFloatArrayAttrib(attributeName);

    return result;
}

string[] GetStringArrayAttrib(string attributeName; int pointNumber)
{

    string result[];
    int success = -1;
    result = pointattrib(geoself(), attributeName, pointNumber, success);

    if (!success)
        CreateStringArrayAttrib(attributeName);

    return result;
}

vector[] GetVectorArrayAttrib(string attributeName; int pointNumber)
{
    float floats[] = GetFloatArrayAttrib(attributeName, pointNumber);
    vector vectors[] = unserialize(floats);
    return vectors;
}

void AddIntToArrayAttrib(string attributeName; int pointNumber; int value)
{

    int attrVal[];
    attrVal = GetIntArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AddFloatToArrayAttrib(string attributeName; int pointNumber; float value)
{

    float attrVal[];
    attrVal = GetFloatArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AddStringToArrayAttrib(string attributeName; int pointNumber; string value)
{

    string attrVal[];
    attrVal = GetStringArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AddVectorToArrayAttrib(string attributeName; int pointNumber; vector value)
{

    float floats[] = GetFloatArrayAttrib(attributeName, pointNumber);
    vector vectors[] = unserialize(floats);

    push(vectors, value);

    float newFloats[] = serialize(vectors);
    //    printf("newfloats:%d\n", len(newFloats));

    int setres = setpointattrib(geoself(), attributeName, pointNumber, newFloats, "set");
}

void AppendIntArrayAttrib(string attributeName; int pointNumber; int value[])
{
    int attrVal[];
    attrVal = GetIntArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AppendFloatArrayAttrib(string attributeName; int pointNumber; float value[])
{
    float attrVal[];
    attrVal = GetFloatArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AppendStringArrayAttrib(string attributeName; int pointNumber; string value[])
{
    string attrVal[];
    attrVal = GetStringArrayAttrib(attributeName, pointNumber);

    push(attrVal, value);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void AppendVectorArrayAttrib(string attributeName; int pointNumber; vector value[])
{
    float attrVal[];
    attrVal = GetFloatArrayAttrib(attributeName, pointNumber);

    vector attrVectors[] = unserialize(attrVal);

    push(attrVectors, value);

    attrVal = serialize(attrVectors);

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void SetIntArrayAttrib(string attributeName; int pointNumber; int attrVal[])
{

    if (!haspointattrib(geoself(), attributeName))
    {
        addpointattrib(geoself(), attributeName, attrVal, '');
        setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
        return;
    }

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void SetVectorArrayAttrib(string attributeName; int pointNumber; vector value[])
{

    float attrVal[] = serialize(value);
    if (!haspointattrib(geoself(), attributeName))
    {
        addpointattrib(geoself(), attributeName, attrVal, '');
        return;
    }

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void SetFloatArrayAttrib(string attributeName; int pointNumber; float attrVal[])
{

    if (!haspointattrib(geoself(), attributeName))
    {
        addpointattrib(geoself(), attributeName, attrVal, '');
        return;
    }

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

void SetStringArrayAttrib(string attributeName; int pointNumber; string attrVal[])
{

    if (!haspointattrib(geoself(), attributeName))
    {
        addpointattrib(geoself(), attributeName, attrVal, '');
        return;
    }

    int setres = setpointattrib(geoself(), attributeName, pointNumber, attrVal, "set");
}

int TryGetIntArrayAttrib(int inputNumber; string attributeName; int pointNumber; int attrVal[])
{

    if (!hasattrib(inputNumber, "point", attributeName))
        return 0;

    int success = -1;
    attrVal = pointattrib(inputNumber, attributeName, pointNumber, success);

    return 1;
}

int TryGetFloatArrayAttrib(int inputNumber; string attributeName; int pointNumber; float attrVal[])
{

    if (!hasattrib(inputNumber, "point", attributeName))
        return 0;

    int success = -1;
    attrVal = pointattrib(inputNumber, attributeName, pointNumber, success);

    return 1;
}

int TryGetStringArrayAttrib(int inputNumber; string attributeName; int pointNumber; string attrVal[])
{

    if (!hasattrib(inputNumber, "point", attributeName))
        return 0;

    int success = -1;
    attrVal = pointattrib(inputNumber, attributeName, pointNumber, success);

    return 1;
}

int TryGetVectorArrayAttrib(int inputNumber; string attributeName; int pointNumber; vector attrVal[])
{

    if (!hasattrib(inputNumber, "point", attributeName))
        return 0;

    int success = -1;
    float uns[];
    uns = pointattrib(inputNumber, attributeName, pointNumber, success);
    attrVal = unserialize(uns);

    return 1;
}

void ClearIntArrayAttrib(string attributeName; int pointNumber)
{

    int wasCreated = CreateIntArrayAttrib(attributeName);
    if (wasCreated)
        return;

    int emptyArray[];
    int setres = setpointattrib(geoself(), attributeName, pointNumber, emptyArray, "set");
}

void ClearFloatArrayAttrib(string attributeName; int pointNumber)
{

    int wasCreated = CreateFloatArrayAttrib(attributeName);
    if (wasCreated)
        return;

    float emptyArray[];
    int setres = setpointattrib(geoself(), attributeName, pointNumber, emptyArray, "set");
}

void ClearStringArrayAttrib(string attributeName; int pointNumber)
{

    int wasCreated = CreateStringArrayAttrib(attributeName);
    if (wasCreated)
        return;

    string emptyArray[];
    int setres = setpointattrib(geoself(), attributeName, pointNumber, emptyArray, "set");
}

void ClearVectorArrayAttrib(string attributeName; int pointNumber)
{
    ClearFloatArrayAttrib(attributeName, pointNumber);
}

vector GetBufferedVectorValue(string attributeName; int pointNumber)
{
    vector vectors[] = GetVectorArrayAttrib(attributeName, pointNumber);

    int nofV = len(vectors);
    if (nofV == 0)
        return {0, 0, 0};

    vector result = AvgValuesFromVectorArray(vectors);
    return result;
}

vector AddBufferedVectorValue(string attributeName; int pointNumber; vector value; int maxSamples)
{

    vector vectors[] = GetVectorArrayAttrib(attributeName, pointNumber);

    insert(vectors, 0, value);

    int nofV = len(vectors);
    if (nofV > maxSamples)
    {
        resize(vectors, maxSamples);
    }

    //vector result = {0,0,0};
    vector result = AvgValuesFromVectorArray(vectors);

    SetVectorArrayAttrib(attributeName, pointNumber, vectors);

    return result;
}


int StoreBufferedVectorValue(string attributeName; int pointNumber; vector value; int maxSamples)
{
    vector vectors[] = GetVectorArrayAttrib(attributeName, pointNumber);

    insert(vectors, 0, value);
  
    int nofV = len(vectors);
    if (nofV > maxSamples)
    {
        resize(vectors, maxSamples);
        nofV = maxSamples;
    }

    SetVectorArrayAttrib(attributeName, pointNumber, vectors);

    return nofV;
}



float AddBufferedFloatValue(string attributeName; int pointNumber; float value; int maxSamples)
{
    float values[] = GetFloatArrayAttrib(attributeName, pointNumber);

    insert(values, 0, value);

    int nofV = len(values);
    if (nofV > maxSamples)
    {
        resize(values, maxSamples);
    }

    float result = AvgValuesFromFloatArray(values);

    SetFloatArrayAttrib(attributeName, pointNumber, values);

    return result;
}

void ShrinkBufferedVectorValue(string attributeName; int pointNumber)
{
    vector vectors[] = GetVectorArrayAttrib(attributeName, pointNumber);

    int nofV = len(vectors);
    if (nofV == 0)
        return;

    resize(vectors, nofV - 1);

    SetVectorArrayAttrib(attributeName, pointNumber, vectors);
}

vector DegrageBufferedVectorValue(string attributeName; int pointNumber; float fade)
{

    float smallNum = 0.0001;
    vector vectors[] = GetVectorArrayAttrib(attributeName, pointNumber);

    int nofV = len(vectors);

    if (nofV == 0)
    {
        return {0, 0, 0};
    }

    vector result[];
    vector curr;
    float currLen;
    int count = 0;

    for (int i = 0; i < nofV; i++)
    {
        curr = vectors[i];
        curr *= fade;
        currLen = length(curr);
        if (currLen < smallNum)
            continue;

        result[count] = curr;
        count++;
    }

    SetVectorArrayAttrib(attributeName, pointNumber, result);
    vector resultVec = AvgValuesFromVectorArray(result);
    return resultVec;
}

void pointAAHTest()
{
    printf("yes, included. pointArrayAttribHelper\n");
}
