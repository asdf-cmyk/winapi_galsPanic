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
	//Ellipse(curHdc, ptTo.x - 10, ptTo.y - 10, ptTo.x + 10, ptTo.y + 10);

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
	Ellipse(curHdc, ptTo.x - 2, ptTo.y - 2, ptTo.x + 2, ptTo.y + 2);
}


void movingPoint::collision(POINT& pt, std::vector<POINT>& polyVertexCont)
{
	//isVertex isInLine 추가?

	unsigned int contSz = polyVertexCont.size();
	for (unsigned int i = 0; i < contSz; i++)
	{
		/*long long ptToLine =
		(polyVertexCont[(i + 1) % contSz].y - polyVertexCont[i].y) * pt.x +
		(polyVertexCont[i].x - polyVertexCont[(i + 1) % contSz].x) * pt.y +
		(polyVertexCont[(i + 1) % contSz].x - polyVertexCont[i].x) * polyVertexCont[i].x +
		(polyVertexCont[i].y - polyVertexCont[(i + 1) % contSz].y) * polyVertexCont[i].y;
		if (ptToLine <= speed)
		{
		collidState = true;
		break;
		}*/
		LONG minX = min(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
		LONG maxX = max(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
		LONG minY = min(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
		LONG maxY = max(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
		if (pt.x >= minX && pt.x <= maxX &&
			pt.y >= minY && pt.y <= maxY &&
			(ptVector.x || ptVector.y) &&
			movPtCont.size() > 0)
		{
			//ptVector = { 0, 0 }; /*test*/
			collidState = true;
			pushMovPtPool(pt);
			break;
		}
	}
}


void movingPoint::move(POINT& pt, POINT& vec, std::vector<POINT>& polyVertexCont,
	std::vector<polyLine>& polyLineCont, int movState, bool downKeyFlag)
{
	if (movState == 4) return;

	unsigned int lineContSz = polyLineCont.size();
	static unsigned int lineIndex;
	static unsigned int vertexIndex;
	//int movingPtState = -1; // 0: outPolyon / 1: inPoly / 2: inLine

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
			4 == 3) //&&
			//(movState == polyLineCont[vertexIndex].polyState ||
			//movState == polyLineCont[(vertexIndex + polyVertexCont.size() - 1) % polyVertexCont.size()].polyState))
			//return;
			isVertex2 = true;

		if (!(polyLineCont[vertexIndex].polyState == movState) &&
			!(polyLineCont[secondLineIndex].polyState == movState)
			)
		{
			pushMovPtPool(pt);
		}
		isVertex = false;
		isInLine = false;
	}
	else if (!isVertex && !isInLine  && movState >= 0 && movState <= 3)
	{
		for (lineIndex = 0; lineIndex < lineContSz; lineIndex++)
		{
			if (lineIndex == 6)
			{
				int test = 0;
			}
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

	if (movPtCont.size() && (ptVector.x != vec.x) && (ptVector.y != vec.y))
	{
		pushMovPtPool(pt);
		isInLine = false;
		isVertex = false;
		isVertex2 = false;
	}

	//if (!movPtCont.size())
	//{
		if (isInLine)
		{
			if (movState == polyLineCont[lineIndex].polyState)
				return;
			else if(!movPtCont.size() &&
				(movState+2) % 4 == polyLineCont[lineIndex].polyState)
				pushMovPtPool(pt);//
		}
		else if (isVertex2)
		{
			if (movState == polyLineCont[vertexIndex].polyState ||
				movState == polyLineCont[(vertexIndex + polyVertexCont.size() - 1) % polyVertexCont.size()].polyState)
				return;
		}


	//if (!downKeyFlag)

	ptVector = vec;

	switch (movState)
	{
	case 0:
		pt.x -= speed;
		break;

	case 1:
		pt.y -= speed;
		break;

	case 2:
		pt.x += speed;
		break;

	case 3:
		pt.y += speed;
		break;
	}
}


//void movingPoint::isVertexStart(POINT& pt, int movState,
//	std::vector<POINT>& polyVertexCont, std::vector<polyLine>& polyLineCont)
//{
//	if (movPtCont.size()) return;
//
//	int pLContSz = polyLineCont.size();
//	for (unsigned int i = 0; i < polyVertexCont.size(); i++)
//	{
//		int secondLineIndex = (i + pLContSz - 1) % pLContSz;
//		if ((polyLineCont[i].polyState + pLContSz - polyLineCont[secondLineIndex].polyState) %
//			pLContSz == 3)
//			continue;
//
//		else if ((pt.x == polyVertexCont[i].x && pt.y == polyVertexCont[i].y) &&
//			!(polyLineCont[i].polyState == movState) &&
//			!(polyLineCont[secondLineIndex].polyState == movState)
//			)
//		{
//			pushMovPtPool(pt);
//		}
//	}
//}


void movingPoint::mergePoly(std::vector<POINT>& polyVertexCont,
	std::vector<polyLine>& polyLineCont)
{
	if (!movPtCont.size()) return;

	unsigned int lineContSz = polyLineCont.size();
	/*static*/ unsigned int startLineIndex, endLineIndex;
	for (unsigned int i = 0; i < lineContSz; i++)
	{
		if (polyVertexCont[0].x)
		{

		}

		if (polyVertexCont[polyVertexCont.size() - 1].x)
		{

		}

	}




	/*std::vector<int> removeVertex;
	for (unsigned int i = 0; i < polyVertexCont.size(); i++)
	{
		int isInFlag = 0;
		unsigned int movPtContSz = movPtCont.size();
		for (int j = 1; j < movPtContSz - 2; j++)
		{
			LONG minX = min(movPtCont[j].x, movPtCont[(j + 1) % movPtContSz].x);
			LONG maxX = max(movPtCont[j].x, movPtCont[(j + 1) % movPtContSz].x);
			LONG minY = min(movPtCont[j].y, movPtCont[(j + 1) % movPtContSz].y);
			LONG maxY = max(movPtCont[j].y, movPtCont[(j + 1) % movPtContSz].y);

			if (polyVertexCont[i].x >= minX && polyVertexCont[i].x <= maxX)
			{
				int secondLine = (i + polyLineCont.size() - 1) % polyLineCont.size();
				if ((polyLineCont[i].polyState == 1 ||
					polyLineCont[secondLine].polyState == 1) &&
					polyVertexCont[i].y < minY)
					isInFlag++;
				else if ((polyLineCont[i].polyState == 3 ||
					polyLineCont[secondLine].polyState == 3) &&
					polyVertexCont[i].y > maxY)
					isInFlag++;				
			}
			else if (polyVertexCont[i].y >= minY && polyVertexCont[i].y <= maxY)
			{
				int secondLine = (i + polyLineCont.size() - 1) % polyLineCont.size();
				if ((polyLineCont[i].polyState == 0 ||
					polyLineCont[secondLine].polyState == 0) &&
					polyVertexCont[i].x < minX)
					isInFlag++;
				else if ((polyLineCont[i].polyState == 2 ||
					polyLineCont[secondLine].polyState == 2) &&
					polyVertexCont[i].x > maxX)
					isInFlag++;
			}

		}
		if (isInFlag % 2)
			removeVertex.push_back(i);
	}


	calMovPtArea();

	if(!clockWise)
		std::reverse(movPtCont.begin(), movPtCont.end());

	unsigned int I;
	if (!removeVertex.size())
	{
		unsigned int lineContSz = polyLineCont.size();
		unsigned int lineIndex;
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

			if ((position.x >= minX) && (position.x <= maxX) &&
				(position.y >= minY) && (position.y <= maxY))
				break;
		}

		I = lineIndex;
	}
	else
	{
		for (unsigned int i = 0; i < removeVertex.size(); i++)
		{
			polyVertexCont.erase(polyVertexCont.begin() + removeVertex[0]);
		}
	}

	if(removeVertex.size())
		for (unsigned int i = 0; i < movPtCont.size(); i++)
		{
			polyVertexCont.insert(polyVertexCont.begin() + removeVertex[0] + i, movPtCont[i]);
		}
	else
		for (unsigned int i = 0; i < movPtCont.size(); i++)
		{
			unsigned int tmp = I + i + 1;
			polyVertexCont.insert(polyVertexCont.begin() + tmp, movPtCont[i]);
		}*/

	collidState = false;
	movPtCont.clear();
	polyLineCont.clear();
	ptVector = { 0, 0 };
	position;
}

void movingPoint::calMovPtArea()
{
	LONG area = 0;
	for (unsigned int i = 0; i < movPtCont.size(); i++)
	{
		area += movPtCont[i].x * movPtCont[(i + 1) % movPtCont.size()].y;
		area -= movPtCont[i].y * movPtCont[(i + 1) % movPtCont.size()].x;
	}

	areaSize = abs(area)/2;
	clockWise = area < 0 ? false : true;
}




//void movingPoint::move(POINT& pt, POINT& vec,
//	std::vector<POINT>& polyVertexCont, bool vertexLine[][5], int movState)
//{
//	//setPosition(pt);
//	//if (ptVector.x != vec.y || ptVector.y != vec.x)
//	if (//poolSz > 0 && 
//		((ptVector.x && !vec.x) || (ptVector.y && !vec.y)))
//	{
//		//movPtCont.push_back(pt);
//		//movPtPool[poolSz++] = pt;
//		pushMovPtPool(pt);
//	}
//	ptVector = vec;
//
//	//std::vector<POINT>& polyVertexCont 필요?
//	switch (movState)
//	{
//	case 0: pt.x -= speed; break;
//	case 1: pt.y -= speed; break;
//	case 2: pt.x += speed; break;
//	case 3: pt.y += speed; break;
//	}
//
//	unsigned int contSz = polyVertexCont.size();
//	for (unsigned int i = 0; i < contSz; i++)
//	{
//		long long minX = min(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
//		long long maxX = max(polyVertexCont[i].x, polyVertexCont[(i + 1) % contSz].x);
//		long long minY = min(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
//		long long maxY = max(polyVertexCont[i].y, polyVertexCont[(i + 1) % contSz].y);
//
//		if (vertexLine[i][2] && vertexLine[i][3])
//		{
//			if (vertexLine[i][4])
//			{
//				if(pt.x > minX && pt.x < maxX && pt.y == minY)
//			}
//		}
//		else if (vertexLine[i][0] && vertexLine[i][3])
//		{
//			if (pt.x > polyVertexCont[i].x)
//				pt.x = polyVertexCont[i].x;
//		}
//		else if (vertexLine[i][0] && vertexLine[i][1])
//		{
//			if (pt.x > polyVertexCont[i].x)
//				pt.x = polyVertexCont[i].x;
//		}
//		else if (vertexLine[i][1] && vertexLine[i][2])
//		{
//			if (pt.x > polyVertexCont[i].x)
//				pt.x = polyVertexCont[i].x;
//		}
//	}
//}
//
//
//void movingPoint::isVertexStart(POINT& pt, int movState,
//	std::vector<POINT>& polyVertexCont, bool vertexLine[][5])
//{
//	//if (poolSz) return;
//	if (movPtCont.size()) return;
//
//	for (unsigned int i = 0; i < polyVertexCont.size(); i++)
//	{
//		if ((pt.x == polyVertexCont[i].x && pt.y == polyVertexCont[i].y) &&
//			!vertexLine[i][movState])
//		{
//			pushMovPtPool(pt);
//		}
//	}
//}
//
//
//void movingPoint::mergePoly(std::vector<POINT>& polyVertexCont,
//	bool vertexLine[][5])
//{
//	std::vector<int> removeVertex;
//	//unsigned int contSz = polyVertexCont.size();
//	// poolSz 개수에 따른 조건문 필요?
//	for (unsigned int i = 0; i < polyVertexCont.size(); i++)
//	{
//		int isInFlag = 0;
//		unsigned int movPtContSz = movPtCont.size();
//		for (int j = 1; j < movPtContSz - 2; j++)
//		{
//			long long minX = min(movPtCont[j].x, movPtCont[(j + 1) % movPtContSz].x);
//			long long maxX = max(movPtCont[j].x, movPtCont[(j + 1) % movPtContSz].x);
//			long long minY = min(movPtCont[j].y, movPtCont[(j + 1) % movPtContSz].y);
//			long long maxY = max(movPtCont[j].y, movPtCont[(j + 1) % movPtContSz].y);
//			if (polyVertexCont[i].x >= minX && polyVertexCont[i].x <= maxX)
//				isInFlag++;
//			else if (polyVertexCont[i].y >= minY && polyVertexCont[i].y <= maxY)
//				isInFlag++;
//		}
//		if (isInFlag >= 2)
//			removeVertex.push_back(i);
//	}


/*int pLSz = polyLineCont.size();
for (unsigned int i = 0; i < removeVertex.size(); i++)
{
I = removeVertex[i];

if (polyLineCont[I].polyState == 3)
{
if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 2)
{
if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
else if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 0)
{
if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
}
else if (polyLineCont[I].polyState == 0)
{
if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 3)
{
if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
else if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 1)
{
if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
}
else if (polyLineCont[I].polyState == 1)
{
if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 0)
{
if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
else if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 2)
{
if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
}
else if (polyLineCont[I].polyState == 2)
{
if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 1)
{
if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
else if (polyLineCont[(I + pLSz - 1) % pLSz].polyState == 3)
{
if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
std::reverse(movPtCont.begin(), movPtCont.end());
}
}

polyVertexCont.erase(polyVertexCont.begin() + I);
}*/


//
//	//for문 제거?
//	unsigned int I;
//	for (unsigned int i = 0; i < removeVertex.size(); i++)
//	{
//		I = removeVertex[i];
//		if (vertexLine[I][2] && vertexLine[I][3])
//		{
//			if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
//				(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
//				std::reverse(movPtCont.begin(), movPtCont.end());
//		}
//		else if (vertexLine[I][0] && vertexLine[I][3])
//		{
//			if (!((movPtCont[i].x <= movPtCont[movPtCont.size() - 1].x) &&
//				(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
//				std::reverse(movPtCont.begin(), movPtCont.end());
//		}
//		else if (vertexLine[I][0] && vertexLine[I][1])
//		{
//			if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
//				(movPtCont[i].y <= movPtCont[movPtCont.size() - 1].y)))
//				std::reverse(movPtCont.begin(), movPtCont.end());
//		}
//		else if (vertexLine[I][1] && vertexLine[I][2])
//		{
//			if (!((movPtCont[i].x >= movPtCont[movPtCont.size() - 1].x) &&
//				(movPtCont[i].y >= movPtCont[movPtCont.size() - 1].y)))
//				std::reverse(movPtCont.begin(), movPtCont.end());
//		}
//		polyVertexCont.erase(polyVertexCont.begin() + I);
//	}
//	for (unsigned int i = 0; i < movPtCont.size(); i++)
//	{
//		polyVertexCont.insert(polyVertexCont.begin() + I + i, movPtCont[i]);
//	}
//
//	collidState = false;
//	movPtCont.clear();
//	ptVector = { 0, 0 };
//}