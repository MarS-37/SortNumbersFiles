#include "FileManager.h"
#include "SortManager.h"


template <typename T>
void SortManager::MergeSort(T* arr, int low, int high)
{
	// algorithm condition
	if (low < high) {
		// find the middle of the array
		int mid = low + (high - low) / 2;

		// recursively sort left half of the array
		MergeSort(arr, low, mid);

		// recursively sort right half of the array
		MergeSort(arr, mid + 1, high);

		// merge sorted halves of the array
		//Merge(arr, low, mid, high);
	}
}