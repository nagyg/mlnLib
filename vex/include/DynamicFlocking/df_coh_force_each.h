

if ((forceControlBits & COH_BIT) > 0 && ptnum != oPtnum && realDist <= coh_forceRadius  ) {

		if (coh_writeAffectors) {
			push(coh_affectedBy, oPtnum);
		}

		coh_accum += oP;
		coh_count ++;

// within effect radius
// not myself
// coh enabled
}