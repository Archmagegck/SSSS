// bytefield.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"

#include <sys/unistd.h>
typedef unsigned long long ticks;

inline unsigned long long GetNTime()
{
     unsigned a, d;
     asm("cpuid");
     asm volatile("rdtsc" : "=a" (a), "=d" (d));

     return (((ticks)a) | (((ticks)d) << 32));

}


/*
�����ֽ�����������ܷ���������һ���ֽ�һ���ֽڵļ���Ӱ��
ȱ�����������������Ļ�ʱ�����������ֵ����ô�����ȱһ�˵�����£����Գ������ȱ��Ӱ�ӡ�
���������Ϣ���ܵĻ������ֳ��Կ�����Ч��
���Թ�������ϢΪ��key|msg����
*/

typedef unsigned char		word8;	
typedef unsigned short		word16;	
typedef unsigned long		word32;

word8 Logtable[256] = {
  0,   0,  25,   1,  50,   2,  26, 198,  75, 199,  27, 104,  51, 238, 223,   3, 
100,   4, 224,  14,  52, 141, 129, 239,  76, 113,   8, 200, 248, 105,  28, 193, 
125, 194,  29, 181, 249, 185,  39, 106,  77, 228, 166, 114, 154, 201,   9, 120, 
101,  47, 138,   5,  33,  15, 225,  36,  18, 240, 130,  69,  53, 147, 218, 142, 
150, 143, 219, 189,  54, 208, 206, 148,  19,  92, 210, 241,  64,  70, 131,  56, 
102, 221, 253,  48, 191,   6, 139,  98, 179,  37, 226, 152,  34, 136, 145,  16, 
126, 110,  72, 195, 163, 182,  30,  66,  58, 107,  40,  84, 250, 133,  61, 186, 
 43, 121,  10,  21, 155, 159,  94, 202,  78, 212, 172, 229, 243, 115, 167,  87, 
175,  88, 168,  80, 244, 234, 214, 116,  79, 174, 233, 213, 231, 230, 173, 232, 
 44, 215, 117, 122, 235,  22,  11, 245,  89, 203,  95, 176, 156, 169,  81, 160, 
127,  12, 246, 111,  23, 196,  73, 236, 216,  67,  31,  45, 164, 118, 123, 183, 
204, 187,  62,  90, 251,  96, 177, 134,  59,  82, 161, 108, 170,  85,  41, 157, 
151, 178, 135, 144,  97, 190, 220, 252, 188, 149, 207, 205,  55,  63,  91, 209, 
 83,  57, 132,  60,  65, 162, 109,  71,  20,  42, 158,  93,  86, 242, 211, 171, 
 68,  17, 146, 217,  35,  32,  46, 137, 180, 124, 184,  38, 119, 153, 227, 165, 
103,  74, 237, 222, 197,  49, 254,  24,  13,  99, 140, 128, 192, 247, 112,   7, 
};

word8 Alogtable[256] = {
  1,   3,   5,  15,  17,  51,  85, 255,  26,  46, 114, 150, 161, 248,  19,  53, 
 95, 225,  56,  72, 216, 115, 149, 164, 247,   2,   6,  10,  30,  34, 102, 170, 
229,  52,  92, 228,  55,  89, 235,  38, 106, 190, 217, 112, 144, 171, 230,  49, 
 83, 245,   4,  12,  20,  60,  68, 204,  79, 209, 104, 184, 211, 110, 178, 205, 
 76, 212, 103, 169, 224,  59,  77, 215,  98, 166, 241,   8,  24,  40, 120, 136, 
131, 158, 185, 208, 107, 189, 220, 127, 129, 152, 179, 206,  73, 219, 118, 154, 
181, 196,  87, 249,  16,  48,  80, 240,  11,  29,  39, 105, 187, 214,  97, 163, 
254,  25,  43, 125, 135, 146, 173, 236,  47, 113, 147, 174, 233,  32,  96, 160, 
251,  22,  58,  78, 210, 109, 183, 194,  93, 231,  50,  86, 250,  21,  63,  65, 
195,  94, 226,  61,  71, 201,  64, 192,  91, 237,  44, 116, 156, 191, 218, 117, 
159, 186, 213, 100, 172, 239,  42, 126, 130, 157, 188, 223, 122, 142, 137, 128, 
155, 182, 193,  88, 232,  35, 101, 175, 234,  37, 111, 177, 200,  67, 197,  84, 
252,  31,  33,  99, 165, 244,   7,   9,  27,  45, 119, 153, 176, 203,  70, 202, 
 69, 207,  74, 222, 121, 139, 134, 145, 168, 227,  62,  66, 198,  81, 243,  14, 
 18,  54,  90, 238,  41, 123, 141, 140, 143, 138, 133, 148, 167, 242,  13,  23, 
 57,  75, 221, 124, 132, 151, 162, 253,  28,  36, 108, 180, 199,  82, 246,   1, 
};

