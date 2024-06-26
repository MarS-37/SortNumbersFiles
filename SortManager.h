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
	static void RunSort(const std::string& filename, std::string& resultfilename);

private:
	template <typename T>
	static void Merge(T* arr, int low, int mid, int high);
};