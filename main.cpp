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
#define LINE_IN_FILE 20
#define TMP_BLOCK    5


class Sorting
{
public:
	template <typename T>
	static void MergeSort(T* arr, int low, int high)
	{
		if (low < high) {
			int mid = low + (high - low) / 2;
			MergeSort(arr, low, mid);
			MergeSort(arr, mid + 1, high);
			Merge(arr, low, mid, high);
		}
	}

private:
	template <typename T>
	static void Merge(T* arr, int low, int mid, int high)
	{
		int n1 = mid - low + 1;
		int n2 = high - mid;

		T* left = new T[n1];
		T* right = new T[n2];

		for (int i = 0; i < n1; i++)
			left[i] = arr[low + i];
		for (int j = 0; j < n2; j++)
			right[j] = arr[mid + 1 + j];

		int i = 0, j = 0, k = low;

		while (i < n1 && j < n2) {
			if (left[i] <= right[j]) {
				arr[k++] = left[i++];
			}
			else {
				arr[k++] = right[j++];
			}
		}

		while (i < n1) {
			arr[k++] = left[i++];
		}

		while (j < n2) {
			arr[k++] = right[j++];
		}

		delete[] left;
		delete[] right;
	}
};


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
		// переменая для работы с временным файлом
		std::fstream temp;

		// константы для сортировки двух чисел
		const int* first;
		const int* second;

		if (arr1[0] < arr2[0]) {
		    // константы получают значение
		    first = arr1;
		    second = arr2;
		}

		// открытие файла
		temp.open("tmp1.txt", std::fstream::out | std::ofstream::trunc);

		// определяем очередность массивов
		if (arr1[0] < arr2[0]) {
			first = arr1;
			second = arr2;
		}
		else {
			first = arr2;
			second = arr1;
			std::swap(elements1, elements2);
		}

		// проверка открытия файла
		if (!temp.is_open()) {
			// исключение runtime_error
			throw std::runtime_error("Failed to open file for reading.");
		}

		// переменные обхода блоков
		int i = 0;
		int j = 0;

		// цикл слияния массивов
		while (i < elements1 && j < elements2) {
			// значение 1 меньше значения 2
			if (first[i] < second[j]) {
				// пишем в txt1.txt
				temp << first[i++] << std::endl;
			}
			// если значения равны
			else if(first[i] == second[j]) {

			}
			else {
				temp << second[j++] << std::endl;
			}
		}
		// если в массиве остались элементы
		while (i < elements1) {
			temp << first[i++] << std::endl;
		}
		// если в массиве остались элементы
		while (j < elements2) {
			temp << second[j++] << std::endl;
		}

		temp.close();
	}
	static void MergeFiles(const std::string& resultfilename)
	{
		std::fstream res;
		std::fstream temp1;
		std::fstream temp2;

		const std::string& tmp1 = "tmp1.txt";
		const std::string& tmp2 = "tmp2.txt";

		temp1.open(tmp1, std::fstream::in);
		res.open(resultfilename, std::fstream::in);
		temp2.open(tmp2, std::fstream::out | std::ofstream::trunc);

		if (!temp1.is_open() || !temp2.is_open() || !res.is_open()) {
			return;
		}

		int temp1_value;
		int res_value;

		temp1 >> temp1_value;
		res >> res_value;

		while (!temp1.eof() && !res.eof()) {
			if (temp1_value <= res_value) {
				temp2 << temp1_value << std::endl;
				temp1 >> res_value;
			}
			else {
				temp2 << res_value << std::endl;
				res >> res_value;
			}
		}

		while (!res.eof()) {
			temp2 << res_value << std::endl;
			res >> res_value;
		}

		while (!temp1.eof()) {
			temp2 << temp1_value << std::endl;
			temp1 >> temp1_value;
		}

		temp1.close();
		temp2.close();
		res.close();

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

			processed += size1 + size2;
			std::cout << " string processing = " << processed << std::endl;

			Sorting::MergeSort(part1.get(), 0, size1 - 1);
			Sorting::MergeSort(part2.get(), 0, size2 - 1);

			FileManager::MergeToFile(part1.get(), part2.get(), size1, size2);
			FileManager::MergeFiles(resultfilename);
		}

		fs.close();
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