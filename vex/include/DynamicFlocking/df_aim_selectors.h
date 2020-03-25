

string aimPointsGeo = chs("../aim_aimpointsgeo");
string aimPointsPath = concat("op:", opfullpath(aimPointsGeo));
int nofAimPoints = npoints(aimPointsPath);
//printf("aim geo: %s\n", opfullpath(aimPointsGeo));

int aimId = 0; //if no selector runs, the aim will use the first point

int aimSelectorMode = chi("../aim_selectormode");
if (aimSelectorMode == 1) {

	// random selector

  	float switchPeriod = chf("../aim_rnd_period");
  	float chance = chf("../aim_rnd_chance");
  
	if (nofAimPoints == 1) {} else  {

		int nofChanges = Gpa("aimNofChanges", ptnum, 0);
		aimId = Gpa("aimId", ptnum, 0);
		float aimLast = Gpa("aimLastChange", ptnum, 0.0);

		if ((Time - aimLast) < switchPeriod) {} else {

			float ft = floor(Frame);

			if (rand( (ptnum+1) * ( ft + 0.31277)) < chance) {
				
				aimId = int(floor( rand((ptnum+1) * (ft+0.1113)) * nofAimPoints ));
				nofChanges ++;

				Spa("aimId", ptnum, aimId);
				Spa("aimLastChange", ptnum, Time);
				Spa("aimNofChanges", ptnum, nofChanges);
			}
		}
	}

} else if (aimSelectorMode == 2) {

	// select next
	float switchPeriod = chf("../aim_next_period");
  	float chance = chf("../aim_next_chance");
  
	if (nofAimPoints == 1) {} else  {

		int nofChanges = Gpa("aimNofChanges", ptnum, 0);
		aimId = Gpa("aimId", ptnum, 0);
		float aimLast = Gpa("aimLastChange", ptnum, 0.0);
		

		if ((Time - aimLast) < switchPeriod) {} else {

			float ft = floor(Frame);

			if (rand( (ptnum+1) * ( ft + 0.31277)) < chance) {
				
				aimId = (aimId + 1) % nofAimPoints;
				nofChanges ++;

				Spa("aimId", ptnum, aimId);
				Spa("aimLastChange", ptnum, Time);
				Spa("aimNofChanges", ptnum, nofChanges);
			}
		}
	}

}
