/*---------------------------------UTILITY_C---------------------------------*/

double UTILITY_max(double a, double b){return a>b?a:b;}
double UTILITY_min(double a, double b){return a<b?a:b;}
double UTILITY_abs(double x){return x>0?x:-x;}

void UTILITY_swap (int *a ,int *b)
{
	int tmp =*a;
	*a = *b;
	*b = tmp;
}

int UTILITY_find_kth(int *arr,int a,int b,int k)
{
	int t = arr[a];
	int i=a+1;
	int j=b;
	while(i<=j)
	{
		if(t>arr[i])
		{
			UTILITY_swap(arr+i-1,arr+i);
			i++;
		}
		else
		{
			UTILITY_swap(arr+i,arr+j);
			j--;
		}
	}
    if(j-a+1>k)return UTILITY_find_kth(arr,a,j-1,k);
    else if (j-a+1==k) return j;
    else return UTILITY_find_kth(arr,j+1,b,k-(j-a+1));
}
