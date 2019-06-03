#include "StdAfx.h"
#include "sha-1.h"

#if!defined(BIG_ENDIAN)
#define	BIG_ENDIAN	1
#endif

#if!defined(LITTLE_ENDIAN)
#define LITTLE_ENDIAN	2
#endif

#if!defined(BYTE_ORDER)
#if defined(WORDS_BIGENDIAN)
#define BYTE_ORDER BIG_ENDIAN
#else
#define BYTE_ORDER LITTLE_ENDIAN
#endif
#endif

/*Implemented from SHA-1 document-The Secure Hash Algorithm
*/

#define INIT_DATA_h0 (unsigned long)0x67452301L
#define INIT_DATA_h1 (unsigned long)0xefcdab89L
#define INIT_DATA_h2 (unsigned long)0x98badcfeL
#define INIT_DATA_h3 (unsigned long)0x10325476L
#define INIT_DATA_h4 (unsigned long)0xc3d2e1f0L

#define K_00_19 0x5a827999L
#define K_20_39 0x6ed9eba1L
#define K_40_59 0x8f1bbcdcL
#define K_60_79 0xca62c1d6L

static void sha1_block(SHA1_CTX *c,register unsigned long *p);

void SHA1Init(SHA1_CTX *c)
{
    c->h0 = INIT_DATA_h0;
    c->h1 = INIT_DATA_h1;
    c->h2 = INIT_DATA_h2;
    c->h3 = INIT_DATA_h3;
    c->h4 = INIT_DATA_h4;
    c->Nl = 0;
    c->Nh = 0;
    c->num = 0;
}

void SHA1Update(SHA1_CTX *c,register const unsigned char* data,unsigned long len)
{
    register ULONG *p;
    int ew,ec,sw,sc;
    ULONG l;

    if(len == 0) return;

    l=(c->Nl + (len<<3))& 0xffffffff;
    if(l<c->Nl)/*overflow*/
        c->Nh++;
    c->Nh+=(len>>29);
    c->Nl = l;

    if(c->num!=0)
    {
        p=c->data;
        sw=c->num>>2;
        sc=c->num&0x03;

        if((c->num+len)>=SHA_CBLOCK)
        {
            l=p[sw];
            p_c2nl(data,l,sc);
            p[sw++]=l;
            for(;sw<SHA_LBLOCK;sw++)
            {
                c2nl(data,l);
                p[sw]=l;
            }
            len-=(SHA_CBLOCK-c->num);

            sha1_block(c,p);
            c->num=0;
            /*drop through and do the rest*/
        }
        else
        {
            int ew,ec;

            c->num+=(int)len;
            if((sc+len)<4)/*ugly,add char's to a word*/
            {
                l=p[sw];
                p_c2nl_p(data,l,sc,len);
                p[sw]=l;
            }
            else
            {
                ew=(c->num>>2);
                ec=(c->num&0x03);
                l=p[sw];
                p_c2nl(data,l,sc);
                p[sw++]=l;
                for(;sw<ew;sw++)
                {c2nl(data,l);p[sw]=l;}
                if(ec)
                {
                    c2nl_p(data,l,ec);
                    p[sw]=l;
                }
            }
            return;
        }
    }
    /*we now can process the input data in blocks of SHA_CBLOCK
    *chars and save the leftovers to c->data.*/
    p=c->data;
    while(len>=SHA_CBLOCK)
    {
#if defined(BYTE_ORDER)&&(defined(BIG_ENDIAN)||defined(LITTLE_ENDIAN))
        memcpy(p,data,SHA_CBLOCK);
        data+=SHA_CBLOCK;
#if BYTE_ORDER == LITTLE_ENDIAN
        for(sw=(SHA_LBLOCK/4);sw;sw--)
        {
            Endian_Reverse32(p[0]);
            Endian_Reverse32(p[1]);
            Endian_Reverse32(p[2]);
            Endian_Reverse32(p[3]);
            p+=4;
        }
#endif
#else
        for(sw=(SHA_BLOCK/4);sw;sw--)
        {
            c2nl(data,l);*(p++)=l;
            c2nl(data,l);*(p++)=l;
            c2nl(data,l);*(p++)=l;
            c2nl(data,l);*(p++)=l;
        }
#endif
        p=c->data;
        sha1_block(c,p);	
        len-=SHA_CBLOCK;
    }
    ec=(int)len;
    c->num=ec;
    ew=(ec>>2);
    ec&=0x03;

    for(sw=0;sw<ew;sw++)
    {c2nl(data,l);p[sw]=l;}
    c2nl_p(data,l,ec);
    p[sw]=l;
}

