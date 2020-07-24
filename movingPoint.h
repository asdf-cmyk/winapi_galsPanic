#pragma once
#include "stdafx.h"
class movingPoint
{
	std::vector<POINT> ptCont;
	POINT position, ptVector;
public:
	movingPoint() : ptVector({ 0, 0 }) {}
	~movingPoint();

	void setPosition(POINT& pt) { position = pt; }
	void moving(POINT&, POINT&);
	void show();
};

