/*---------------------------------HRV_C---------------------------------*/

#include <math.h>

#include "config.h"
#include "mqueue.h"
#include "utility.h"
#include "pwr.h"

static int * HRV_COMPUTE_arr = 0;
static int HRV_COMPUTE_n = 0;

static float HRV_COMPUTE_min = 99999.0 ;
static float HRV_COMPUTE_max = -99999.0 ;
static float HRV_COMPUTE_mid = 0 ;
static float HRV_COMPUTE_mean = 0 ;
static float HRV_COMPUTE_var_sum = 0;
static float HRV_COMPUTE_var = 0;
static float HRV_COMPUTE_std = 0;
static float HRV_COMPUTE_std_ddof = 0;

static int HRV_COMPUTE_diff[HRV_CACHE_SIZE] = {0};
static float HRV_COMPUTE_diff_mean = 0 ;
//static float HRV_COMPUTE_diff_var_sum = 0 ;
//static float HRV_COMPUTE_diff_var = 0 ;
//static float HRV_COMPUTE_diff_std = 0;
static float HRV_COMPUTE_diff_std_ddof = 0;
static float HRV_COMPUTE_inputDataForFrequencyDomain[HRV_CACHE_SIZE][2];

static float HRV_COMPUTE_vlf = 0;
static float HRV_COMPUTE_lf = 0;
static float HRV_COMPUTE_hf = 0;
static float HRV_COMPUTE_total_power = 0;

void HRV_COMPUTE_init(int * arr , int n)
{
	HRV_COMPUTE_arr = arr;
	HRV_COMPUTE_n = n;
	
	HRV_COMPUTE_min = 99999 ;
	HRV_COMPUTE_max = -99999 ;
	HRV_COMPUTE_mid = 0 ;
	HRV_COMPUTE_mean = 0 ;
	HRV_COMPUTE_var_sum = 0;
	HRV_COMPUTE_var = 0;
	HRV_COMPUTE_std = 0;
	HRV_COMPUTE_std_ddof = 0;
	
	HRV_COMPUTE_diff_mean = 0 ;
	//HRV_COMPUTE_diff_var_sum = 0 ;
	//HRV_COMPUTE_diff_var = 0 ;
	//HRV_COMPUTE_diff_std = 0;
	HRV_COMPUTE_diff_std_ddof = 0;
	
	HRV_COMPUTE_vlf = 0;
	HRV_COMPUTE_lf = 0;
	HRV_COMPUTE_hf = 0;
	HRV_COMPUTE_total_power = 0;
}

void HRV_COMPUTE_compute(int * arr , int n)
{
	HRV_COMPUTE_arr = arr;
	HRV_COMPUTE_n = n;	
	
	for(int i=0;i<n;i++)
	{
		float tmp = (float)arr[i];
		HRV_COMPUTE_min = UTILITY_min(HRV_COMPUTE_min,tmp);
		HRV_COMPUTE_max = UTILITY_max(HRV_COMPUTE_max,tmp);
		HRV_COMPUTE_mean = HRV_COMPUTE_mean + tmp;
	}
	HRV_COMPUTE_mid = (float)arr[UTILITY_find_kth(arr,0,n-1,n/2+1)];
	HRV_COMPUTE_mean /= n;
	for(int i=0;i<n;i++)
	{
		float tmp = (float)HRV_COMPUTE_mean-(float)arr[i];
		HRV_COMPUTE_var_sum+=tmp*tmp;
	}
	
	HRV_COMPUTE_var = HRV_COMPUTE_var_sum / n;
	HRV_COMPUTE_std = sqrt( HRV_COMPUTE_var_sum /n );
	HRV_COMPUTE_std_ddof = sqrt( HRV_COMPUTE_var_sum / (n-1) );
	
	for(int i=0;i<n-1;i++)
	{
		HRV_COMPUTE_diff[i] = arr[i+1]-arr[i];
	}
	
	for(int i=0;i<n-1;i++)
	{
		float tmp = (float)HRV_COMPUTE_diff[i];
		HRV_COMPUTE_diff_mean = HRV_COMPUTE_diff_mean + tmp;
	}
	HRV_COMPUTE_diff_mean /= n-1;
	
	for(int i=0;i<n-1;i++)
	{
		float tmp = (float)HRV_COMPUTE_diff_mean-(float)HRV_COMPUTE_diff[i];
		HRV_COMPUTE_var_sum+=tmp*tmp;
	}
	HRV_COMPUTE_diff_std_ddof = sqrt(HRV_COMPUTE_var_sum/(n-2));
	
	HRV_COMPUTE_inputDataForFrequencyDomain[0][1]=(float)arr[0]/1000.0;
	HRV_COMPUTE_inputDataForFrequencyDomain[0][0]=0.0;
	for(int i=1;i<n-1;i++)
	{
		HRV_COMPUTE_inputDataForFrequencyDomain[i][1]=(float)arr[0]/1000.0;
		HRV_COMPUTE_inputDataForFrequencyDomain[i][0]=HRV_COMPUTE_inputDataForFrequencyDomain[i-1][0]+HRV_COMPUTE_inputDataForFrequencyDomain[i][1];
	}
	
	float *frequencyResult = getFrequencyResult(n,2,HRV_COMPUTE_inputDataForFrequencyDomain);
	
	HRV_COMPUTE_vlf = frequencyResult[0];
	HRV_COMPUTE_lf = frequencyResult[3];
	HRV_COMPUTE_hf = frequencyResult[4];
	HRV_COMPUTE_total_power = HRV_COMPUTE_vlf + HRV_COMPUTE_lf + HRV_COMPUTE_hf;
	
}

