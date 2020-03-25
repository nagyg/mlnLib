

float SolveForPlane(vector forcePos, forceDir ) {
    // align the system to X axis
    matrix mtx = dihedral(forcePos, {1,0,0});
    // also rotate the force dir
    vector rotForce = mtx * forceDir;
    // since we aligned to X the perpendicular / useful component is Y
    return rotForce.y;
}


vector SphericalTorque(vector targetPos, forceSourcePos, forceDir) {
 
    // relative position of the force
    vector forceRel = forceSourcePos - targetPos; 

    // split the direction into 3 planes and solve without knowing wtf
    //   the axis order is the trick here
    vector forcePosYZ = set( forceRel.y, forceRel.z ,0);
    vector forceDirYZ = set( forceDir.y, forceDir.z ,0);
    float fx = SolveForPlane(forcePosYZ, forceDirYZ);

    vector forcePosXZ = set( forceRel.z, forceRel.x ,0);
    vector forceDirXZ = set( forceDir.z, forceDir.x, 0);
    float fy =  SolveForPlane(forcePosXZ, forceDirXZ);

    vector forcePosXY = set( forceRel.x, forceRel.y, 0);
    vector forceDirXY = set( forceDir.x, forceDir.y, 0);
    float fz = SolveForPlane(forcePosXY, forceDirXY);

    return set(fx,fy,fz);
}

// point cloud version
float SphericalTorquePc( int inputId, maxPts; vector pos; float radius, size; string forceName, distRampName; vector torqueResult ) {

    torqueResult = {0,0,0};
    float oDistance, oMult, accuResult, result, distMult, distRate;
    int oPtnum, count;
    vector oP, oForce, accuTorque, torqueCurrent;
     
    int handle = pcopen(inputId,"P",pos, radius, maxPts );

    accuResult = 0;
    count=0;
    while (pciterate(handle))
    {  
        pcimport(handle, "P", oP);
        pcimport(handle, "point.distance", oDistance);
        pcimport(handle,  forceName, oForce);

        if (oDistance > size) continue; // far case
        torqueCurrent = SphericalTorque(pos, oP, oForce);
        distRate = oDistance / size;
        distMult = chramp(distRampName, distRate);
        count++;
        //printf("distRate:%d distMult:%d count:%d ", distRate, distMult, count);
        accuResult += distMult;
        torqueResult += torqueCurrent * distMult;
    }
    pcclose(handle);
   
    if (count==0) {
        return -1;
    }
    float rate = accuResult / float(count);
    return rate;
}

