/*---------------------------------HRV_C---------------------------------*/
#ifndef HRV_H
#define HRV_H

void HRV_COMPUTE_init(int * arr , int n);
void HRV_COMPUTE_compute(int * arr , int n);

/////////////////////////desc/////////////////////////
float HRV_COMPUTE_desc_min(void);
float HRV_COMPUTE_desc_max(void);
float HRV_COMPUTE_desc_mean(void);
float HRV_COMPUTE_desc_var(void);
float HRV_COMPUTE_desc_std(void);
float HRV_COMPUTE_desc_median(void);
float HRV_COMPUTE_desc_amplitude(void);

/////////////////////////time_domain/////////////////////////
float HRV_COMPUTE_time_domain_rmssd(void);
float HRV_COMPUTE_time_domain_sdnn(void);
float HRV_COMPUTE_time_domain_sdsd(void);
float HRV_COMPUTE_time_domain_nn50(void);
float HRV_COMPUTE_time_domain_pnn50(void);
float HRV_COMPUTE_time_domain_mrri(void);
float HRV_COMPUTE_time_domain_mhr(void);

/////////////////////////frequency_domain/////////////////////////
float HRV_COMPUTE_frequency_domain_total_power(void);
float HRV_COMPUTE_frequency_domain_vlf(void);
float HRV_COMPUTE_frequency_domain_lf(void);
float HRV_COMPUTE_frequency_domain_hf(void);
float HRV_COMPUTE_frequency_domain_lf_hf(void);
float HRV_COMPUTE_frequency_domain_lfnu(void);
float HRV_COMPUTE_frequency_domain_hfnu(void);

/////////////////////////non_linear/////////////////////////
float HRV_COMPUTE_non_linear_sd1(void);
float HRV_COMPUTE_non_linear_sd2(void);

#endif
