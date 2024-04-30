#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>


// size constants
#define LINE_IN_FILE	20'000'000
#define TMP_BLOCK		5'000'000


class FileManager
{
public:
	static void CreateRandFile(const std::string& filename);
	static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2);
	static void MergeFiles(const std::string& resultfilename);
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
