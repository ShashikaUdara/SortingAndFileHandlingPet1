#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

using namespace std;


void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d --> %d \n", i, arr[i]);
    printf("\n");
}

void writeToFile(int arr[], int size)
{
	// Main Output: \t logical thread number \t start offset \t end offset \n
	int i, fdw, lineCount = 0, ret, writeSizeOld = 0, j;
	char tempStr1[10] = {0};
	char tempStr2[10] = {0};
	char tempBuff[50] = {0};

	char wfl_nm[]={"apple.txt"};

	fdw = open(wfl_nm, O_RDWR|O_CREAT, 0777);
	if(fdw < 0)
    {
        perror("open failed");
    }


	for(i=0;i<size; i++)
	{
		if(i%1000 == 0)
		{
			sprintf(tempStr1, "%d", arr[i]);
		}

		if(i%1000 == 999)
		{
			sprintf(tempStr2, "%d", arr[i]);
			lineCount++;
			sprintf(tempBuff, "Main Output: \t %d \t %s \t %s \n", lineCount, tempStr1, tempStr2);
			// cout<<tempBuff<<endl;

			int writeSize = sizeof(tempBuff)/sizeof(tempBuff[0]);
			if((ret = pwrite(fdw, &tempBuff, writeSize, writeSizeOld)) == -1)
    		{
    		    perror("pwrite failed");
    		}
    		else
    		{
    			writeSizeOld += writeSize;
    			for(j=0; j<50; j++)
    			{
    				tempBuff[j] = '\0';
    			}
    		}
		}
	}
    // close(fdw);
    if(close(fdw) == -1)
    {
        perror("[error in close: writeToFile]\n");
    }
    else
    {
        printf("[succeeded in close]\n");
    }
}

int main()
{
	int fd, i, countL, letterCount;
	char rfl_nm[]={"numberList.txt"};
	fd = open(rfl_nm, O_RDONLY);
	if(fd < 0)
    {
        perror("open failed");
        return 1;
    }

    // lseek(fd, 0, SEEK_END);

    char buf[60000] = {0};
    char tempBuf[10] = {0};
    int numBuf[10000];

    int offset = 0;
    ssize_t ret = 0;
    int count = 100;
    count = lseek(fd, 0, SEEK_END);
    if((ret = pread(fd, buf, count, offset)) == -1)
    {
        perror("pread failed");
        return 1;
    }

    if(close(fd) == -1)
    {
        perror("[error in close :main]\n");
    }
    else
    {
        printf("[succeeded in close]\n");
    }

    int buffLength = sizeof(buf)/sizeof(buf[0]);
    // cout << "read buf = " << buf << endl;
    // cout<<"buffLength: "<<buffLength<<endl;
    
    for(i = 0; i<buffLength; i++)
    {
    	if((char)buf[i] != '\n')
    	{
    		tempBuf[letterCount] = (char)buf[i];
    		letterCount++;
    	}
    	else
    	{
    		letterCount = 0;
    		numBuf[countL] = atoi(tempBuf);
    		// cout<<"numBuf: "<<numBuf[countL]<<endl;
    		countL++;
    	}
    }

    quickSort(numBuf, 0, 10000);
    printArray(numBuf, 10000);

    writeToFile(numBuf, 10000);

	return 0;
}