

if ((forceControlBits & VIS_BIT) > 0) {

    vector visV = { 0,0,0 };
    if (vis_count >= vis_amountLimit) {
        visV = vis_accum;
    } else {
        vis_count =0;
    }

    float accumLen = length(vis_accum);
    vector accuClamped = normalize(vis_accum) * clamp(accumLen, 0, vis_accumMax);
    vector visResult = accuClamped * vis_forceStrength * vis_pMult;
        
    vector bufferedVis = { 0,0,0 };
    if (vis_count >= vis_amountLimit) {
        bufferedVis = AddBufferedVectorValue("vis_buf", ptnum, visResult, vis_bufferSize);
    } else {
        bufferedVis = DegrageBufferedVectorValue("vis_buf", ptnum, vis_fadeReal);
    }

    if (vis_relativeVelocity) {
        float vmult  = FilterByAngle(forward, bufferedVis, vis_relativeStart, vis_relativeEnd);
        bufferedVis *= vmult;
    }

    ForcePushComponent(forceNames, forceVectors, forcePriorities, VIS_FC, bufferedVis, vis_priority);


    // the amount of points affecting the result
    if (vis_writeAffectors){
        Spa("st_i_visAffect", ptnum, vis_count);
        SubmitAffectorPoints("vis", ptnum, vis_affectedBy);
    }

// vis enabled
}