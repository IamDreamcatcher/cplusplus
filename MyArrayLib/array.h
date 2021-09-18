#pragma once

#ifdef ARRAYDLL_EXPORTS
#define DLLIMPORT_EXPORT __declspec(dllexport)
#else
#define DLLIMPORT_EXPORT __declspec(dllimport)
#endif

extern "C"
{
    DLLIMPORT_EXPORT  int _stdcall SumOfArray(int a[], int size);
    DLLIMPORT_EXPORT  int _cdecl MinInArray(int a[], int size);
    DLLIMPORT_EXPORT  int _cdecl MaxInArray(int a[], int size);
    DLLIMPORT_EXPORT  double _stdcall Average(int a[], int size);
}