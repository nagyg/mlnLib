

int AddPolyLinePrimitive() {

	int primId = addprim(geoself(), "polyline");
	//printf(" * primitive created:%d\n",primId);
	return primId;
}

 int[] GetWalkablePointIdsFromPrimitive(int inputId, primitiveId, pointId){

 	int pointsInPrim[];
 	pointsInPrim = primpoints(inputId, primitiveId);
 	int nofPoints = len(pointsInPrim);

 	if (nofPoints == 0) return pointsInPrim;

 	if (pointId==-1 || pointsInPrim[0]==pointId)
 	{
 		//printf(">> first point, straight walk.\n");
 		pointId = pointsInPrim[0]; //fix -1 always
 		return pointsInPrim;
 	}

 	if (pointsInPrim[nofPoints-1] == pointId)
 	{
 		//printf("<< last point, reverse walk.\n");
 		return reverse(pointsInPrim);	
 	}

 	//printf("?? some mid point, not interesting prim:%d\n", primitiveId);
 	int empty[];
 	return empty;
 }

 int FindClosePolyLine(int inputId, inPrimId, inPointId; float range){

	//find near point
	int oneNearId, nearestPointId, foundPrims[];
	vector endPointPos = point(inputId,"P", inPointId);
	int nearPointIds[] = nearpoints(inputId,endPointPos,range);
	int nofNearPoints = len(nearPointIds);
	//printf(". found %d near points\n", nofNearPoints-1);

	if (nofNearPoints==1){

		// no near point was found
		// start a new primitive
		//printf(". no near points. prim:%d done.\n",inPrimId);
		return -1;
	}

	nearestPointId = -1;
	for(int nIdx=0;nIdx<nofNearPoints;nIdx++){
		oneNearId = nearPointIds[nIdx];
		if (oneNearId == inPointId) continue; //skip myself
		//printf(". nearest point:%d\n",oneNearId);
		nearestPointId = oneNearId;
		break;	
	}

	// found a point near. get the primitive it belongs to
	foundPrims = pointprims(inputId, nearestPointId);

	if (len(foundPrims)==0){
		//printf(". Wtf? no prims?\n");
		return -1;
	}

	if (foundPrims[0]==inPrimId){
		//printf(". not good, prim:%d is the current prim. myself.\n",foundPrims[0]);
		return -1;
	}

	inPrimId = foundPrims[0];

	// save the current point for next round => determine curve direction
	inPointId = nearestPointId;
	//printf(". continue walking on prim:%d start point:%d\n", inPrimId, inPointId);

	return 1;
 }

void AddPointToPolyLine(int primitiveId; vector pos){
	int pidx = addpoint(geoself(), pos);
 	addvertex(geoself(), primitiveId, pidx);
}

void Fuze(int inputId; float range)
{
	//printf("\n\nFuze starts on input:%d range:%d\n",inputId,range);

	
	int nofPrims = nprimitives(inputId);
	//printf("%d points in %d primitives on input:%d\n",npoints(inputId), nofPrims, inputId);

	int outPrimId=-1;
	int pointsInPrim[], nofPointsInPrim;
	int isPrimProcessed;
	vector pos;
	
	int currentPrim=0;
	int skipFirst=0;
	// create a processed prim lookup
	int processedPrims[];
	resize(processedPrims, nofPrims);

	int inPrimId = -1;
	int inPointId= -1;
	//loop 
	while(1){

		if (currentPrim >= nofPrims) {
				//printf("all %d input primitives done.\n", nofPrims);
				break;
			}

		if (inPrimId < 0){
			//printf("stepping to next input primitive:%d\n",currentPrim);
			inPrimId = currentPrim++;
			
		}else{
			//printf("continue walking on primitive:%d\n",inPrimId);	
		}

		isPrimProcessed = processedPrims[inPrimId];
		if (isPrimProcessed==1){
			//printf("primitive:%d was already processed => skip.\n",inPrimId);
			inPrimId = -1;
			inPointId = -1;
			outPrimId = -1;
			continue;
			}

		processedPrims[inPrimId] = 1; //mark it processed
		
		if (inPointId < 0) {
				skipFirst=0;
				//printf("using first point from prim:%d\n",inPrimId);
			}else{
				skipFirst=1;
				//printf("skipping first point from prim:%d\n",inPrimId);
			} 

		// get point ids in an array with proper direction =>>> inPointId
		pointsInPrim = GetWalkablePointIdsFromPrimitive(inputId, inPrimId, inPointId);
		nofPointsInPrim = len(pointsInPrim);
		if (nofPointsInPrim==0){
			//printf("-> prim:%d at point:%d is not connectable!\n",inPrimId, inPointId);
			inPrimId = -1;
			inPointId = -1;
			outPrimId = -1;
			continue;
		}

		//printf("processing input prim:%d nofPoints:%d\n", inPrimId,nofPointsInPrim);
		
		if (outPrimId<0){
			outPrimId = AddPolyLinePrimitive();	
		}

		//loop input prim's points
		for(int pIdx = 0; pIdx < nofPointsInPrim; pIdx++){

			if (skipFirst==1 && pIdx==0) continue;

			inPointId = pointsInPrim[pIdx];
			//printf(" working on point:%d\n", inPointId);

			pos = point(inputId,"P", inPointId);
			AddPointToPolyLine(outPrimId, pos);

			//is last?
			if (pIdx == nofPointsInPrim-1){

				//printf(". last point in prim:%d point:%d\n",inPrimId,inPointId);

				int result = FindClosePolyLine(inputId, inPrimId, inPointId, range);

				if (result >= 0) {



				}else{
					inPrimId = -1;
					inPointId = -1;
					outPrimId = -1;
					continue;
				}
				

			} // is last

		} // poinrs in prim


		 

	} // all prims

}

