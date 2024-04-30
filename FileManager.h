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