/////////////////////////compute/////////////////////////	



/////////////////////////compute/////////////////////////	

/////////////////////////desc/////////////////////////
float HRV_COMPUTE_desc_min()
{
	return HRV_COMPUTE_min;
}
float HRV_COMPUTE_desc_max()
{
	return HRV_COMPUTE_max;
}
float HRV_COMPUTE_desc_mean()
{
	return HRV_COMPUTE_mean;
}
float HRV_COMPUTE_desc_var()
{
	return HRV_COMPUTE_var;
}
float HRV_COMPUTE_desc_std()
{
	return HRV_COMPUTE_std;
}
float HRV_COMPUTE_desc_median()
{
	return HRV_COMPUTE_mid;
}
float HRV_COMPUTE_desc_amplitude()
{
	return HRV_COMPUTE_max - HRV_COMPUTE_min;
}
/////////////////////////desc/////////////////////////

/////////////////////////time_domain/////////////////////////
float HRV_COMPUTE_time_domain_rmssd()
{
	float sum = 0;
	for(int i=0;i<HRV_COMPUTE_n-1;i++)
	{
		float tmp = HRV_COMPUTE_diff[i];
		sum += tmp*tmp;
	}
	return sqrt(sum/(HRV_COMPUTE_n-1));
}
float HRV_COMPUTE_time_domain_sdnn()
{
	return HRV_COMPUTE_std_ddof;
}
float HRV_COMPUTE_time_domain_sdsd()
{
	return HRV_COMPUTE_diff_std_ddof;
}
float HRV_COMPUTE_time_domain_nn50()
{
	float cnt = 0;
	for(int i=0;i<HRV_COMPUTE_n-1;i++)
	{
		if(UTILITY_abs(HRV_COMPUTE_diff[i])>50.0)cnt++;
	}
	return cnt;
}
float HRV_COMPUTE_time_domain_pnn50()
{
	return HRV_COMPUTE_time_domain_nn50() / (float)HRV_COMPUTE_n * 100.0;
}
float HRV_COMPUTE_time_domain_mrri()
{
	return HRV_COMPUTE_mean;
}
float HRV_COMPUTE_time_domain_mhr()
{
	float sum = 0;
	for(int i=0;i<HRV_COMPUTE_n;i++)
	{
		sum += 60000.0/(float)HRV_COMPUTE_arr[i];
	}
	return sum/HRV_COMPUTE_n;
}
/////////////////////////time_domain/////////////////////////

/////////////////////////frequency_domain/////////////////////////
float HRV_COMPUTE_frequency_domain_total_power()
{
	return HRV_COMPUTE_total_power;
}
float HRV_COMPUTE_frequency_domain_vlf()
{
	return HRV_COMPUTE_vlf;
}
float HRV_COMPUTE_frequency_domain_lf()
{
	return HRV_COMPUTE_lf;
}
float HRV_COMPUTE_frequency_domain_hf()
{
	return HRV_COMPUTE_hf;
}
float HRV_COMPUTE_frequency_domain_lf_hf()
{
	return HRV_COMPUTE_lf/HRV_COMPUTE_hf;
}
float HRV_COMPUTE_frequency_domain_lfnu()
{
	return (HRV_COMPUTE_lf / (HRV_COMPUTE_total_power - HRV_COMPUTE_vlf)) * 100.0;
}
float HRV_COMPUTE_frequency_domain_hfnu()
{
	return (HRV_COMPUTE_hf / (HRV_COMPUTE_total_power - HRV_COMPUTE_vlf)) * 100;
}
/////////////////////////frequency_domain/////////////////////////

/////////////////////////non_linear/////////////////////////

float HRV_COMPUTE_non_linear_sd1()
{
	return sqrt(HRV_COMPUTE_diff_std_ddof * HRV_COMPUTE_diff_std_ddof * 0.5);
}
float HRV_COMPUTE_non_linear_sd2()
{
	return sqrt(2 * HRV_COMPUTE_std_ddof * HRV_COMPUTE_std_ddof - 0.5 * HRV_COMPUTE_diff_std_ddof * HRV_COMPUTE_diff_std_ddof );
}

/////////////////////////non_linear/////////////////////////
