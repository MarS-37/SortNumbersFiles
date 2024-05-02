//+------------------------------------------------------------------+
//|                         SortNumbersFiles                         |
//|       tutorial program for sorting numbers in large files        |
//|                                                                  |
//|                                      Copyright 2024, Mars37 Ltd. |
//|                                       markin.sergey.37@yandex.ru |
//+------------------------------------------------------------------+


#include <chrono>
#include <string>
#include <ctime>
#include "FileManager.h"
#include "SortManager.h"


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
	SortManager::RunSort(sourceFileName, resultFileName);

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