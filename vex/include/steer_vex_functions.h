
#define M_PI 3.14159265

float getAngleBetween(vector a, b) {

    return acos( dot(normalize(a),normalize(b)) );

}

vector projectOnPlane(vector in; vector planeNormal) {
  
  return (in - dot(in, normalize(planeNormal) ));
}

void addSteer(vector steer_force; float steer_priority; string name) {

    //float steer_force = 0;
    int steer_count = 0;

    // todo: set in detail attribute
    import("steer_count", steer_count, 0 );

  

    // set steering force and priority
    addattribute(concat("steer_force",sprintf("%d",steer_count)), steer_force);
    addattribute(concat("steer_priority", sprintf("%d", steer_count)), steer_priority);

    // extend steering forces
    steer_count++;
    addattribute("steer_count", steer_count);
    

}

vector spaceAgent2World(vector in) {

    vector up;
    import("up",up, 0);
    vector side = normalize(cross(v,up));
    vector forward = normalize(v);

    float x = in[0];
    float y = in[1];
    float z = in[2];
    

    return x * forward + y * up + z * side; 
 
}

void setSpeed(float newSpeed) {
  v = normalize(v) * newSpeed;
  addattribute("speed", newSpeed);
}

float speed() {
  float speed = 0;
  import("speed", speed, 0);
  //return length(v);
  return speed;

}

void setForward(vector frwd) {     
  addattribute("forward", normalize(frwd));
}

vector forward() {

  vector frd = {1,0,0};
  if (!import("forward",frd,0)) {
    frd = v;
  };
  if (frd == {0,0,0}) {
    frd = {1,0,0};
    /*
    frd = v;
    if (frd == {0,0,0}) {
      return {1,0,0};
    } 
      */
  } 
  return normalize(frd);
  
}

vector up() {
  vector up = {0,1,0};
  import("up", up, 0);
  return up;
}

void setUp(vector newUp) {
  addattribute("up",normalize(newUp));
}

matrix3 agentRotationMatrix() {

  vector up = {0,1,0};
  import("up",up, 0);
  
  return dihedral({1,0,0},forward());
}

vector agent2world_vector(vector in) {

  // get rotation matrix
  matrix3 rot = ident();
  if (!import("rot", rot, 0)) {
    rot = agentRotationMatrix();
  }
  return in * rot;
}

vector world2agent_vector(vector in) {
  // get rotation matrix
  matrix3 rot = ident();
  if (!import("rot", rot, 0)) {
    rot = agentRotationMatrix();
  }
  return invert(rot) * in;
}


vector setVectorSize(vector in; float newsize) {
  return normalize(in) * newsize;
}

void bubbleSort(float prio[]; vector steer[]) {

    int num = len(prio);
    int exch = 0;

    do {
        exch = 0;
        int i;
        for (i = 0; i < num-1; i++) {
            if (prio[i] < prio[i+1]) {
                
                float tempp = prio[i];
                prio[i] = prio[i+1];
                prio[i+1] = tempp;

                vector temps = steer[i];
                steer[i] = steer[i+1];
                steer[i+1] = temps;
                exch = 1;
            }
        }
    } while (exch && num > 1);

}