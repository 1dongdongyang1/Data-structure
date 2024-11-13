#include"Sort.h"
#include"Stack.h"


//交换
void Swap(int* p1, int* p2)
{
	int tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

//三数取中
int GetMidIndex(int* a, int left, int right)
{
	int mid = (left + right) / 2;
	if (a[left] < a[mid])
	{
		if (a[mid] < a[right])
			return mid;
		else if (a[left] > a[right])
			return left;
		else
			return right;
	}
	else //left>mid
	{
		if (a[right] < a[mid])
			return mid;
		else if (a[right] > a[left])
			return left;
		else
			return right;
	}
}

// 插入排序 O(N^2)
void InsertSort(int* a, int n)
{
	assert(a);

	for (int i = 0; i < n - 1; i++)//注意n-2的边界
	{
		//把end+1的数据插入[0,end+1]的有序空间
		int end=i;
		int tmp = a[end + 1];

		while (end >= 0)
		{
			if (tmp < a[end])
			{
				a[end + 1] = a[end];
				end--;
			}
			else
				break;
		}
		a[end + 1] = tmp;
	}
}

// 希尔排序 O(N^1.3——N^2)
void ShellSort(int* a, int n)
{
	assert(a);

	//1.gap>1相当于预排列，让数组接近有序
	//2.gap==1就相当于直接插入排列，保证有序
	int gap=n;
	while (gap > 1)
	{
		gap = gap / 3 + 1;	//+1保证最后一次gap一定是1
							//gap==1相当于直接插入排列

		for (int i = 0; i < n - gap; i++)//多组并排
		{
			int end = i;
			int tmp = a[end + gap];

			while (end >= 0)
			{
				if (tmp < a[end])
				{
					a[end + gap] = a[end];
					end -= gap;
				}
				else
					break;
			}

			a[end + gap] = tmp;
		}
	}

}

// 选择排序 O(N^2)
void SelectSort(int* a, int n)
{
	assert(a);

	int begin = 0, end = n - 1;

	while (begin < end)
	{
		int mini, maxi;
		mini = maxi = begin;

		for (int i = begin+1 ; i <= end; i++)
		{
			if (a[i] > a[maxi])
				maxi = i;

			if (a[i] < a[mini])
				mini = i;
		}

		Swap(&a[begin], &a[mini]);
		//如果maxi跟begin位置重叠，则maxi的位置需要修正
		if (begin == maxi)
			maxi = mini;
		Swap(&a[end], &a[maxi]);

		begin++;
		end--;
	}
}

// 堆排序
void AdjustDwon(int* a, int n, int root)
{
	int parent = root;
	int child = parent * 2 + 1;

	while (child < n)
	{
		if (child + 1 < n && a[child + 1] > a[child])
			child++;

		if (a[child] > a[parent])
		{
			Swap(&a[child], &a[parent]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
			break;
	}
}
//时间复杂度 O(N*logN)
void HeapSort(int* a, int n)
{	//升序建大堆
	for (int i = (n - 1 - 1) / 2; i >= 0; i--)
	{
		AdjustDwon(a, n, i);
	}

	int end = n - 1;
	while (end)
	{
		Swap(&a[0], &a[end]);
		AdjustDwon(a, end, 0);
		end--;
	}
}

// 冒泡排序 O(N^2)
void BubbleSort(int* a, int n)
{
	int end = n;
	while (end)
	{
		int exchange = 0;
		for (int i = 1; i < end; i++)
		{
			if (a[i - 1] > a[i])
			{
				Swap(&a[i - 1], &a[i]);
				exchange = 1;
			}
		}

		//如果一趟冒泡的过程中没有发生交换，则说明前部分已经有序，不需要继续冒泡
		if (exchange == 0)
			break;
		end--;
	}
}

// 快速排序递归实现
// 快速排序hoare版本(左右指针法) 
int PartSort1(int* a, int left, int right)//[left,right]
{
	int midindex = GetMidIndex(a,left, right);//三数取中,规避掉最大和最小，让最坏的情况不在发生
	Swap(&a[midindex], &a[right]);

	int keyindex = right;	//key选择右边的，让left先走，这样可以保证最后重叠停下的位置是比key大的数(left找到的数)，
							//与key交换后满足左边都比key小，右边比key大
	
	while (left < right)
	{
		while (left < right && a[left] <= a[keyindex])//没有等于会出现死循环
			left++;

		while (left < right && a[right] >= a[keyindex])
			right--;

		Swap(&a[left], &a[right]);
	}
	Swap(&a[left], &a[keyindex]);
	return left;
}

// 快速排序挖坑法  跟左右指针法相比，挖坑填坑就相当于是把交换拆分开
int PartSort2(int* a, int left, int right)
{
	int midindex = GetMidIndex(a, left, right);//三数取中,规避掉最大和最小，让最坏的情况不在发生
	Swap(&a[midindex], &a[right]);

	//坑
	int key = a[right];

	while (left < right)
	{
		while (left < right && a[left] <= key)
			left++;

		//把左边找到的比key大的数填到右边的坑里
		a[right] = a[left];

		while (left < right && a[right] >= key)
			right--;

		//把右边找到的比key小的数填到左边的坑里
		a[left] = a[right];
	}
	a[left] = key;
	
	return left;
}

// 快速排序前后指针法
int PartSort3(int* a, int left, int right)		
{
	int midindex = GetMidIndex(a, left, right);//三数取中,规避掉最大和最小，让最坏的情况不在发生
	Swap(&a[midindex], &a[right]);

	int keyindex = right;

	int prev = left - 1;
	int cur = left;

	while (cur < right)
	{
		if (a[cur] < a[keyindex] && ++prev != cur)
			Swap(&a[cur], &a[prev]);
		cur++;
	}
		
	Swap(&a[++prev], &a[keyindex]);
	return prev;
}

//时间复杂度：O(N*logN)
//空间复杂度：O(logN)
void QuickSort(int* a, int left, int right)
{
	assert(a);

	if (left >= right)
		return;

	if ((right - left + 1) > 10)
	{
		int div = PartSort3(a, left, right);

		//[left,div-1]div[div+1,right]
		QuickSort(a, left, div - 1);
		QuickSort(a, div + 1, right);
	}
	else //小区间使用插入排序
		InsertSort(a + left, right - left + 1);
}

// 快速排序 非递归实现
//递归改非递归————1.改成循环（斐波那契数列）一些简单递归才能改循环  2.栈模拟储存数据非递归
//非递归：1.提高效率（递归建立栈帧是有消耗的，但是对于现代计算机，这些消耗可以忽略不记）
//		 2.递归的最大缺陷是，如果栈帧的深度太深，可能会导致栈溢出，因为系统栈空间一般不大，在M级别
//		   数据结构栈模拟非递归，数据是存储在堆上的，堆是G级别的空间
void QuickSortNonR(int* a, int left, int right)
{
	//栈模拟实现
	Stack st;
	StackInit(&st);

	StackPush(&st, right);
	StackPush(&st, left);

	while (!StackEmpty(&st))
	{
		int begin = StackTop(&st);
		StackPop(&st);
		int end = StackTop(&st);
		StackPop(&st);

		//[begin,end]
		int div = PartSort1(a, begin, end);
		//[begin,div-1] div [div+1,end]

		if (div + 1 < end)
		{
			StackPush(&st, end);
			StackPush(&st, div + 1);
		}

		if (begin < div - 1)
		{
			StackPush(&st, div - 1);
			StackPush(&st, begin);
		}
	}
	StackDestory(&st);
}

void MergeArr(int* a, int begin1, int end1, int begin2, int end2, int* tmp)
{
	int left = begin1, right = end2;
	int index = left;

	while (begin1 <= end1 && begin2 <= end2)
	{
		if (a[begin1] < a[begin2])
			tmp[index++] = a[begin1++];
		else
			tmp[index++] = a[begin2++];
	}

	while (begin1 <= end1)
		tmp[index++] = a[begin1++];
	while (begin2 <= end2)
		tmp[index++] = a[begin2++];

	//把归并好的数组拷贝到原数组
	for (int i = left; i <= right; i++)
		a[i] = tmp[i];
}

//时间复杂度：O(N*logN)
//空间复杂度：O(N)
void _MergeSort(int* a, int left, int right, int* tmp)
{
	if (left >= right)
		return;

	int mid = (left + right) / 2;
	//[left,mid][mid+1,right]有序，则可以合并，现在他们没有序，子问题解决

	_MergeSort(a, left, mid, tmp);
	_MergeSort(a, mid+1,right, tmp);

	//归并[left,mid][mid+1,right]有序
	int begin1 = left, end1 = mid;
	int begin2 = mid + 1, end2 = right;
	MergeArr(a, begin1, end1, begin2, end2, tmp);
}

// 归并排序递归实现
void MergeSort(int* a, int n)
{
	assert(a);
	int* tmp = (int*)malloc(sizeof(int) * n);

	_MergeSort(a, 0, n - 1, tmp);
	free(tmp);
}

// 归并排序非递归实现
void MergeSortNonR(int* a, int n)
{
	assert(a);
	int* tmp = (int*)malloc(sizeof(int) * n);
	
	int gap = 1;

	while (gap < n)
	{
		for (int i = 0; i < n; i += 2 * gap)
		{
			//[i,i+gap-1][i+gap,i+2*gap-1]
			int begin1 = i, end1 = i + gap - 1;
			int begin2 = i + gap, end2 = i + 2 * gap - 1;

			//1.合并时只有第一组，就不需要合并
			if (begin2>=n)
				break;
			//2.合并时第二组只有部分数据，需要修正end2边界
			if (end2 >= n)
				end2 = n - 1;

			MergeArr(a, begin1, end1, begin2, end2, tmp);
		}
		gap *= 2;
	}
	free(tmp);
}

void _MergeFile(const char* file1,const char* file2,const char* mfile)
{
	FILE* fout1 = fopen(file1, "r");
	if (!fout1)
		return;

	FILE* fout2 = fopen(file2, "r");
	if (!fout2)
		return;

	FILE* fin = fopen(mfile, "w");
	if (!fin)
		return;

	int num1, num2;
	int ret1 = fscanf(fout1, "%d\n", &num1);
	int ret2 = fscanf(fout2, "%d\n", &num2);

	while ( ret1!= EOF && ret2 != EOF)
	{
		if (num1 < num2)
		{
			fprintf(fin, "%d\n", num1);
			ret1 = fscanf(fout1, "%d\n", &num1);
		}
		else
		{
			fprintf(fin, "%d\n", num2);
			ret2 = fscanf(fout2, "%d\n", &num2);
		}
	}
	
	while (ret1 != EOF)
	{
		fprintf(fin, "%d\n", num1);
		ret1 = fscanf(fout1, "%d\n", &num1);
	}

	while (ret2 != EOF)
	{
		fprintf(fin, "%d\n", num2);
		ret2 = fscanf(fout2, "%d\n", &num2);
	}

	fclose(fout1);
	fclose(fout2);
	fclose(fin);
}

void MergeSortFile(const char* file)
{
	FILE* fout = fopen(file, "r");
	if (!fout)
		return;

	//分割成一段一段的数据，内存排序后，写到小文件
	int n = 10;
	int num = 0;
	int filei = 1;
	int a[10];
	char subfile[20];
	int i = 0;
	memset(a, 0, sizeof(int) * n);

	while (fscanf(fout, "%d\n", &num) != EOF)
	{
		if (i < n - 1)
			a[i++] = num;
		else
		{
			a[i] = num;
			QuickSort(a, 0, n - 1);
			sprintf(subfile, "%d", filei++);
			FILE* fin = fopen(subfile, "w");
			if (!fin)
				return;
			for (int i = 0; i < n; i++)
				fprintf(fin, "%d\n", a[i]);
			fclose(fin);
			i = 0;
			memset(a, 0, sizeof(int) * n);
		}
	}

	//利用互相归并到文件，实现整体有序
	char mfile[100] = "12";
	char file1[100] = "1";
	char file2[100] = "2";
	for (int i = 2; i <= n; i++)
	{
		//读取file1和file2，进行归并成mfile
		_MergeFile(file1, file2, mfile);

		strcpy(file1, mfile);
		sprintf(file2, "%d", i + 1);
		sprintf(mfile, "%s%d", mfile, i + 1);
	}

	fclose(fout);
}

// 计数排序
//时间复杂度：O(N+range)
//空间复杂度：O(range)
//只适应于整形
void CountSort(int* a, int n)
{
	int min, max;
	min = max = a[0];

	for (int i = 1; i < n; i++)
	{
		if (a[i] > max)
			max = a[i];
		if (a[i] < min)
			min = a[i];
	}

	int range = max - min + 1;
	int* countArr = (int*)malloc(sizeof(int) * range);
	memset(countArr, 0, sizeof(int) * range);

	//统计次数
	for (int i = 0; i < n; i++)
		countArr[a[i]-min]++;

	//排序
	int index = 0;
	for (int j = 0; j < range; j++)
	{
		while (countArr[j]--)
			a[index++] = j+min;
	}

	free(countArr);
}