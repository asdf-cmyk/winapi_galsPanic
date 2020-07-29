#include "stdafx.h"
#include "movingPoint.h"


movingPoint::~movingPoint()
{
	ptCont.clear();
}

void movingPoint::move(POINT& pt, POINT& vec, int movFlag)
{
	//setPosition(pt);
	//if (ptVector.x != vec.y || ptVector.y != vec.x)
	if ((ptVector.x && !vec.x) || (ptVector.y && !vec.y))
	{
		//ptCont.push_back(pt);
		//movPtPool[poolSz++] = pt;
		pushMovPtPool(pt);
	}
	ptVector = vec;

	switch (movFlag)
	{
	case 1: pt.x -= speed; break;
	case 2: pt.y -= speed; break;
	case 3: pt.x += speed; break;
	case 4: pt.y += speed; break;
	}
}

void movingPoint::show(HDC curHdc, POINT& ptTo)
{
	//Ellipse(curHdc, ptTo.x - 10, ptTo.y - 10, ptTo.x + 10, ptTo.y + 10);

	unsigned int i = 0;
	/*for (i; i < ptCont.size()-1; i++)
	{
		POINT ptFrom = ptCont[i];
		MoveToEx(curHdc, ptCont[i].x, ptCont[i].y, NULL);
		LineTo(curHdc, ptCont[i+1].x, ptCont[i+1].y);
	}
	MoveToEx(curHdc, ptCont[i].x, ptCont[i].y, NULL);
	LineTo(curHdc, ptTo.x, ptTo.y);*/

	if (poolSz > 0)
	{
		for (i; i < poolSz - 1; i++)
		{
			POINT ptFrom = movPtPool[i];
			MoveToEx(curHdc, movPtPool[i].x, movPtPool[i].y, NULL);
			LineTo(curHdc, movPtPool[i + 1].x, movPtPool[i + 1].y);
		}
		MoveToEx(curHdc, movPtPool[i].x, movPtPool[i].y, NULL);
		LineTo(curHdc, ptTo.x, ptTo.y);
	}
	Ellipse(curHdc, ptTo.x - 10, ptTo.y - 10, ptTo.x + 10, ptTo.y + 10);
}

void movingPoint::collision(POINT& pt, std::vector<POINT> polyCont)
{


	//sizeof(polyPool) / sizeof(*polyPool);
	unsigned int contSz = polyCont.size();
	for (unsigned int i = 0; i < contSz; i++)
	{
		/*long long ptToLine =
			(polyCont[(i + 1) % contSz].y - polyCont[i].y) * pt.x +
			(polyCont[i].x - polyCont[(i + 1) % contSz].x) * pt.y +
			(polyCont[(i + 1) % contSz].x - polyCont[i].x) * polyCont[i].x +
			(polyCont[i].y - polyCont[(i + 1) % contSz].y) * polyCont[i].y;
		if (ptToLine <= speed)
		{
			collidState = true;
			break;
		}*/
		long long minX = min(polyCont[i].x, polyCont[(i + 1) % contSz].x);
		long long maxX = max(polyCont[i].x, polyCont[(i + 1) % contSz].x);
		long long minY = min(polyCont[i].y, polyCont[(i + 1) % contSz].y);
		long long maxY = max(polyCont[i].y, polyCont[(i + 1) % contSz].y);
		if (pt.x >= minX && pt.x <= maxX &&
			pt.y >= minY && pt.y <= maxY &&
			(ptVector.x || ptVector.y) &&
			poolSz > 0)
		{
			collidState = true;
			pushMovPtPool(pt);
			break;
		}
	}
}

void movingPoint::mergePoly(std::vector<POINT> polyCont)
{
	std::vector<int> removeVertex;
	for (unsigned int i = 0; i < polyCont.size(); i++)
	{

	}
}

void movingPoint::isVertexStart(POINT& pt, POINT& vec, std::vector<POINT> polyCont)
{
	if (poolSz) return;

	for (unsigned int i = 0; i < polyCont.size(); i++)
	{
		if (pt.x == polyCont[i].x && pt.y == polyCont[i].y)
		{
			pushMovPtPool(pt);
		}
	}
}