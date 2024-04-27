//+------------------------------------------------------------------+
//|                         SortNumberFiles                          |
//|        tutorial program for sorting numbers in large files       |
//|                                                                  |
//|                                      Copyright 2024, Mars37 Ltd. |
//|                                       markin.sergey.37@yandex.ru |
//+------------------------------------------------------------------+

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>


class FileManager
{
public:
	static void CreateRandFile(const std::string& filename)
	{
		// use ofstream directly for file creation
		std::ofstream fs(filename);

		if (!fs.is_open()) {
			throw std::runtime_error("Failed to create file.");
		}

		srand(static_cast<unsigned int>(time(nullptr)));

		const int low = -1'000;
		const int high = 4'000;

		for (int i = 0; i < LINE_IN_FILE; i++) {
			fs << (low + rand() % high) << std::endl;
		}

		std::cout << "File " << filename << " created\n";
		std::cout << "File " << filename << " is full\n";
	}
	static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
	{
		std::fstream temp;

		const int* first;
		const int* second;

		temp.open("tmp1.txt", std::fstream::out | std::ofstream::trunc);


		if (arr1[0] < arr2[0]) {
			first = arr1;
			second = arr2;
		}
		else {
			first = arr2;
			second = arr1;
			std::swap(elements1, elements2);
		}

		if (temp.is_open())
		{
			int i = 0;
			int j = 0;

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

			temp.close();
		}
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
				temp1 >> temp1_value;
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
		arr.reset(new int[TMP_BLOCK]);
		int* tmp_arr;
		int i;
		for (i = 0; i < TMP_BLOCK && !fs.eof(); i++)
			fs >> arr[i];

		if (i == 1) {
			arr.reset();
			return 0;
		}

		if (i != TMP_BLOCK) {
			tmp_arr = new int[i];
			for (size_t j = 0; j < i; j++)
				tmp_arr[j] = arr[j];

			arr.reset(tmp_arr);
			return i - 1;
		}

		return TMP_BLOCK;
	}
	static void DeletedFiles()
	{
		std::filesystem::remove("tmp1.txt");
		std::filesystem::remove("tmp2.txt");
	}

private:
	// size constants
	static constexpr int LINE_IN_FILE = 500'000;
	static constexpr int TMP_BLOCK = 500'000;
};


class SortingManager
{
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
	static void RunSort(const std::string& filename, std::string& resultfilename)
	{
		static int processed = 0;

		std::fstream fs;
		fs.open(filename, std::fstream::in);

		if (!fs.is_open()) {
			throw std::runtime_error("Failed to open file for reading.");
		}

		while (!fs.eof()) {
			std::unique_ptr<int[]> part1;
			std::unique_ptr<int[]> part2;

			int size1 = FileManager::ReadTempBlock(fs, part1);
			int size2 = FileManager::ReadTempBlock(fs, part2);

			if (size1 == 0 || size2 == 0) {
				return;
			}

			processed += size1 + size2;
			std::cout << " string processing = " << processed << std::endl;

			MergeSort(part1.get(), 0, size1 - 1);
			MergeSort(part2.get(), 0, size2 - 1);

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

	FileManager::CreateRandFile(sourceFileName);

	// creating an empty file for the result
	std::fstream res;
	res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
	res.close();

	return EXIT_SUCCESS;
}