static void sha1_block(SHA1_CTX *c,register unsigned long *X)
{
    register ULONG A,B,C,D,E,T;

    A=c->h0;
    B=c->h1;
    C=c->h2;
    D=c->h3;
    E=c->h4;

    BODY_00_15(0,A,B,C,D,E,T);
    BODY_00_15(1,T,A,B,C,D,E);
    BODY_00_15(2,E,T,A,B,C,D);
    BODY_00_15(3,D,E,T,A,B,C);
    BODY_00_15(4,C,D,E,T,A,B);
    BODY_00_15(5,B,C,D,E,T,A);
    BODY_00_15(6,A,B,C,D,E,T);
    BODY_00_15(7,T,A,B,C,D,E);
    BODY_00_15(8,E,T,A,B,C,D);
    BODY_00_15(9,D,E,T,A,B,C);
    BODY_00_15(10,C,D,E,T,A,B);
    BODY_00_15(11,B,C,D,E,T,A);
    BODY_00_15(12,A,B,C,D,E,T);
    BODY_00_15(13,T,A,B,C,D,E);
    BODY_00_15(14,E,T,A,B,C,D);	
    BODY_00_15(15,D,E,T,A,B,C);	
    BODY_16_19(16,C,D,E,T,A,B);
    BODY_16_19(17,B,C,D,E,T,A);
    BODY_16_19(18,A,B,C,D,E,T);
    BODY_16_19(19,T,A,B,C,D,E);

    BODY_20_39(20,E,T,A,B,C,D);
    BODY_20_39(21,D,E,T,A,B,C);
    BODY_20_39(22,C,D,E,T,A,B);
    BODY_20_39(23,B,C,D,E,T,A);
    BODY_20_39(24,A,B,C,D,E,T);
    BODY_20_39(25,T,A,B,C,D,E);
    BODY_20_39(26,E,T,A,B,C,D);
    BODY_20_39(27,D,E,T,A,B,C);
    BODY_20_39(28,C,D,E,T,A,B);
    BODY_20_39(29,B,C,D,E,T,A);
    BODY_20_39(30,A,B,C,D,E,T);
    BODY_20_39(31,T,A,B,C,D,E);
    BODY_20_39(32,E,T,A,B,C,D);
    BODY_20_39(33,D,E,T,A,B,C);
    BODY_20_39(34,C,D,E,T,A,B);
    BODY_20_39(35,B,C,D,E,T,A);
    BODY_20_39(36,A,B,C,D,E,T);
    BODY_20_39(37,T,A,B,C,D,E);
    BODY_20_39(38,E,T,A,B,C,D);
    BODY_20_39(39,D,E,T,A,B,C);

    BODY_40_59(40,C,D,E,T,A,B);
    BODY_40_59(41,B,C,D,E,T,A);
    BODY_40_59(42,A,B,C,D,E,T);
    BODY_40_59(43,T,A,B,C,D,E);
    BODY_40_59(44,E,T,A,B,C,D);
    BODY_40_59(45,D,E,T,A,B,C);
    BODY_40_59(46,C,D,E,T,A,B);
    BODY_40_59(47,B,C,D,E,T,A);
    BODY_40_59(48,A,B,C,D,E,T);
    BODY_40_59(49,T,A,B,C,D,E);
    BODY_40_59(50,E,T,A,B,C,D);
    BODY_40_59(51,D,E,T,A,B,C);
    BODY_40_59(52,C,D,E,T,A,B);
    BODY_40_59(53,B,C,D,E,T,A);
    BODY_40_59(54,A,B,C,D,E,T);
    BODY_40_59(55,T,A,B,C,D,E);
    BODY_40_59(56,E,T,A,B,C,D);
    BODY_40_59(57,D,E,T,A,B,C);
    BODY_40_59(58,C,D,E,T,A,B);
    BODY_40_59(59,B,C,D,E,T,A);

    BODY_60_79(60,A,B,C,D,E,T);
    BODY_60_79(61,T,A,B,C,D,E);
    BODY_60_79(62,E,T,A,B,C,D);
    BODY_60_79(63,D,E,T,A,B,C);
    BODY_60_79(64,C,D,E,T,A,B);
    BODY_60_79(65,B,C,D,E,T,A);
    BODY_60_79(66,A,B,C,D,E,T);
    BODY_60_79(67,T,A,B,C,D,E);
    BODY_60_79(68,E,T,A,B,C,D);
    BODY_60_79(69,D,E,T,A,B,C);
    BODY_60_79(70,C,D,E,T,A,B);
    BODY_60_79(71,B,C,D,E,T,A);
    BODY_60_79(72,A,B,C,D,E,T);
    BODY_60_79(73,T,A,B,C,D,E);
    BODY_60_79(74,E,T,A,B,C,D);	
    BODY_60_79(75,D,E,T,A,B,C);	
    BODY_60_79(76,C,D,E,T,A,B);
    BODY_60_79(77,B,C,D,E,T,A);
    BODY_60_79(78,A,B,C,D,E,T);
    BODY_60_79(79,T,A,B,C,D,E);

    c->h0=(c->h0+E)&0xffffffff;
    c->h1=(c->h1+T)&0xffffffff;
    c->h2=(c->h2+A)&0xffffffff;
    c->h3=(c->h3+B)&0xffffffff;
    c->h4=(c->h4+C)&0xffffffff;
}

