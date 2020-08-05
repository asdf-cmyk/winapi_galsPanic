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
	int speed;
	bool collidState;
	bool isInLine;
	bool isVertex;
	bool isVertex2;
	bool outPolygon;
	bool clockWise;
	LONG areaSize;
public:
	movingPoint() : ptVector({ 0, 0 }), speed(5), collidState(false), isInLine(false), isVertex(true), isVertex2(false) {}
	~movingPoint();

	void pushMovPtPool(POINT& pt);
	void show(HDC, POINT&);
	void move(POINT&, POINT&, std::vector<POINT>&, std::vector<polyLine>&, int, bool);
	void collision(POINT&, std::vector<POINT>&);
	void mergePoly(std::vector<POINT>&, std::vector<polyLine>&);
	//void isVertexStart(POINT&, int, std::vector<POINT>&, std::vector<polyLine>&);
	void calMovPtArea();


	std::vector<POINT> getPtCont() { return movPtCont; }
	POINT getPos() { return position; }
	POINT getPtVec() { return ptVector; }
	bool getCollidState() { return collidState; }
	int getContSz() { return movPtCont.size(); }

	void setPtVec(POINT& vec) { ptVector = vec; }
};

