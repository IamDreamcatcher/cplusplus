#include "array.h"
#include <algorithm>

int SumOfArray(int a[], int size) 
{
    int sum = 0;
    for (int i = 0; i < size; i++) 
    {
        sum += a[i];
    }
    return sum;
}

int MinInArray(int a[], int size)
{
    if (size == 0) {
        return 0;
    }
    int minimum = a[0];
    
    for (int i = 1; i < size; i++) 
    {
        minimum = std::min(a[i], minimum);
    }
    return minimum; 
}

int MaxInArray(int a[], int size)
{    
    if (size == 0) {
        return 0;
    }
    int maximum = a[0];
    
    for (int i = 1; i < size; i++) 
    {
        maximum = std::max(a[i], maximum);
    }
    return maximum;
}

double Average(int a[], int size)
{    
    if (size == 0) {
        return 0;
    }
    int sum = SumOfArray(a, size);
    
    return 1.0 * sum / size;
}