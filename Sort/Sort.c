#include"Sort.h"
#include"Stack.h"


//����
void Swap(int* p1, int* p2)
{
	int tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

//����ȡ��
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

// �������� O(N^2)
void InsertSort(int* a, int n)
{
	assert(a);

	for (int i = 0; i < n - 1; i++)//ע��n-2�ı߽�
	{
		//��end+1�����ݲ���[0,end+1]������ռ�
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

// ϣ������ O(N^1.3����N^2)
void ShellSort(int* a, int n)
{
	assert(a);

	//1.gap>1�൱��Ԥ���У�������ӽ�����
	//2.gap==1���൱��ֱ�Ӳ������У���֤����
	int gap=n;
	while (gap > 1)
	{
		gap = gap / 3 + 1;	//+1��֤���һ��gapһ����1
							//gap==1�൱��ֱ�Ӳ�������

		for (int i = 0; i < n - gap; i++)//���鲢��
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

// ѡ������ O(N^2)
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
		//���maxi��beginλ���ص�����maxi��λ����Ҫ����
		if (begin == maxi)
			maxi = mini;
		Swap(&a[end], &a[maxi]);

		begin++;
		end--;
	}
}

// ������
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
//ʱ�临�Ӷ� O(N*logN)
void HeapSort(int* a, int n)
{	//���򽨴��
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

// ð������ O(N^2)
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

		//���һ��ð�ݵĹ�����û�з�����������˵��ǰ�����Ѿ����򣬲���Ҫ����ð��
		if (exchange == 0)
			break;
		end--;
	}
}

// ��������ݹ�ʵ��
// ��������hoare�汾(����ָ�뷨) 
int PartSort1(int* a, int left, int right)//[left,right]
{
	int midindex = GetMidIndex(a,left, right);//����ȡ��,��ܵ�������С�������������ڷ���
	Swap(&a[midindex], &a[right]);

	int keyindex = right;	//keyѡ���ұߵģ���left���ߣ��������Ա�֤����ص�ͣ�µ�λ���Ǳ�key�����(left�ҵ�����)��
							//��key������������߶���keyС���ұ߱�key��
	
	while (left < right)
	{
		while (left < right && a[left] <= a[keyindex])//û�е��ڻ������ѭ��
			left++;

		while (left < right && a[right] >= a[keyindex])
			right--;

		Swap(&a[left], &a[right]);
	}
	Swap(&a[left], &a[keyindex]);
	return left;
}

// ���������ڿӷ�  ������ָ�뷨��ȣ��ڿ���Ӿ��൱���ǰѽ�����ֿ�
int PartSort2(int* a, int left, int right)
{
	int midindex = GetMidIndex(a, left, right);//����ȡ��,��ܵ�������С�������������ڷ���
	Swap(&a[midindex], &a[right]);

	//��
	int key = a[right];

	while (left < right)
	{
		while (left < right && a[left] <= key)
			left++;

		//������ҵ��ı�key�������ұߵĿ���
		a[right] = a[left];

		while (left < right && a[right] >= key)
			right--;

		//���ұ��ҵ��ı�keyС�������ߵĿ���
		a[left] = a[right];
	}
	a[left] = key;
	
	return left;
}

// ��������ǰ��ָ�뷨
int PartSort3(int* a, int left, int right)		
{
	int midindex = GetMidIndex(a, left, right);//����ȡ��,��ܵ�������С�������������ڷ���
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

//ʱ�临�Ӷȣ�O(N*logN)
//�ռ临�Ӷȣ�O(logN)
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
	else //С����ʹ�ò�������
		InsertSort(a + left, right - left + 1);
}

// �������� �ǵݹ�ʵ��
//�ݹ�ķǵݹ顪������1.�ĳ�ѭ����쳲��������У�һЩ�򵥵ݹ���ܸ�ѭ��  2.ջģ�ⴢ�����ݷǵݹ�
//�ǵݹ飺1.���Ч�ʣ��ݹ齨��ջ֡�������ĵģ����Ƕ����ִ����������Щ���Ŀ��Ժ��Բ��ǣ�
//		 2.�ݹ�����ȱ���ǣ����ջ֡�����̫����ܻᵼ��ջ�������Ϊϵͳջ�ռ�һ�㲻����M����
//		   ���ݽṹջģ��ǵݹ飬�����Ǵ洢�ڶ��ϵģ�����G����Ŀռ�
void QuickSortNonR(int* a, int left, int right)
{
	//ջģ��ʵ��
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

	//�ѹ鲢�õ����鿽����ԭ����
	for (int i = left; i <= right; i++)
		a[i] = tmp[i];
}

//ʱ�临�Ӷȣ�O(N*logN)
//�ռ临�Ӷȣ�O(N)
void _MergeSort(int* a, int left, int right, int* tmp)
{
	if (left >= right)
		return;

	int mid = (left + right) / 2;
	//[left,mid][mid+1,right]��������Ժϲ�����������û������������

	_MergeSort(a, left, mid, tmp);
	_MergeSort(a, mid+1,right, tmp);

	//�鲢[left,mid][mid+1,right]����
	int begin1 = left, end1 = mid;
	int begin2 = mid + 1, end2 = right;
	MergeArr(a, begin1, end1, begin2, end2, tmp);
}

// �鲢����ݹ�ʵ��
void MergeSort(int* a, int n)
{
	assert(a);
	int* tmp = (int*)malloc(sizeof(int) * n);

	_MergeSort(a, 0, n - 1, tmp);
	free(tmp);
}

// �鲢����ǵݹ�ʵ��
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

			//1.�ϲ�ʱֻ�е�һ�飬�Ͳ���Ҫ�ϲ�
			if (begin2>=n)
				break;
			//2.�ϲ�ʱ�ڶ���ֻ�в������ݣ���Ҫ����end2�߽�
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

	//�ָ��һ��һ�ε����ݣ��ڴ������д��С�ļ�
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

	//���û���鲢���ļ���ʵ����������
	char mfile[100] = "12";
	char file1[100] = "1";
	char file2[100] = "2";
	for (int i = 2; i <= n; i++)
	{
		//��ȡfile1��file2�����й鲢��mfile
		_MergeFile(file1, file2, mfile);

		strcpy(file1, mfile);
		sprintf(file2, "%d", i + 1);
		sprintf(mfile, "%s%d", mfile, i + 1);
	}

	fclose(fout);
}

// ��������
//ʱ�临�Ӷȣ�O(N+range)
//�ռ临�Ӷȣ�O(range)
//ֻ��Ӧ������
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

	//ͳ�ƴ���
	for (int i = 0; i < n; i++)
		countArr[a[i]-min]++;

	//����
	int index = 0;
	for (int j = 0; j < range; j++)
	{
		while (countArr[j]--)
			a[index++] = j+min;
	}

	free(countArr);
}