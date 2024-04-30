//+------------------------------------------------------------------+
//|                         SortNumbersFiles                         |
//|       tutorial program for sorting numbers in large files        |
//|                                                                  |
//|                                      Copyright 2024, Mars37 Ltd. |
//|                                       markin.sergey.37@yandex.ru |
//+------------------------------------------------------------------+


#include <string>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>


// size constants
#define LINE_IN_FILE	20'000'000
#define TMP_BLOCK		5'000'000


class FileManager
{
public:
	static void CreateRandFile(const std::string& filename)
	{
		// file variable
		std::fstream fs;

		// random number generator initialization
		srand(static_cast<unsigned int>(time(nullptr)));

		// selection range constant
		const int low = -1'000;
		const int high = 4'000;

		// open the file for writing to
		fs.open(filename, std::fstream::out | std::ofstream::trunc);

		// file opening check
		if (!fs.is_open()) {
			throw std::runtime_error("Failed to open file for writing");
		}

		std::cout << "File " << filename << " created\n";

		// fill the file with numbers		
		for (int i = 0; i < LINE_IN_FILE; i++) {
			fs << (low + rand() % high) << std::endl;
		}

		// close file
		fs.close();

		std::cout << "File " << filename << " is full\n";
	}
	static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
	{
		// file variable
		std::fstream temp;

		// array pointers
		const int* first;
		const int* second;

		temp.open("tmp1.txt", std::fstream::out | std::ofstream::trunc);

		// selection of the first and second arrays 
		// depending on the value of the first element
		if (arr1[0] < arr2[0]) {
			first = arr1;
			second = arr2;
		}
		else {
			first = arr2;
			second = arr1;
			std::swap(elements1, elements2);
		}

		// file opening check
		if (!temp.is_open())
		{
			throw std::runtime_error("Failed to open temp file for writing");
		}

		// value position
		int i = 0;
		int j = 0;

		// sorting
		while (i < elements1 && j < elements2) {
			if (first[i] < second[j]) {
				temp << first[i++] << std::endl;
			}
			else if (first[i] == second[j]) {
				temp << first[i++] << std::endl;
				temp << second[j++] << std::endl;
			}
			else {
				temp << second[j++] << std::endl;
			}
		}
		while (i < elements1) {
			temp << first[i++] << std::endl;
		}
		while (j < elements2) {
			temp << second[j++] << std::endl;
		}

		// close file
		temp.close();

	}
	static void MergeFiles(const std::string& resultfilename)
	{
		// files variables
		std::fstream res;
		std::fstream temp1;
		std::fstream temp2;

		// temporary file name
		const std::string& tmp1 = "tmp1.txt";
		const std::string& tmp2 = "tmp2.txt";

		// files opening
		temp1.open(tmp1, std::fstream::in);
		res.open(resultfilename, std::fstream::in);
		temp2.open(tmp2, std::fstream::out | std::ofstream::trunc);

		// if at least one file is not open
		if (!temp1.is_open() || !temp2.is_open() || !res.is_open()) {
			return;
		}

		// temp values
		int temp1_value;
		int res_value;


		temp1 >> temp1_value;
		res >> res_value;
		while (!temp1.eof() && !res.eof()) {
			if (temp1_value <= res_value) {
				temp2 << temp1_value << std::endl;
				temp1 >> temp1_value;
			}
			else {
				temp2 << res_value << std::endl;
				res >> res_value;
			}
		}

		// files merge
		while (!res.eof()) {
			temp2 << res_value << std::endl;
			res >> res_value;
		}

		while (!temp1.eof()) {
			temp2 << temp1_value << std::endl;
			temp1 >> temp1_value;
		}

		// files close
		temp1.close();
		temp2.close();
		res.close();

		// copies the contents of the temporary file to the resulting file
		if (!std::filesystem::copy_file("tmp2.txt", resultfilename,
			std::filesystem::copy_options::overwrite_existing)) {
			return;
		}
	}
	static int ReadTempBlock(std::fstream& fs, std::unique_ptr<int[]>& arr)
	{
		// allocate memory for an array of size TMP_BLOCK
		arr.reset(new int[TMP_BLOCK]);

		// temp array pointer
		int* tmp_arr;

		// block position
		int i;
		// loop until the block is read the file ends
		for (i = 0; i < TMP_BLOCK && !fs.eof(); i++) {
			fs >> arr[i];
		}

		// block empty
		if (i == 1) {
			// allocate memory
			arr.reset();

			return 0;
		}

		// if the block is smaller, we process it differently
		if (i != TMP_BLOCK) {
			// allocate memory for the temp array
			// by the amount of data in the block
			tmp_arr = new int[i];

			// copy data into a temporary array
			for (size_t j = 0; j < i; j++) {
				tmp_arr[j] = arr[j];
			}

			// update memory size
			arr.reset(tmp_arr);

			// the block size
			return i - 1;
		}

		// the block size
		return TMP_BLOCK;
	}
	static void DeletedFiles()
	{
		std::filesystem::remove("tmp1.txt");
		std::filesystem::remove("tmp2.txt");
	}
};


class SortingManager
{
public:
	template <typename T>
	static void MergeSort(T* arr, int low, int high)
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
			Merge(arr, low, mid, high);
		}
	}
	static void RunSort(const std::string& filename, std::string& resultfilename)
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


int main()
{
	std::string sourceFileName = "unsorted_file.txt";
	std::string resultFileName = "sorted_file.txt";

	// time tracking start
	auto start = std::chrono::high_resolution_clock::now();

	// creating a source file 
	FileManager::CreateRandFile(sourceFileName);

	// time tracking finish
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> durations = finish - start;
	std::cout << "File creation time: " << (durations.count()) / 60 << " min" << std::endl;

	// creating a result file
	std::fstream res;
	res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
	res.close();

	// time tracking start
	start = std::chrono::high_resolution_clock::now();

	// file sorting
	SortingManager::RunSort(sourceFileName, resultFileName);

	// time tracing finish
	finish = std::chrono::high_resolution_clock::now();
	durations = finish - start;
	std::cout << "Sorting time: " << (durations.count()) / 60 << " min" << std::endl;

	// deletion request
	char choice;
	std::cout << "Delete temporary files? (y/n): ";
	std::cin >> choice;
	if (choice == 'y' or choice == 'Y') {
		// Delete temporary files
		FileManager::DeletedFiles();
	}

	return 0;
}