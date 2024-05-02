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
void SortManager::RunSort(const std::string& filename, std::string& resultfilename)
{
	// static process indicator variable
	static int processed = 0;

	// file variable
	std::fstream fs;

	// opening a file to read from
	fs.open(filename, std::fstream::in);

	// file opening check
	if (!fs.is_open()) {
		throw std::runtime_error("Failed to open file for reading.");
	}

	// loop to the end file
	while (!fs.eof()) {
		// number block pointers
		std::unique_ptr<int[]> part1;
		std::unique_ptr<int[]> part2;

		// pointer to number blocks and their size
		int size1 = FileManager::ReadTempBlock(fs, part1);
		int size2 = FileManager::ReadTempBlock(fs, part2);

		// the blocks are gone
		if (size1 == 0 || size2 == 0) {
			return;
		}

		// for process output
		processed += size1 + size2;
		std::cout << " string processing = " << processed << std::endl;

		// implementation of sorting by merge
		MergeSort(part1.get(), 0, size1 - 1);
		MergeSort(part2.get(), 0, size2 - 1);

		// merge two sorted arrays
		FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
		FileManager::MergeFiles(resultfilename);
	}

	fs.close();
}
template <typename T>
void SortManager::Merge(T* arr, int low, int mid, int high)
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