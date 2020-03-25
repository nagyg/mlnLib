

if ((forceControlBits & COH_BIT) > 0) {

    vector avgPoint = {0,0,0};
    vector newCoh = {0,0,0};

    if (coh_count >= coh_amountLimit) {
        avgPoint = coh_accum / (float)coh_count;
        newCoh = avgPoint - P ;
    } else {
        //reset counter to reflect that we are under amount limit
        coh_count =0;
    }

    newCoh = normalize(newCoh) * coh_pMult * coh_forceStrength;
        
    vector bufferedCoh = { 0,0,0 };
    if (coh_count >= coh_amountLimit) {
        bufferedCoh = AddBufferedVectorValue("coh_buf", ptnum, newCoh, coh_bufferSize);
    } else {
        bufferedCoh = DegrageBufferedVectorValue("coh_buf", ptnum, coh_fadeReal);
        }

    if (coh_relativeVelocity) {
        float relativeFactor = FilterByAngle(forward, bufferedCoh, coh_relativeStart, coh_relativeEnd);
        bufferedCoh *= relativeFactor;
        }

    ForcePushComponent(forceNames, forceVectors, forcePriorities, COH_FC, bufferedCoh, coh_priority);
  
    if (coh_writeAffectors) {
        Spa("st_i_cohAffect", ptnum, coh_count);
        SubmitAffectorPoints("coh", ptnum, coh_affectedBy);
    }

//coh enabled
}        


