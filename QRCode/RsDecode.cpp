#include "stdafx.h"
#include "RsDecode.h"

/*
���鳤��
sg0===npar
sg1===npar
sigma===npar/2+2
omega===npar/2+1
syn===npar
*/

RsDecode::RsDecode()
{
	galois = Galois();
}

RsDecode::~RsDecode()
{

}

RsDecode::RsDecode(int npar)
{
	galois = Galois();
	this->npar = npar;
}

/**
 * Modified Berlekamp-Massey
 *
 * @param sigma int[]
 *		��(z)��{�����С����npar/2 + 2�����I�򤬱�Ҫ
 * 		��0,��1,��2, ... ��<jisu>
 * @param omega int[]
 *		��(z)��{�����С����npar/2 + 1�����I�򤬱�Ҫ
 *		��0,��1,��2, ... ��<jisu-1>
 * @param syn int[]
 *		����ɥ�`������
 * 		s0,s1,s2, ... s<npar-1>
 * @return int
 * 		>= 0: �Ҥδ���
 * 		< 0: ����`
 */
int RsDecode::calcSigmaMBM(int * sigma, int * omega, int * syn)
{
	int * sg0 = new int[npar];
	int * sg1 = new int[npar];
	int i;
	for(i=0;i<npar;i++)
	{
		sg0[i]=0;sg1[i]=0;
	}
	sg0[1] = 1;
	sg1[0] = 1;
	int jisu0 = 1;
	int jisu1 = 0;
	int m = -1;

	for(int n = 0; n < npar; n++) {
		// �Єeʽ��Ӌ��
		int d = syn[n];
		for(int i = 1; i <= jisu1; i++) {
			d ^= galois.mul(sg1[i], syn[n - i]);
		}
		if(d != 0) {
			int logd = galois.toLog(d);
			int * wk = new int[npar];
			for(int i=0;i<npar;i++)
				wk[i]=0;
			for(int i = 0; i <= n; i++) {
				wk[i] = sg1[i] ^ galois.mulExp(sg0[i], logd);
			}
			int js = n - m;
			if(js > jisu1) {
				m = n - jisu1;
				jisu1 = js;
				if(jisu1 > npar / 2) {
					return -1;				// �Ҥδ�����npar / 2�򳬤����饨��`
				}
				for(int i = 0; i <= jisu0; i++) {
					sg0[i] = galois.divExp(sg1[i], logd);
				}
				jisu0 = jisu1;
			}
			sg1 = wk;
		}

		int cnt=npar-1<jisu0 ? npar-1 : jisu0;
		for(int i=cnt-1;i>=0;i--)
			sg0[i+1]=sg0[i];

		sg0[0] = 0;
		jisu0++;
	}
	galois.mulPoly(omega, sg1, syn, npar/2+1, npar, npar);

	int cnt=npar<npar/2+2 ? npar : npar/2+2;
	for(i=0;i<cnt;i++)
		sigma[i]=sg1[i];

	return jisu1;
}

/**
 * ������̽���ˤ���`��λ�ä�����
 *		��(z) = 0�ν��̽������
 *		��������̽���ϥǩ`���L���ڤν�Τߤ�
 *		jisu���ν⤬Ҋ�Ĥ���ʤ���С�����`�Ȥ���
 * @param pos int[]
 * 		�`��λ�ø�{�����С�jisu�����I�򤬱�Ҫ
 * @param n int
 * 		�ǩ`���L
 * @param jisu int
 * 		�Ҥδ���
 * @param sigma int[]
 * 		��0,��1,��2, ... ��<jisu>
 * @return int
 *		0: �����K��
 *		< 0: ����`
 */
int RsDecode::chienSearch(int * pos, int n, int jisu, int * sigma)
{
	/*
	 * ��(z) = (1-��^i*z)(1-��^j*z)(1-��^k*z)
	 *       = 1 + ��1z + ��2z^2 +...
	 * ��1 = ��^i + ��^j + ��^k
	 * ��ӛ�����|�����ä��ơ��ץ����m��
	 * last = ��1���顢Ҋ�Ĥ������Ρ��������Ƥ������Ȥˤ�ꡢ����ν��last�Ȥʤ�
	 */
	int last = sigma[1];

	if(jisu == 1) {
		// ������1�ʤ�С�last�����ν�Ǥ���
		if(galois.toLog(last) >= n) {
			return RS_CORRECT_ERROR;	// ������ʤΤǥ���`
		}
		pos[0] = last;
		return 0;
	}

	int posIdx = jisu - 1;		// �`��λ�ø�{�å���ǥå���
	for(int i = 0; i < n; i++) {
		/*
		 * ��(z)��Ӌ��
		 * w ��1(0�\���)�˳��ڻ������ᡢ�Ф���<1..jisu>�����
		 * z = 1/��^i = ��^I�Ȥ����
		 * ��(z) = 1 + ��1��^I + ��2(��^I)^2 + ��3(��^I)^3 + ... + ��<jisu>/(��^I)^<jisu>
		 *       = 1 + ��1��^I + ��2��^(I*2) + ��3��^(I*3) + ... + ��<jisu>��^(I*<jisu>)
		 */
		int z = 255 - i;					// z = 1/��^i�Υ�����`
		int wk = 1;
		for(int j = 1; j <= jisu; j++) {
			wk ^= galois.mulExp(sigma[j], (z * j) % 255);
		}
		if(wk == 0) {
			int pv = galois.toExp(i);		// ��(z) = 0�ν�
			last ^=  pv;					// last�����Ҋ�Ĥ��ä��������
			pos[posIdx--] = pv;
			if(posIdx == 0) {
				// �Ф꤬һ�Ĥʤ�С�last�����ν�Ǥ���
				if(galois.toLog(last) >= n) {
					return RS_CORRECT_ERROR;	// ����ν⤬������ʤΤǥ���`
				}
				pos[0] = last;
				return 0;
			}
		}
	}
	// ̽���ˤ��ǩ`���L���ڤˡ�jisu���ν⤬Ҋ�Ĥ���ʤ��ä�
	return RS_CORRECT_ERROR;
}

