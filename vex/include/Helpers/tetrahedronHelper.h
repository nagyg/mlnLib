

// match against one triangle plane
// if the all match the the point is inside of the tetrahedron
int PlaneSide(vector  v1,v2,v3,v4, p) {
    vector normV = cross(v2 - v1, v3 - v1);
    float dotV4 = dot(normV, v4 - v1);
    float dotP = dot(normV, p - v1);
    return sign(dotV4) == sign(dotP);
}

// checks if P is inside the tetrahedron
// per plane is the method
int TetraInside(vector  v1,v2,v3,v4, p) {
    int res = 
        PlaneSide(v1, v2, v3, v4, p) &&
        PlaneSide(v2, v3, v4, v1, p) &&
        PlaneSide(v3, v4, v1, v2, p) &&
        PlaneSide(v4, v1, v2, v3, p);
    return res;
}

// the tale about the tetrahedron and its determinant
float TetraDeterminant(vector pa,pb,pc,pd) {
    vector ab = pb - pa;
    vector ac = pc - pa;
    vector ad = pd - pa;
    matrix3 mm = set(ab,ac,ad);
    return determinant(mm);
}

// In the case of tetrahedron the determinant is a signed volume.
//
//  The tetra is valid if the determinant <> 0
int TetraIsValid(float tDeterminant) {
    return (abs(tDeterminant) > M_TOLERANCE);
}

int TetraIsValid(vector pa,pb,pc,pd) {
    return TetraIsValid(TetraDeterminant(pa,pb,pc,pd));
}

// note: it should match with H's volume measure
float TetraVolume(vector pa,pb,pc,pd) {
    return abs(TetraDeterminant(pa,pb,pc,pd)) / 6.0;

}

// combo
int TetraIsInsideWithCheck(vector  v1,v2,v3,v4, p) {
    if (!TetraIsValid(v1,v2,v3,v4)) return 0; // invalid
    return TetraInside( v1,v2,v3,v4, p);
}
