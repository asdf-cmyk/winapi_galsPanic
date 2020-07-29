#pragma once
#include "stdafx.h"
class movingPoint
{
	std::vector<POINT> ptCont;
	POINT position, ptVector;
	POINT movPtPool[1360];
	int poolSz = 0;
	int speed = 5;
	bool collidState = false;
public:
	movingPoint() : ptVector({ 0, 0 }) {}
	~movingPoint();

	void pushMovPtPool(POINT& pt) { position = pt;
	//ptCont.push_back(position);
	movPtPool[poolSz++] = position;
	}
	void move(POINT&, POINT&, int);
	void show(HDC, POINT&);
	void collision(POINT&, std::vector<POINT>);
	void mergePoly(std::vector<POINT>);
	void isVertexStart(POINT&, POINT&, std::vector<POINT>);


	std::vector<POINT> getPtCont() { return ptCont; }
	POINT* getMovPtPool() { return movPtPool; }
	POINT getPos() { return position; }
	POINT getPtVec() { return ptVector; }
	bool getCollidState() { return collidState; }
	int getPoolSz() { return poolSz; }

	void setPtVec(POINT& vec) { ptVector = vec; }
};

