#include "stdafx.h"
#include "movingPoint.h"


movingPoint::~movingPoint()
{
	movPtCont.clear();
}


void movingPoint::pushMovPtPool(POINT& pt) {
	if (isVertex2) return;
	position = pt;
	movPtCont.push_back(position);
}


void movingPoint::show(HDC curHdc, POINT& ptTo)
{
	unsigned int i = 0;
	if (movPtCont.size() > 0)
	{
		for (i; i < movPtCont.size() - 1; i++)
		{
			POINT ptFrom = movPtCont[i];
			MoveToEx(curHdc, movPtCont[i].x, movPtCont[i].y, NULL);
			LineTo(curHdc, movPtCont[i + 1].x, movPtCont[i + 1].y);
		}
		MoveToEx(curHdc, movPtCont[i].x, movPtCont[i].y, NULL);
		LineTo(curHdc, ptTo.x, ptTo.y);
	}
	Ellipse(curHdc, ptTo.x - radius, ptTo.y - radius, ptTo.x + radius, ptTo.y + radius);
}


void movingPoint::collision(POINT& pt, std::vector<POINT>& polyVertexCont, bool downKeyFlag)
{
	unsigned int contSz = polyVertexCont.size();
	for (unsigned int i = 0; i < contSz; i++)
	{
		LONG minX = min(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
		LONG maxX = max(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
		LONG minY = min(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
		LONG maxY = max(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
		if (pt.x >= minX && pt.x <= maxX &&
			pt.y >= minY && pt.y <= maxY &&
			(ptVector.x || ptVector.y) &&
			movPtCont.size() > 0)
		{
			collidState = true;
			pushMovPtPool(pt);
			break;
		}
	}
}


void movingPoint::move(POINT& pt, POINT& vec, std::vector<POINT>& polyVertexCont,
	std::vector<polyLine>& polyLineCont, int movState, bool downKeyFlag, RECT rectView)
{
	if (movState == 4) return;

	unsigned int lineContSz = polyLineCont.size();
	static unsigned int lineIndex;
	static unsigned int vertexIndex;

	if (!isVertex)
	{
		for (vertexIndex = 0; vertexIndex < polyVertexCont.size(); vertexIndex++)
		{
			if ((pt.x == polyVertexCont[vertexIndex].x) && (pt.y == polyVertexCont[vertexIndex].y))
			{
				isVertex = true;
				isVertex2 = false;
				isInLine = false;
				break;
			}
		}
	}

	if (isVertex && !movPtCont.size() && movState >= 0 && movState <= 3)
	{
		int secondLineIndex = (vertexIndex + lineContSz - 1) % lineContSz;

		if ((polyLineCont[vertexIndex].polyState + 4 - polyLineCont[secondLineIndex].polyState) %
			4 == 3)
			isVertex2 = true;

		if (!(polyLineCont[vertexIndex].polyState == movState) &&
			!(polyLineCont[secondLineIndex].polyState == movState))
		{
			if (!downKeyFlag && !isVertex2)
				return;
			pushMovPtPool(pt);
			ptVector.x = ptVector.y = 0;
		}
		isVertex = false;
		isInLine = false;
	}
	else if (!isVertex && !isInLine  && movState >= 0 && movState <= 3)
	{
		for (lineIndex = 0; lineIndex < lineContSz; lineIndex++)
		{
			LONG minX = min(polyVertexCont[polyLineCont[lineIndex].startVertex].x,
				polyVertexCont[polyLineCont[lineIndex].endVertex].x);
			LONG maxX = max(polyVertexCont[polyLineCont[lineIndex].startVertex].x,
				polyVertexCont[polyLineCont[lineIndex].endVertex].x);
			LONG minY = min(polyVertexCont[polyLineCont[lineIndex].startVertex].y,
				polyVertexCont[polyLineCont[lineIndex].endVertex].y);
			LONG maxY = max(polyVertexCont[polyLineCont[lineIndex].startVertex].y,
				polyVertexCont[polyLineCont[lineIndex].endVertex].y);

			if ((pt.x >= minX) && (pt.x <= maxX) && (pt.y >= minY) && (pt.y <= maxY))
			{
				isInLine = true;
				isVertex = false;
				isVertex2 = false;
				break;
			}
		}
	}


	/*if (movPtCont.size() && (ptVector.x != vec.x) && (ptVector.y != vec.y))
	{
		if (!downKeyFlag)
			return;
		pushMovPtPool(pt);
		isInLine = false;
		isVertex = false;
		isVertex2 = false;
	}

	if (movPtCont.size())
	{
		if (!downKeyFlag ||
			(ptVector.x != 0 && ptVector.x == -1 * vec.x) ||
			(ptVector.y != 0 && ptVector.y == -1 * vec.y))
			return;
	}*/

	//bool interPtFlag = false;
	if (movPtCont.size() > 2)
	{
		if (!downKeyFlag)
			return;

		for (unsigned int i = 0; i < movPtCont.size() - 2; i++)
		{
			switch (movState)
			{
			case 0:
				if (movPtCont[i].x == movPtCont[i + 1].x &&
					min(movPtCont[i].y, movPtCont[i + 1].y) <= pt.y &&
					pt.y <= max(movPtCont[i].y, movPtCont[i + 1].y) &&
					pt.x > movPtCont[i].x &&
					pt.x - max(radius, speed) <= movPtCont[i].x)
					//interPtFlag = true;
					return;
				break;
			case 1:
				if (movPtCont[i].y == movPtCont[i + 1].y &&
					min(movPtCont[i].x, movPtCont[i + 1].x) <= pt.x &&
					pt.x <= max(movPtCont[i].x, movPtCont[i + 1].x) &&
					pt.y > movPtCont[i].y &&
					pt.y - max(radius, speed) <= movPtCont[i].y)
					//interPtFlag = true;
					return;
				break;
			case 2:
				if (movPtCont[i].x == movPtCont[i + 1].x &&
					min(movPtCont[i].y, movPtCont[i + 1].y) <= pt.y &&
					pt.y <= max(movPtCont[i].y, movPtCont[i + 1].y) &&
					pt.x < movPtCont[i].x &&
					pt.x + max(radius, speed) >= movPtCont[i].x)
					//interPtFlag = true;
					return;
				break;
			case 3:
				if (movPtCont[i].y == movPtCont[i + 1].y &&
					min(movPtCont[i].x, movPtCont[i + 1].x) <= pt.x &&
					pt.x <= max(movPtCont[i].x, movPtCont[i + 1].x) &&
					pt.y < movPtCont[i].y &&
					pt.y + max(radius, speed) >= movPtCont[i].y)
					//interPtFlag = true;
					return;
				break;
			}
		}
	}

	if (movPtCont.size() && (ptVector.x != vec.x) && (ptVector.y != vec.y))
	{
		if (!downKeyFlag/* || interPtFlag*/)
			return;
		pushMovPtPool(pt);
		isInLine = false;
		isVertex = false;
		isVertex2 = false;
	}

	if (movPtCont.size())
	{
		if (!downKeyFlag ||
			(ptVector.x != 0 && ptVector.x == -1 * vec.x) ||
			(ptVector.y != 0 && ptVector.y == -1 * vec.y))
			return;
	}

	if (isInLine)
	{
		if (movState == polyLineCont[lineIndex].polyState)
			return;
		else if (!movPtCont.size() &&
			(movState + 2) % 4 == polyLineCont[lineIndex].polyState)
		{
			if (!downKeyFlag)
				return;
			pushMovPtPool(pt);
		}
	}
	else if (isVertex2)
	{
		if (movState == polyLineCont[vertexIndex].polyState ||
			movState == polyLineCont[(vertexIndex + polyVertexCont.size() - 1) % polyVertexCont.size()].polyState)
			return;
	}


	ptVector = vec;

	switch (movState)
	{
	case 0:
		if (pt.x - speed < 0) return;
		pt.x -= speed;
		break;
	case 1:
		if (pt.y - speed < 0) return;
		pt.y -= speed;
		break;
	case 2:
		if (pt.x + speed > rectView.right) return;
		pt.x += speed;
		break;
	case 3:
		if (pt.y + speed > rectView.bottom) return;
		pt.y += speed;
		break;
	}
}


void movingPoint::mergePoly(std::vector<POINT>& polyVertexCont,
	std::vector<polyLine>& polyLineCont)
{
	if (!movPtCont.size()) return;

	std::vector<POINT> tmpMerge1;

	areaSize = shoelace(movPtCont);
	if (!clockWise)
		std::reverse(movPtCont.begin(), movPtCont.end());

	unsigned int lineContSz = polyLineCont.size();
	unsigned int startLineIndex = -1, startLineIndex2 = -1, endLineIndex = -1, endLineIndex2 = -1;
	int lineCnt = 0;
	bool isMovPtBeginVertex = false, isMovPtEndVertex = false;
	for (unsigned int i = 0; i < lineContSz; i++)
	{
		LONG minX = min(polyVertexCont[polyLineCont[i].startVertex].x,
			polyVertexCont[polyLineCont[i].endVertex].x);
		LONG maxX = max(polyVertexCont[polyLineCont[i].startVertex].x,
			polyVertexCont[polyLineCont[i].endVertex].x);
		LONG minY = min(polyVertexCont[polyLineCont[i].startVertex].y,
			polyVertexCont[polyLineCont[i].endVertex].y);
		LONG maxY = max(polyVertexCont[polyLineCont[i].startVertex].y,
			polyVertexCont[polyLineCont[i].endVertex].y);

		if ((movPtCont[0].x >= minX) && (movPtCont[0].x <= maxX) &&
			(movPtCont[0].y >= minY) && (movPtCont[0].y <= maxY))
		{
			if (startLineIndex != -1)
				isMovPtBeginVertex = true;
			if (startLineIndex == -1)
				startLineIndex = i;
			else if (startLineIndex2 == -1)
				startLineIndex2 = i;
		}

		if ((movPtCont[movPtCont.size() - 1].x >= minX) && (movPtCont[movPtCont.size() - 1].x <= maxX) &&
			(movPtCont[movPtCont.size() - 1].y >= minY) && (movPtCont[movPtCont.size() - 1].y <= maxY))
		{
			if (endLineIndex != -1)
				isMovPtEndVertex = true;
			if (endLineIndex == -1)
				endLineIndex = i;
			else if (endLineIndex2 == -1)
				endLineIndex2 = i;
		}
	}

	for (unsigned int i = 0; i < movPtCont.size(); i++)
		tmpMerge1.push_back(movPtCont[i]);

	bool vertexToLineFlag = false;
	POINT backup;

	if (isMovPtBeginVertex)
	{
		if (polyVertexCont[polyLineCont[startLineIndex2].endVertex].x == movPtCont[0].x &&
			polyVertexCont[polyLineCont[startLineIndex2].endVertex].y == movPtCont[0].y)
			startLineIndex = startLineIndex2;
		startLineIndex2 = -1;

		if (((movPtCont[0].x == movPtCont[1].x) && (polyLineCont[startLineIndex].slope != 0)) ||
			((movPtCont[0].x != movPtCont[1].x) && (polyLineCont[startLineIndex].slope == 0)))
		{
			vertexToLineFlag = true;
			tmpMerge1.erase(tmpMerge1.begin());
		}
	}
	if (isMovPtEndVertex)
	{
		if (polyVertexCont[polyLineCont[endLineIndex2].startVertex].x == movPtCont[movPtCont.size() - 1].x &&
			polyVertexCont[polyLineCont[endLineIndex2].startVertex].y == movPtCont[movPtCont.size() - 1].y)
			endLineIndex = endLineIndex2;
		endLineIndex2 = -1;

		if (((movPtCont[movPtCont.size() - 1].x == movPtCont[movPtCont.size() - 2].x) && (polyLineCont[endLineIndex].slope != 0)) ||
			((movPtCont[movPtCont.size() - 1].x != movPtCont[movPtCont.size() - 2].x) && (polyLineCont[endLineIndex].slope == 0)))
		{
			vertexToLineFlag = true;
			tmpMerge1.pop_back();
		}
	}

	if (startLineIndex != endLineIndex)
	{
		for (unsigned int i = endLineIndex; i != startLineIndex; i = (i + 1) % polyVertexCont.size())
			tmpMerge1.push_back(polyVertexCont[polyLineCont[i].endVertex]);

		LONG tmp1 = shoelace(tmpMerge1);
		LONG poly = shoelace(polyVertexCont);

		if (shoelace(tmpMerge1) < shoelace(polyVertexCont))
		{
			std::reverse(movPtCont.begin(), movPtCont.end());
			mergePoly(polyVertexCont, polyLineCont);
			return;
		}
	}
	else
	{
		std::vector<POINT> tmpMerge2(tmpMerge1);
		std::vector<POINT> tmpMerge3(tmpMerge1);
		for (unsigned int i = !vertexToLineFlag; i <= polyVertexCont.size() - vertexToLineFlag; i++)
			tmpMerge1.push_back(polyVertexCont[(startLineIndex + i) % polyVertexCont.size()]);
		for (unsigned int i = polyVertexCont.size(); i >= 1; i--)
			tmpMerge2.push_back(polyVertexCont[(startLineIndex + i) % polyVertexCont.size()]);

		if (shoelace(tmpMerge2) < shoelace(tmpMerge3))
			tmpMerge1.swap(tmpMerge3);
	}

	tmpMerge1.swap(polyVertexCont);

	collidState = false;
	movPtCont.clear();
	polyLineCont.clear();
	ptVector = { 0, 0 };
	isVertex = false;
	isVertex2 = false;
	isInLine = false;
}

LONG movingPoint::shoelace(std::vector<POINT>& movPtCont)
{
	LONG area = 0;
	for (unsigned int i = 0; i < movPtCont.size(); i++)
	{
		area += movPtCont[i].x * movPtCont[(i + 1) % movPtCont.size()].y;
		area -= movPtCont[i].y * movPtCont[(i + 1) % movPtCont.size()].x;
	}

	clockWise = area < 0 ? false : true;

	return abs(area) / 2;
}