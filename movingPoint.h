#pragma once
#include "stdafx.h"
#include <algorithm>

struct polyLine {
	int startVertex;
	int endVertex;
	double slope;
	int polyState;
};
class movingPoint
{
	std::vector<POINT> movPtCont;
	POINT position, ptVector;
	int speed, radius;
	bool collidState;
	bool isInLine;
	bool isVertex;
	bool isVertex2;
	bool clockWise;
	LONG areaSize;
public:
	movingPoint() : ptVector({ 0, 0 }), speed(10), radius(10), collidState(false),
		isInLine(false), isVertex(false), isVertex2(false) {}
	~movingPoint();

	void pushMovPtPool(POINT&);
	void show(HDC, POINT&);
	void move(POINT&, POINT&, std::vector<POINT>&, std::vector<polyLine>&, int, bool, RECT);
	void collision(POINT&, std::vector<POINT>&, bool);
	void mergePoly(std::vector<POINT>&, std::vector<polyLine>&);
	LONG shoelace(std::vector<POINT>&);

	std::vector<POINT> getPtCont() { return movPtCont; }
	POINT getPos() { return position; }
	POINT getPtVec() { return ptVector; }
	bool getCollidState() { return collidState; }
	int getContSz() { return movPtCont.size(); }

	void setPtVec(POINT& vec) { ptVector = vec; }
};