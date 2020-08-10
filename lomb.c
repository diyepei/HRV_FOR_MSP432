#include <math.h>

#include "config.h"

static long lmaxarg1, lmaxarg2;
#define LMAX(a,b) (lmaxarg1 = (a),lmaxarg2 = (b), (lmaxarg1 > lmaxarg2 ? \
lmaxarg1 : lmaxarg2))
static long lminarg1, lminarg2;
#define LMIN(a,b) (lminarg1 = (a),lminarg2 = (b), (lminarg1 < lminarg2 ? \
lminarg1 : lminarg2))
#define MOD(a,b)    while (a >= b) a -= b
#define MACC 4
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
static float sqrarg;
#define SQR(a) ((sqrarg = (a)) == 0.0 ? 0.0 : sqrarg*sqrarg)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

char *pname;
float pwr;

void avevar(float * data,unsigned long n,float *ave,float* var), realft(float * data,unsigned long n, int isign);
void spread(float y, float *yy,unsigned long n, float x, int m);
void four1(float data[],unsigned long nn,int isign);

void fasper(float x[], float y[],unsigned long n,float ofac,float hifac,float wk1[],float wk2[],unsigned long nwk,unsigned long *nout,unsigned long *jmax,float *prob)
{
    unsigned long j, k, ndim, nfreq, nfreqt;
    float ave, ck, ckk, cterm, cwt, den, df, effm, expy, fac, fndim, hc2wt,
    hs2wt, hypo, pmax, sterm, swt, var, xdif, xmax, xmin;
    
    *nout = 0.5*ofac*hifac*n;
    nfreqt = ofac*hifac*n*MACC;
    nfreq = 64;
    while (nfreq < nfreqt) nfreq <<= 1;
        ndim = nfreq << 1;
        if (ndim > nwk)
            avevar(y, n, &ave, &var);
            xmax = xmin = x[1];
            for (j = 2; j <= n; j++) {
                if (x[j] < xmin) xmin = x[j];
                if (x[j] > xmax) xmax = x[j];
            }
    xdif = xmax - xmin;
    for (j = 1; j <= ndim; j++) wk1[j] = wk2[j] = 0.0;
        fac = ndim/(xdif*ofac);
        fndim = ndim;
        for (j = 1; j <= n; j++) {
            ck = (x[j] - xmin)*fac;
            MOD(ck, fndim);
            ckk = 2.0*(ck++);
            MOD(ckk, fndim);
            ++ckk;
            spread(y[j] - ave, wk1, ndim, ck, MACC);
            spread(1.0, wk2, ndim, ckk, MACC);
        }
    realft(wk1, ndim, 1);
    realft(wk2, ndim, 1);
    df = 1.0/(xdif*ofac);
    pmax = -1.0;
    for (k = 3, j = 1; j <= (*nout); j++, k += 2) {
        hypo = sqrt(wk2[k]*wk2[k] + wk2[k+1]*wk2[k+1]);
        hc2wt = 0.5*wk2[k]/hypo;
        hs2wt = 0.5*wk2[k+1]/hypo;
        cwt = sqrt(0.5+hc2wt);
        swt = SIGN(sqrt(0.5-hc2wt), hs2wt);
        den = 0.5*n + hc2wt*wk2[k] + hs2wt*wk2[k+1];
        cterm = SQR(cwt*wk1[k] + swt*wk1[k+1])/den;
        sterm = SQR(cwt*wk1[k+1] - swt*wk1[k])/(n - den);
        wk1[j] = j*df;
        wk2[j] = (cterm+sterm)/(2.0*var);
        if (wk2[j] > pmax) pmax = wk2[(*jmax = j)];
    }
    expy = exp(-pmax);
    effm = 2.0*(*nout)/ofac;
    *prob = effm*expy;
    if (*prob > 0.01) *prob = 1.0 - pow(1.0 - expy, effm);
        }

void spread(float y, float *yy,unsigned long n, float x, int m)
{
    int ihi, ilo, ix, j, nden;
    static int nfac[11] = { 0, 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };
    float fac;
		ix = (int)x;
		if (x == (float)ix) yy[ix] += y;
				else {
						ilo = LMIN(LMAX((long)(x - 0.5*m + 1.0), 1), n - m + 1);
						ihi = ilo + m - 1;
						nden = nfac[m];
						fac = x - ilo;
						for (j = ilo + 1; j <= ihi; j++) fac *= (x - j);
						yy[ihi] += y*fac/(nden*(x - ihi));
						for (j = ihi-1; j >= ilo; j--) {
								nden = (nden/(j + 1 - ilo))*(j - ihi);
								yy[j] += y*fac/(nden*(x - j));
						}
				}
}

void avevar(float * data,unsigned long n,float *ave,float* var)
{
    unsigned long j;
    float s, ep;
    
    for (*ave = 0.0, j = 1; j <= n; j++) *ave += data[j];
        *ave /= n;
        *var = ep = 0.0;
        for (j = 1; j <= n; j++) {
            s = data[j] - (*ave);
            ep += s;
            *var += s*s;
        }
    *var = (*var - ep*ep/n)/(n-1);
    pwr = *var;
}