word8 Inv[256]= {
   0,   1, 141, 246, 203,  82, 123, 209, 232,  79,  41, 192, 176, 225, 229, 199,
 116, 180, 170,  75, 153,  43,  96,  95,  88,  63, 253, 204, 255,  64, 238, 178,
  58, 110,  90, 241,  85,  77, 168, 201, 193,  10, 152,  21,  48,  68, 162, 194,
  44,  69, 146, 108, 243,  57, 102,  66, 242,  53,  32, 111, 119, 187,  89,  25,
  29, 254,  55, 103,  45,  49, 245, 105, 167, 100, 171,  19,  84,  37, 233,   9,
 237,  92,   5, 202,  76,  36, 135, 191,  24,  62,  34, 240,  81, 236,  97,  23,
  22,  94, 175, 211,  73, 166,  54,  67, 244,  71, 145, 223,  51, 147,  33,  59,
 121, 183, 151, 133,  16, 181, 186,  60, 182, 112, 208,   6, 161, 250, 129, 130,
 131, 126, 127, 128, 150, 115, 190,  86, 155, 158, 149, 217, 247,   2, 185, 164,
 222, 106,  50, 109, 216, 138, 132, 114,  42,  20, 159, 136, 249, 220, 137, 154,
 251, 124,  46, 195, 143, 184, 101,  72,  38, 200,  18,  74, 206, 231, 210,  98,
  12, 224,  31, 239,  17, 117, 120, 113, 165, 142, 118,  61, 189, 188, 134,  87,
  11,  40,  47, 163, 218, 212, 228,  15, 169,  39,  83,   4,  27, 252, 172, 230,
 122,   7, 174,  99, 197, 219, 226, 234, 148, 139, 196, 213, 157, 248, 144, 107,
 177,  13, 214, 235, 198,  14, 207, 173,   8,  78, 215, 227,  93,  80,  30, 179,
  91,  35,  56,  52, 104,  70,   3, 140, 221, 156, 125, 160, 205,  26,  65,  28,
};


word8 mul(word8 a, word8 b) {
   /* multiply two elements of GF(2^m)
    * needed for MixColumn and InvMixColumn
    */
	if (a && b) return Alogtable[(Logtable[a] + Logtable[b])%255];
	else return 0;
};


word8 inv(word8 a) {
   /* invert of GF(2^m)   
    */
	 return Inv[a];
};

class Cbytefield
{
private:
	word8 a;
public:
	Cbytefield(word8 b){a=b;};
	Cbytefield(){};
	word8 value() {return a;};
	Cbytefield operator+ (Cbytefield b){return a^b.value();};
	Cbytefield operator- (Cbytefield b){return a^b.value();};
	Cbytefield operator* (Cbytefield b){return mul(a,b.value());};
	Cbytefield operator/ (Cbytefield b){return mul(a,inv(b.value()));};
	Cbytefield operator= (Cbytefield b){return a=b.value();};
	int operator== (Cbytefield b){return a==b.value();};
	int	operator< (Cbytefield b){return a<b.value();};
	Cbytefield invert() {return inv(a);};
	Cbytefield pow (Cbytefield b){	word8 c=1;
								for(int i=0;i<b.value();i++) c=mul(c,a); 
								return c;};	
};

class CbyteShare
{
private:
	Cbytefield toShareBytem;
	Cbytefield an[256],Ax[256],Ay[256];


	Cbytefield v[256][257];

	unsigned short m,n;
			
	
	void setv()
	{
		 int i,j;
		 
		 for(i=0;i<n;i++)
		 {	Cbytefield k(i+1);
			for(j=0;j<m;j++)
			{			
				v[i][j]=k.pow(j);
				
			}
		
		 }
/*
		 for(i=0;i<n;i++)
		{	printf("\n");
			for(j=0;j<m;j++)
				printf(" %3d",v[i][j].value());
		}
*/
		
	}


	
	void RandSetAx(){
						
						for(int i=0;i<m;i++) 
						{	word8 r=rand()%256;
							if(0==r)  {i--;continue;}
							Ax[i]=r; 
						}
						
					};

public:
	


	CbyteShare(unsigned short _m,unsigned short _n) {m=_m;n=_n;setv();};
	void SetShareByte(word8 bytemsg) {toShareBytem=bytemsg;	};

	void MakeShare();
	
