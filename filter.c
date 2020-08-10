/*---------------------------------FILTER_C---------------------------------*/

#include <stdint.h>

#include "mqueue.h"
#include "bit.h"
#include "config.h"

// 中值滤波函数

QUEUE(filter_queue,FILTER_SIZE);
uint16_t FILTER_doFilter(uint16_t cur)
{
	/*cur++;
	if(QUEUE_size(filter_queue)<QUEUE_MAX_SIZE(filter_queue))
	{
		QUEUE_push(filter_queue,cur);
		BIT_update(cur,1);
		return cur-1;
	}
	else
	{
		BIT_update(cur,1);
		BIT_update(QUEUE_front(filter_queue),-1);
		QUEUE_pop(filter_queue);
		QUEUE_push(filter_queue,cur);
		return BIT_find_kth(QUEUE_size(filter_queue)/2)-1;
	}*/
	return cur;
}