void realft(float * data,unsigned long n, int isign)
{
    
    unsigned long i, i1, i2, i3, i4, np3;
    float c1 = 0.5, c2, h1r, h1i, h2r, h2i;
    float wr, wi, wpr, wpi, wtemp, theta;
    
    theta = 3.141592653589793/(float)(n>>1);
    if (isign == 1) {
        c2 = -0.5;
        four1(data, n>>1, 1);
    } else {
        c2 = 0.5;
        theta = -theta;
    }
    wtemp = sin(0.5*theta);
    wpr = -2.0*wtemp*wtemp;
    wpi = sin(theta);
    wr = 1.0 + wpr;
    wi = wpi;
    np3 = n+3;
    for (i = 2; i <= (n>>2); i++) {
        i4 = 1 + (i3 = np3 - (i2 = 1 + (i1 = i + i - 1)));
        h1r =  c1*(data[i1] + data[i3]);
        h1i =  c1*(data[i2] - data[i4]);
        h2r = -c2*(data[i2] + data[i4]);
        h2i =  c2*(data[i1] - data[i3]);
        data[i1] =  h1r + wr*h2r - wi*h2i;
        data[i2] =  h1i + wr*h2i + wi*h2r;
        data[i3] =  h1r - wr*h2r + wi*h2i;
        data[i4] = -h1i +wr*h2i + wi*h2r;
        wr = (wtemp = wr)*wpr - wi*wpi + wr;
        wi = wi*wpr + wtemp*wpi + wi;
    }
    if (isign == 1) {
        data[1] = (h1r = data[1]) + data[2];
        data[2] = h1r - data[2];
    } else {
        data[1] = c1*((h1r = data[1]) + data[2]);
        data[2] = c1*(h1r - data[2]);
        four1(data, n>>1, -1);
    }
}

void four1(float data[],unsigned long nn,int isign)
{
    unsigned long n, mmax, m, j, istep, i;
    float wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;
    
    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            SWAP(data[j], data[i]);
            SWAP(data[j+1], data[i+1]);
        }
        m = n >> 1;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = isign*(6.28318530717959/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m;i <= n;i += istep) {
                j = i + mmax;
                tempr = wr*data[j] - wi*data[j+1];
                tempi = wr*data[j+1] + wi*data[j];
                data[j] = data[i] - tempr;
                data[j+1] = data[i+1] - tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr = (wtemp = wr)*wpr - wi*wpi + wr;
            wi = wi*wpr + wtemp*wpi + wi;
        }
        mmax = istep;
    }
}

float *x, *y;//, *wk1, *wk2;
float wk1[64*HRV_CACHE_SIZE];
float wk2[64*HRV_CACHE_SIZE];
float *xx, *yy;
long nmax = HRV_CACHE_SIZE;    /* Initial buffer size (must be a power of 2).
                      Note that input() will increase this value as
                      necessary by repeated doubling, depending on
                      the length of the input series. */


/* Read input data, allocating and filling x[] and y[].  The return value is
 the number of points read.
 
 This function allows the input buffers to grow as large as necessary, up to
 the available memory (assuming that a long int is large enough to address
 any memory location). */


/* This function calculates the mean of all sample values and subtracts it
 from each sample value, so that the mean of the adjusted samples is zero. */

void zeromean(unsigned long n)

{
    unsigned long i;
    float ysum = 0.0;
    
    for (i = 0; i < n; i++)
        ysum += y[i];
        ysum /= n;
        for (i = 0; i < n; i++)
            y[i] -= ysum;
}

static float * rows[HRV_CACHE_SIZE*5];
static float values[HRV_CACHE_SIZE*9];
float** createArray(int m, int n)
{
	
	//static float* values = (float *)calloc(m*n, sizeof(float));
	//static float** rows = (float **)malloc(n*sizeof(float*));
	for (int i=0; i<n; ++i)
	{
			rows[i] = values + i*m;
	}
	return rows;
}

static float xxx[HRV_CACHE_SIZE];
static float yyy[HRV_CACHE_SIZE];

float **getPowerResult(int x, int y, float inputData[][2])
{


    float prob;
    unsigned long nout, jmax, maxout;
    
    unsigned long npts = 0L;

    for (int i = 0; i < x; i++) {
        xxx[npts] = inputData[i][0];
        yyy[npts] = inputData[i][1];
    }
    
    
    /* Compute the Lomb periodogram. */
    fasper(xxx-1, yyy-1, x, 4.0, 2.0, wk1-1, wk2-1, 64*nmax, &nout, &jmax, &prob);

    maxout = nout/2;

    float** powerData = createArray(2,(int)maxout);

    for (int n = 0; n < maxout; n++) {
//        printf("%g\t%g\n", wk1[n], sqrt(wk2[n]/(nout/(2.0*pwr))));
        powerData[n][0]=wk1[n];
        powerData[n][1]=sqrt(wk2[n]/(nout/(2.0*pwr)));
    }
    
//    destroyArray(powerData);

    return powerData;
}