	word8 RestorShareByte(unsigned char share[]);


};
long long timeMakeShare=0,timeRestore=0;

void  CbyteShare::MakeShare()
{
	int i,j,k;
/*	printf("\n Ax=");
	for(i=0;i<m;i++)
		printf(" %d",Ax[i].value());
*/
/*
	for(i=0;i<n;i++)
	{
		printf("\n");
		for(j=0;j<m;j++)
		{
			printf(" %3d",v[i][j].value()); 
		}
		
	}

*/		//printf("\n---share--------------------\n");
	long long time1 = GetNTime();

	RandSetAx();
	Ax[m-1]=toShareBytem; 

	for(i=0;i<n;i++)
	{
		v[i][m]=0;
		for(j=0;j<m;j++)
		{
			v[i][m]=v[i][m]+v[i][j]*Ax[j];
		}
		//v[i][j]=Ay[i]; //give v[i][m]=ay[i]

		//printf(" %3d ",v[i][m].value());
	}
	long long time2 = GetNTime();
	timeMakeShare+=time2-time1;

	
	//printf("\n-------share end------------");
	
}

word8 CbyteShare::RestorShareByte(unsigned char share[])
{

	short  i,j;
	
	Cbytefield rv[256][256],tmp,numhead,tmp2;  //tmp restorshare matrix
	
	//printf("\n --------------------------------------------");
	for( i=0;i<m;i++)
	{ 	// printf("\n");
		for( j=0;j<m+1;j++)
			{
				rv[i][j]=v[share[i]][j];
				//printf(" %3d",rv[i][j].value());
			}
		

	}
	//printf("\n --------------------------------------------");

	long long time1 = GetNTime();
	for(i=0;i<m-1;i++)
	{
		
		tmp=rv[i][i];

			for(j=i+1;j<m;j++)
			{			
				numhead=rv[j][i];	
				if(numhead==0) continue;
				tmp2=tmp/numhead;
				for(int k=i;k<m+1;k++)
				{	rv[j][k]=rv[j][k]* tmp2;
					
					rv[j][k]=rv[j][k]-rv[i][k];
					
				}
				
			}

	}
	
	long long time2 = GetNTime();
	timeRestore+=time2-time1;

	/*
	for(i=0;i<m;i++)
	{	printf("\n");
		for(j=0;j<m+1;j++)
		{
			printf(" %3d",rv[i][j].value());
		}
	}
	printf("\n--------------");
*/
/*
	for(i=m-1;i>0;i--)
	{	
		//rv[i][m]=rv[i][m]/rv[i][i];
		tmp=rv[i][i];

			for(j=i-1;j>=0;j--)
			{		
				numhead=rv[j][i];
  				if(numhead==0) continue;				
				tmp2=tmp/numhead;
				
				for(int k=j;k<=m+1;k++)
				{	
											
						
					rv[j][k]=rv[j][k]* tmp2;
					
					rv[j][k]=rv[j][k]-rv[i][k];
					
				}
				
			}

	}
*/
	/*
	for(i=0;i<m;i++)
	{	printf("\n");
		for(j=0;j<m+1;j++)
		{
			printf(" %3d",rv[i][j].value());
		}
	}
*/
/*	printf("\n");
	for(i=0;i<m;i++)
	{
	rv[i][m]=rv[i][m]/rv[i][i];
	printf(" %d,",rv[i][m].value());
	}
*/	
	
//	return rv[0][m].value();
	return (rv[m-1][m]/rv[m-1][m-1]).value();
}



int main(int argc, char* argv[])
{
	word16	 a,b,c;
	unsigned char share[256];
	unsigned long times,Times;
	
	
	

	
	int m=255,n=255,msg;
	
	scanf("%d%d%lu",&m,&n,&Times);
	printf("\n%d %d %lu\n",m,n,Times);
	//m=3,n=11;

	srand( (unsigned)time( NULL ) );
	for(a=0;a<n;a++) share[a]=a;
	
	for(a=0;a<3*n;a++) 
			{	word8 r=rand()%n;
				word8 tmp=share[0]; share[0]=share[r];share[r]=tmp;
							
			}
	

	CbyteShare mshare(m,n);
	for(times=0;times<=Times;times++)
	{
	mshare.SetShareByte((unsigned char)times);

	 mshare.MakeShare();
	
	if(mshare.RestorShareByte(share)!=(unsigned char)times) 
		{
			printf("oooooooooo"); break;
		}
	
	}

	 printf( "\ntimes=%ul(%d,%d),mt=%qi rt=%qi mshare=%d\n",times,m,n,timeMakeShare,timeRestore,mshare.RestorShareByte(share) );
	

	return 0;


}