/**
 * Forney�����`��ӆ�����Ф�
 *		��(z) = (1-��^i*z)(1-��^j*z)(1-��^k*z)
 *		��'(z) = ��^i * (1-��^j*z)(1-��^k*z)...
 *			   + ��^j * (1-��^i*z)(1-��^k*z)...
 *			   + ��^k * (1-��^i*z)(1-��^j*z)...
 *		��(z) = (E^i/(1-��^i*z) + E^j/(1-��^j*z) + ...) * ��(z)
 *			  = E^i*(1-��^j*z)(1-��^k*z)...
 *			  + E^j*(1-��^i*z)(1-��^k*z)...
 *			  + E^k*(1-��^i*z)(1-��^j*z)...
 *		�� E^i = ��^i * ��(z) / ��'(z)
 * @param data int[]
 *		�����ǩ`������
 * @param length int
 *		�����ǩ`���L��
 * @param jisu int
 *		�Ҥδ���
 * @param pos int[]
 *		�`��λ������
 * @param sigma int[]
 *		��0,��1,��2, ... ��<jisu>
 * @param omega int[]
 *		��0,��1,��2, ... ��<jisu-1>
 */
void RsDecode::doForney(BYTE * data, int length, int jisu, int * pos, int * sigma, int * omega)
{
	int i,j;
	for(i = 0; i < jisu; i++)
	{
		int ps = pos[i];
		int zlog = 255 - galois.toLog(ps);					// z�Υ�����`

		// ��(z)��Ӌ��
		int ov = omega[0];
		for(j = 1; j < jisu; j++)
		{
			ov ^= galois.mulExp(omega[j], (zlog * j) % 255);		// ov += ��i * z^j
		}

		// ��'(z)�΂���Ӌ��(��(z)����ʽ��΢��)
		int dv = sigma[1];
		for(j = 2; j < jisu; j += 2)
		{
			dv ^= galois.mulExp(sigma[j + 1], (zlog * j) % 255);	// dv += ��<j+1> * z^j
		}

		/*
		 * �`��ӆ�� E^i = ��^i * ��(z) / ��'(z)
		 * �`��λ�äι���ϥ�����̽���ΤȤ��˱��^����Ƥ���Τǡ�
		 * �����Ǥϥ����å����ʤ�
		 */
		data[galois.toPos(length, ps)] ^= galois.mul(ps, galois.div(ov, dv));
	}
}

/**
 * RS���`�ɤΥǥ��`��
 *
 * @param data int[]
 *		�����ǩ`������
 * @param length int
 * 		�ѥ�ƥ��򺬤᤿�ǩ`���L
 * @param noCorrect boolean
 * 		�����å��Τߤ�ӆ�����Ф�ʤ�
 * @return int
 * 		0: ����`�ʤ�
 * 		> 0: ���ꂎ�����`���ӆ������
 * 		< 0: ӆ������
 */
int RsDecode::decode(BYTE * data, int length, bool noCorrect)
{
	if(length < npar || length > 255) {
		return RS_PERM_ERROR;
	}
	// ����ɥ�`���Ӌ��
	int * syn = new int[npar];
	for(int i=0;i<npar;i++)syn[i]=0;

	if(galois.calcSyndrome(data, length, syn, npar)) {
		return 0;		// ����`�o��
	}
	// ����ɥ�`����ҤȦؤ�����
	int * sigma = new int[npar/2+2];
	for(int i=0;i<npar/2+2;i++)sigma[i]=0;
	int * omega = new int[npar/2+1];
	for(int i=0;i<npar/2+1;i++)omega[i]=0;
	int jisu = calcSigmaMBM(sigma, omega, syn);
	if(jisu <= 0) {
		return RS_CORRECT_ERROR;
	}
	// ������̽���ˤ���`��λ�ä�����
	int * pos = new int[jisu];
	for(int i=0;i<jisu;i++)pos[i]=0;

	int r = chienSearch(pos, length, jisu, sigma);
	if(r < 0) {
		return r;
	}
	if(!noCorrect) {
		// �`��ӆ��
		doForney(data, length, jisu, pos, sigma, omega);
	}
	return jisu;
}

int RsDecode::decode(BYTE * data, int length)
{
	return decode(data, length, false);
}
