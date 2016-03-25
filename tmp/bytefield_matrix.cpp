// bytefield.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"

/*
基于字节有限域的秘密分享，秘密一个字节一个字节的计算影子
缺点如果秘密是有意义的话时，而不是随机值，那么如果在缺一人的情况下，可以尝试穷举缺的影子。
但如果把信息加密的话，这种尝试可能无效。
可以共享的信息为：key|msg更好
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
	Cbytefield an[255],yn[255][2];


	Cbytefield v[255][256];

	unsigned short m,n;
			
	Cbytefield funxm(Cbytefield x)
	{
		Cbytefield y(0);
	//	an[m-1]=toShareBytem;
		unsigned short i;
		for(i=0;i<m-1;i++)
		{
			y=y+an[i]*x.pow(m-1-i); 
		}

		y=y+toShareBytem;
		
		return y;
	};

	
	void RandSetAn(){
						srand( (unsigned)time( NULL ) );
						for(int i=0;i<m;i++) 
						{	word8 r=rand()%256;
							if(0==r)  {i--;continue;}
							an[i]=r; 
						}
						
					};

public:
	Cbytefield yn[255][2];


	CbyteShare(unsigned short _m,unsigned short _n) {m=_m;n=_n;};
	/*void SetShareByte(word8 bytemsg){toShareBytem=bytemsg;RandSetAn();for(word16 a=1;a<=n;a++){yn[a-1][0]=a;yn[a-1][1]=funxm(a); }	};*/
	word8 RestorShareByte();

	void setv()
	{
		 int i,j,j;
		 for(i=0;i<255;i++)
		 {	
			for(j=i,k=1;j<255;j++,k++)
			{			
				v[i][j]=k;
				v[j][i]=k;
			}
		
		 }
		 for(i=0;i<255;i++)
		{	printf("\n");
			for(j=0;j<255;j++)
				printf("% 3d",v[i][j]);
		}
	}
		
	

};
word8 CbyteShare::RestorShareByte()
{

	unsigned short  i,j;

	for( i=0;i<m;i++)
	{ 	 
		for( j=0;j<m;j++)
			{Cbytefield tmp;
			 tmp=yn[i][0];
			 v[i][j]=tmp.pow(m-1-j);	
			 //printf("%3d,",v[i][j].value()); 
			}

	}


	for(i=0;i<m;i++)
	{v[i][m]=yn[i][1];}



	for(i=0;i<m;i++)
	{
		Cbytefield tmp;
		tmp=v[i][i];

			for(j=i+1;j<m;j++)
			{Cbytefield numhead;			
				numhead=v[j][i];
				for(int k=i;k<m+1;k++)
				{	
					Cbytefield tmp2;						
						
					v[j][k]=v[j][k]* tmp;
					
					v[255][k]= v[i][k]* numhead;

					v[j][k]=v[j][k]-v[255][k];
					
				}
				
			}

	}
	
	toShareBytem=v[m-1][m]/v[m-1][m-1];
	
	//printf("\nm=%d,",toShareBytem.value());
	return toShareBytem.value();
}



int main(int argc, char* argv[])
{
	word16	 a,b,c;

	
	int m=255,n=255,msg;
	//scanf("%d%d%d",&m,&n,&msg);

	CbyteShare mshare(m,n);

//	mshare.SetShareByte(msg);
	mshare.setv();
//	mshare.makeShare();

	//printf("\nm=====%d ",mshare.RestorShareByte()); 
	return 0;


}

