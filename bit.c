/*---------------------------------BIT_C---------------------------------*/
//树状数组
/*
#include <stdint.h>

static const uint16_t MAX_VAL = 17000; 
static const uint8_t MAX_VAL_LOG2 = 14;
static int8_t BIT[MAX_VAL];				//17kb

inline uint16_t lowbit(uint16_t x){return x&-x;}

void BIT_update(uint16_t pos,int8_t x)
{
	while(pos<MAX_VAL)
	{
		BIT[pos]+=x;
		pos += lowbit(pos);
	}
}

uint16_t BIT_find_kth(int8_t k)
{
	uint16_t ans = 0;
	int8_t sum = 0;
	for(int16_t i=MAX_VAL_LOG2; i>=0 ; i--)
	{
	    uint16_t i_2 = (1<<i);
	 if(ans+i_2<MAX_VAL)
	 {
		 ans+=i_2;
		 int8_t tmp = sum+BIT[ans];
		 if(tmp<k)sum=tmp;
		 else ans-=i_2;
	 }
	}
	return ans+1;
}
*/
