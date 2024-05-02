#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <memory>


class SortManager
{
public:
	template <typename T>
	static void MergeSort(T* arr, int low, int high);
	

private:
	template <typename T>
	static void Merge(T* arr, int low, int mid, int high)
	{
		// calculate dimensions of the halves
		int n1 = mid - low + 1;
		int n2 = high - mid;

		// create temporary arrays to store halves
		T* left = new T[n1];
		T* right = new T[n2];

		// copy data to temporary arrays
		for (int i = 0; i < n1; i++)
			left[i] = arr[low + i];
		for (int j = 0; j < n2; j++)
			right[j] = arr[mid + 1 + j];

		// merge temporary arrays back into the main array
		int i = 0, j = 0, k = low;
		while (i < n1 && j < n2) {
			if (left[i] <= right[j]) {
				arr[k++] = left[i++];
			}
			else {
				arr[k++] = right[j++];
			}
		}

		// copy the remaining elements from the left array
		while (i < n1) {
			arr[k++] = left[i++];
		}

		// copy the remaining elements from the right array
		while (j < n2) {
			arr[k++] = right[j++];
		}

		// free the allocated memory
		delete[] left;
		delete[] right;
	}
};

