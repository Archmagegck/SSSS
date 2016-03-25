#include <gmp.h>


#define MAXDEGREE 1024
static const unsigned char irred_coeff[] = {
  4,3,1,5,3,1,4,3,1,7,3,2,5,4,3,5,3,2,7,4,2,4,3,1,10,9,3,9,4,2,7,6,2,10,9,
  6,4,3,1,5,4,3,4,3,1,7,2,1,5,3,2,7,4,2,6,3,2,5,3,2,15,3,2,11,3,2,9,8,7,7,
  2,1,5,3,2,9,3,1,7,3,1,9,8,3,9,4,2,8,5,3,15,14,10,10,5,2,9,6,2,9,3,2,9,5,
  2,11,10,1,7,3,2,11,2,1,9,7,4,4,3,1,8,3,1,7,4,1,7,2,1,13,11,6,5,3,2,7,3,2,
  8,7,5,12,3,2,13,10,6,5,3,2,5,3,2,9,5,2,9,7,2,13,4,3,4,3,1,11,6,4,18,9,6,
  19,18,13,11,3,2,15,9,6,4,3,1,16,5,2,15,14,6,8,5,2,15,11,2,11,6,2,7,5,3,8,
  3,1,19,16,9,11,9,6,15,7,6,13,4,3,14,13,3,13,6,3,9,5,2,19,13,6,19,10,3,11,
  6,5,9,2,1,14,3,2,13,3,1,7,5,4,11,9,8,11,6,5,23,16,9,19,14,6,23,10,2,8,3,
  2,5,4,3,9,6,4,4,3,2,13,8,6,13,11,1,13,10,3,11,6,5,19,17,4,15,14,7,13,9,6,
  9,7,3,9,7,1,14,3,2,11,8,2,11,6,4,13,5,2,11,5,1,11,4,1,19,10,3,21,10,6,13,
  3,1,15,7,5,19,18,10,7,5,3,12,7,2,7,5,1,14,9,6,10,3,2,15,13,12,12,11,9,16,
  9,7,12,9,3,9,5,2,17,10,6,24,9,3,17,15,13,5,4,3,19,17,8,15,6,3,19,6,1 };

class Cpoly
{
private:

	unsigned int degree;
	mpz_t poly;
public:
	Cpoly(int deg ){ if(deg < 8) deg=8; if (deg > MAXDEGREE) deg=MAXDEGREE; if(deg % 8 != 0) deg=int(deg/8)*8;        mpz_init_set_ui(poly, 0);
  mpz_setbit(poly, deg);
  mpz_setbit(poly, irred_coeff[3 * (deg / 8 - 1) + 0]);
  mpz_setbit(poly, irred_coeff[3 * (deg / 8 - 1) + 1]);
  mpz_setbit(poly, irred_coeff[3 * (deg / 8 - 1) + 2]);
  mpz_setbit(poly, 0);
  degree = deg;};
	~Cpoly(int deg ){mpz_clear(poly);}
	 friend class Cfield;
	
};


class Cfield
{
private:
	
	mpz_t a;
	Cpoly *poly;
	mpz_t tmp;

public:
	
	Cfield(Cpoly *p){poly=p;mpz_init (tmp);mpz_init (a); };
	~Cfield(){mpz_clear(a);mpz_clear(tmp);}
	
	Cfield operator+ (Cfield &b){mpz_xor(tmp, a, b);return tmp;};
	Cfield operator- (Cfield &b){mpz_xor(tmp, a, b);return tmp;};
	Cfield operator* (Cfield &b);
	Cfield operator/ (Cfield &b);
	Cfield &operator= (Cfield &b){mpz_set(a, b); return *this;};
	int operator== (Cfield &b){return mpz_cmp(a,b)==0;};
	int	operator< (Cfield &b){return mpz_cmp(a,b)<0;};
	Cfield invert() ;
	Cfield pow (Cbytefield &b);	
};



Cfield Cfield::operator* (Cfield &b)
{
//void field_mult(mpz_t z, const mpz_t x, const mpz_t y)
//{
  mpz_t t;
  unsigned int i;
  
  mpz_init_set(t, a);
  if (mpz_tstbit(b, 0))
    mpz_set(tmp, t);
  else
    mpz_set_ui(tmp, 0);
  for(i = 1; i < poly->degree; i++) {
    mpz_lshift(t, t, 1);
    if (mpz_tstbit(t, degree))
      mpz_xor(t, t, poly->poly);
    if (mpz_tstbit(b, i))
      mpz_xor(tmp, tmp, t);
  }
  mpz_clear(t);

 return tmp;
//}
}

Cfield Cfield::invert ()
{
 mpz_t u, v, g, h;
  int i;
  assert(mpz_cmp_ui(a, 0));
  mpz_init_set(u, a);
  mpz_init_set(v, poly);
  mpz_init_set_ui(g, 0);
  mpz_set_ui(tmp, 1);
  mpz_init(h);
  while (mpz_cmp_ui(u, 1)) {
    i = mpz_sizeinbits(u) - mpz_sizeinbits(v);
    if (i < 0) {
      mpz_swap(u, v);
      mpz_swap(tmp, g);
      i = -i;
    }
    mpz_lshift(h, v, i);
    mpz_xor(u, u, h);
    mpz_lshift(h, g, i);
    mpz_xor(tmp, tmp, h);
  }
  mpz_clear(u); mpz_clear(v); mpz_clear(g); mpz_clear(h);
  return tmp;
}

Cfield Cfield::operator/ (Cfield &b)
{

}


