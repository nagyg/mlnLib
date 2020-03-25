
string SeqNumber(float currentTime) {
    int rounded = floor(currentTime * 1000);
    return itoa(rounded);

    }
    
string PadString(string in; string padValue; int totalLen) {
    
    string result = in;
    while(strlen(result) < totalLen) {
        insert(result, 0, padValue );
        }
    return result;
    
    }

string PadWithZero(string in; int totalLen) {
    return PadString(in,"0",totalLen);
    }