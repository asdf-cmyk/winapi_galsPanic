#include "stdafx.h"
#include "movingPoint.h"


movingPoint::~movingPoint()
{
}

void movingPoint::moving(POINT& pt, POINT& vec)
{
	//setPosition(pt);
	if (ptVector.x != vec.y || ptVector.y != vec.x)
	{
		ptCont.push_back(position);
	}
	ptVector = vec;
}

void movingPoint::show()
{
	
}