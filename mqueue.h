/*--------------------------------MQUEUE_H--------------------------------*/
//队列

#ifndef MQUEUE_H
#define MQUEUE_H

#define QUEUE(name,max_size)\
int QUEUE_##name##_front = 0;\
int QUEUE_##name##_tail = 0;\
int QUEUE_##name##_size = 0;\
int QUEUE_##name##_arr [ max_size ];\
int QUEUE_##name##_sum = 0;\
int QUEUE_##name##_MAX_SIZE = max_size;

#define QUEUE_push(name,x)\
{\
	QUEUE_##name##_arr[QUEUE_##name##_tail++]=x;\
	if(QUEUE_##name##_tail>=QUEUE_##name##_MAX_SIZE)QUEUE_##name##_tail=0;\
	QUEUE_##name##_sum+=x;\
	QUEUE_##name##_size++;\
}
#define QUEUE_pop(name)\
{\
	QUEUE_##name##_sum -= QUEUE_##name##_arr [ QUEUE_##name##_front ];\
	QUEUE_##name##_front++;\
	if(QUEUE_##name##_front>=QUEUE_##name##_MAX_SIZE)QUEUE_##name##_front=0;\
	QUEUE_##name##_size--;\
}
#define QUEUE_front(name) QUEUE_##name##_arr [ QUEUE_##name##_front ]
#define QUEUE_size(name) QUEUE_##name##_size
#define QUEUE_tail(name) QUEUE_##name##_arr [ ( QUEUE_##name##_tail ) >=1 ? QUEUE_##name##_tail - 1 : QUEUE_##name##_MAX_SIZE - 1 ]
#define QUEUE_MAX_SIZE(name) QUEUE_##name##_MAX_SIZE
#define QUEUE_clear(name)\
{\
	QUEUE_##name##_front = 0;\
	QUEUE_##name##_tail = 0;\
	QUEUE_##name##_size = 0;\
}

#endif
