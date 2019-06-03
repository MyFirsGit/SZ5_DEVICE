#include "stdafx.h"
#include "AlignmentPattern.h"
#include "QRcodeDef.h"
#include "QRErrorCode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AlignmentPattern::AlignmentPattern()
{

}

AlignmentPattern::~AlignmentPattern()
{

}

//У��ͼ�μ��
int AlignmentPattern::findAlignmentPattern(BYTE ** mybitmap, int mynWidth, int mynHeight, FinderPattern * finderPattern, int myDECIMAL_POINT) 
{
	DECIMAL_POINT=myDECIMAL_POINT;
	bitmap=mybitmap;
	nWidth=mynWidth;
	nHeight=mynHeight;
	m_finderPattern=finderPattern;
	getLogicalCenter(m_finderPattern);
	logicalDistance = logicalCenters[1][0].x - logicalCenters[0][0].x;
	return getCenter();
}

//get logical center coordinates of each alignment patterns
void AlignmentPattern::getLogicalCenter(FinderPattern * finderPattern)
{
	//get logical center coordinates of each alignment patterns
	int version = finderPattern->m_version;

	int logicalSeedsCnt;
	logicalSeedsCnt=QR_VersonInfo[version].ncAlignPoint+1;
	int * logicalSeeds=new int[logicalSeedsCnt];
	logicalSeeds[0]=6;
	for(int i=1;i<logicalSeedsCnt;i++)
		logicalSeeds[i]=QR_VersonInfo[version].nAlignPoint[i-1];
	
	logicalCenters = new Point *[logicalSeedsCnt];
	for(int i=0;i<logicalSeedsCnt;i++)
		logicalCenters[i]=new Point[logicalSeedsCnt];
	
	//create real relative coordinates
	for (int col = 0; col < logicalSeedsCnt; col++)
	{
		for (int row = 0; row < logicalSeedsCnt; row++)
		{
			logicalCenters[row][col] = Point(logicalSeeds[row], logicalSeeds[col]);
		}
	}
	logicalCentersCnt=logicalSeedsCnt;	
}

int AlignmentPattern::getCenter() 
{
	int moduleSize = m_finderPattern->m_moduleSize[UL];

	int * angle=m_finderPattern->m_sincos;
	Axis axis = Axis(angle, moduleSize, DECIMAL_POINT);
	
	m_centersCnt = logicalCentersCnt;
	m_centers = new Point *[m_centersCnt];
	for(int i=0;i<m_centersCnt;i++)
		m_centers[i]=new Point[m_centersCnt];
	//Point[][] centers = new Point[m_centersCnt][m_centersCnt];
	
	axis.setOrigin(m_finderPattern->m_center[UL]);
	m_centers[0][0] = axis.translate(3, 3);

	axis.setOrigin(m_finderPattern->m_center[UR]);
	m_centers[m_centersCnt - 1][0] = axis.translate(-3, 3);

	axis.setOrigin(m_finderPattern->m_center[DL]);
	m_centers[0][m_centersCnt - 1] = axis.translate(3, -3);

	Point tmpPoint=m_centers[0][0];

	for (int y = 0; y < m_centersCnt; y++)
	{
		for (int x = 0; x < m_centersCnt; x++)
		{
			if ((x==0 && y==0) || (x==0 && y==m_centersCnt-1) || (x==m_centersCnt-1 && y==0))
			{
				continue;
			}
			Point target;
			if (y == 0)
			{
				if (x > 0 && x < m_centersCnt - 1)
				{
					target = axis.translate(m_centers[x-1][y], logicalCenters[x][y].x-logicalCenters[x-1][y].x, 0);
					m_centers[x][y] = Point(target.x, target.y);
				}
			}
			else if (x == 0)
			{
				if (y > 0 && y < m_centersCnt - 1)
				{
					target = axis.translate(m_centers[x][y-1], 0, logicalCenters[x][y].y-logicalCenters[x][y-1].y);
					m_centers[x][y] = Point(target.x, target.y);
				}
			}
			else
			{
				Point t1 = axis.translate(m_centers[x-1][y], logicalCenters[x][y].x-logicalCenters[x-1][y].x, 0);
				Point t2 = axis.translate(m_centers[x][y-1], 0, logicalCenters[x][y].y-logicalCenters[x][y-1].y);
				m_centers[x][y] = Point((t1.x+t2.x)/2, (t1.y + t2.y)/2+1);
			}
			if (m_finderPattern->m_version > 1)
			{
				Point precisionCenter;
				int iRet = getPrecisionCenter(m_centers[x][y],precisionCenter);
				if (iRet != QRCODE_OK)
				{
					return iRet;
				}

				int dx = precisionCenter.x - m_centers[x][y].x;
				int dy = precisionCenter.y - m_centers[x][y].y;
				
				m_centers[x][y] = precisionCenter;
			}
			tmpPoint=m_centers[x][y];
		}
	}
	return QRCODE_OK;
}

int AlignmentPattern::getPrecisionCenter(Point targetPoint,Point &PrecisionCenter) 
{
	// find nearest dark point and update it as new rough center point 
	// when original rough center points light point 
	int tx = targetPoint.x, ty = targetPoint.y;
	if ((tx < 0 || ty < 0) || (tx > nWidth - 1 || ty > nHeight -1))
	{
		return QRCODE_OUT_OF_IMAGE;
	}
		
	if (bitmap[targetPoint.x][targetPoint.y] == 0)
	{
		int scope = 0;
		bool found = false;
		while (!found) {
			scope++;
			for (int dy = scope; dy > -scope; dy--)
			{
				for (int dx = scope; dx > -scope; dx--)
				{
					int x = targetPoint.x + dx;
					int y = targetPoint.y + dy;
					if ((x < 0 || y < 0) || (x > nWidth - 1 || y > nHeight -1))
					{
						return QRCODE_OUT_OF_IMAGE;						
					}
					if (bitmap[x][y] == 1)
					{
						targetPoint = Point(targetPoint.x + dx,targetPoint.y + dy);
						found = true;
					}
				}
			}
		}
	}
	int x, lx, rx, y, uy, dy;
	x = lx = rx = targetPoint.x;
	y = uy = dy = targetPoint.y;

	while (lx >= 1 && targetPointOnTheCorner(lx, y, lx - 1, y) != QRCODE_OK) lx--;
	while (rx < nWidth-1 && targetPointOnTheCorner(rx, y, rx + 1, y) != QRCODE_OK) rx++;
	while (uy >= 1 && targetPointOnTheCorner(x, uy, x, uy - 1) != QRCODE_OK) uy--;
	while (dy < nHeight-1 && targetPointOnTheCorner(x, dy, x, dy + 1) != QRCODE_OK) dy++;
	
	PrecisionCenter.set((lx + rx + 1) / 2, (uy + dy + 1) / 2);
	return QRCODE_OK;
}

int AlignmentPattern::targetPointOnTheCorner(int x, int y, int nx, int ny)
{
	if( x < 0 || y < 0 || nx < 0 || ny < 0 || x > nWidth || y > nHeight || nx > nWidth || ny > nHeight)
	{
		return QRCODE_OUT_OF_IMAGE;
	}
	else 
	{
		if (bitmap[x][y] == 0&& bitmap[nx][ny] == 1)
		{
			return QRCODE_OK;
		}
	}
	return QRCODE_OUT_OF_IMAGE;
}