void SHA1Final(unsigned char *md,SHA1_CTX *c)
{
    register int i,j;
    register ULONG l;
    register ULONG *p;
    static unsigned char end[4]={0x80,0x00,0x00,0x00};
    unsigned char *cp=end;

    /*c->num should definitly have room for at least one more byte.*/
    p=c->data;
    j=c->num;
    i=j>>2;
#ifdef PURIFY
    if((j&0x03)==0)p[i]=0;
#endif
    l=p[i];
    p_c2nl(cp,l,j&0x03);
    p[i]=l;
    i++;
    /*i is the next 'undefined word'*/
    if(c->num >= SHA_LAST_BLOCK)
    {
        for(;i<SHA_LBLOCK;i++)
            p[i]=0;
        sha1_block(c,p);
        i=0;
    }
    for(;i<(SHA_LBLOCK-2);i++)
        p[i]=0;
    p[SHA_LBLOCK-2]=c->Nh;
    p[SHA_LBLOCK-1]=c->Nl;
    sha1_block(c,p);
    cp=md;
    l=c->h0;nl2c(l,cp);
    l=c->h1;nl2c(l,cp);
    l=c->h2;nl2c(l,cp);
    l=c->h3;nl2c(l,cp);
    l=c->h4;nl2c(l,cp);

    /*clear stuff,sha1_block may be leaving some stuff on the stack
    *but I'm not worried:-)*/
    c->num=0;
    /*memset((char*)&c,0,sizeof(c));*/
}

#define SHA_DIGEST_LENGTH 20

//////////////////////////////////////////////////////////////////////////
/**
@brief      TAC�����õ�20�ֽڵ�ת������

@param      (i)unsigned char*d      �������ݵ��׵�ַ
@param      (i)unsigned long n      �������ݳ���
@param      (o)unsigned char* md    ����������̶�20�ֽڣ��ⲿ���ռ䣩

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void SHA1(unsigned char*d,unsigned long n,unsigned char* md)
{
    SHA1_CTX c;

    SHA1Init(&c);
    SHA1Update(&c,d,n);
    SHA1Final(md,&c);
    memset(&c,0,sizeof(c));
}