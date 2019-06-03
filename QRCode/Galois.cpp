#include "stdafx.h"
#include "Galois.h"

Galois::Galois()
{
	initGaloisTable();
}

Galois::~Galois()
{
}
/*
* ������ָ���໥�任���ݵ�����
*/
void Galois::initGaloisTable()
{
	//logTbl[0] = 0;
	int d = 1;
	for(int i = 0; i < 255; i++) 
	{
		expTbl[i] = expTbl[255 + i] = d;
		logTbl[d] = i;
		d <<= 1;
		if((d & 0x100) != 0) 
		{
			d=(d^POLYNOMIAL)&0xff;
		}
	}
}

/**
 *ָ���������ı任
 *
 * @param a int
 * @return int
 */
int Galois::toExp(int a)
{
	return expTbl[a];
}

/**
 * ������ָ���ı任
 *
 * @param a int
 * @return int
 */
int Galois::toLog(int a)
{
	return logTbl[a];
}

/**
 * �������λ��index
 *
 * @param length int,���ݳ���
 * @param a int,����λ������
 * @return int,����λ��index
 */
int Galois::toPos(int length, int a)
{
	return length - 1 - logTbl[a];
}

/**
 * �˷�
 *
 * @param a int
 * @param b int
 * @return int = a * b
 */
int Galois::mul(int a, int b)
{
	return (a == 0 || b == 0)? 0 : expTbl[logTbl[a] + logTbl[b]];
}

/**
 * ����ָ��
 *
 * @param a int
 * @param b int
 * @return int = a * ��^b
 */
int Galois::mulExp(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] + b];
}

/**
 * ����
 *
 * @param a int
 * @param b int
 * @return int = a / b
 */
int Galois::div(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] - logTbl[b] + 255];
}

/**
 * ����ָ��
 *
 * @param a int
 * @param b int
 * @return int = a / ��^b
 */
int Galois::divExp(int a, int b)
{
	return (a == 0)? 0 : expTbl[logTbl[a] - b + 255];
}

/**
 * ����
 *
 * @param a int
 * @return int = 1/a
 */
int Galois::inv(int a)
{
	return expTbl[255 - logTbl[a]];
}

/**
 * ����ʽ�˷�
 *
 * @param seki int[]
 * 		seki = a * b
 * @param a int[]
 * @param b int[]
 */
void Galois::mulPoly(int * seki, int * a, int * b, int sekilength, int alength, int blength)
{
	ZeroMemory(seki, sekilength);
	for(int ia = 0; ia < alength; ia++) 
	{
		if(a[ia] != 0) 
		{
			int loga = logTbl[a[ia]];
			int ib2 = blength < (sekilength-ia) ? blength : (sekilength-ia);
			for(int ib = 0; ib < ib2; ib++) 
			{
				if(b[ib] != 0) 
				{
					seki[ia + ib] ^= expTbl[loga + logTbl[b[ib]]];	// = a[ia] * b[ib]
				}
			}
		}
	}
}

/**
 * У�����
 * @param data int[]
 *		������������
 * @param length int
 *		���ݳ���
 * @param syn int[]
 *		(x - ��^0) (x - ��^1) (x - ��^2) ...��У��
 * @return boolean
 *		true: ����У��Ϊ0��У�������Ϊ0
 */
bool Galois::calcSyndrome(BYTE * data, int length, int * syn, int synlength)
{
	int hasErr = 0;
	for(int i = 0; i < synlength;  i++) 
	{
		int wk = 0;
		for(int idx = 0; idx < length; idx++) 
		{
			 wk = data[idx] ^ ((wk == 0)? 0 : expTbl[logTbl[wk] + i]);		// wk = data + wk * ��^i
		}
		syn[i] = wk;
		hasErr |= wk;
	}
	return hasErr == 0;
}
