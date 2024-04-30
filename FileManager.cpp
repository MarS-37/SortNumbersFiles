#include "FileManager.h"


void FileManager::CreateRandFile(const std::string& filename)
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
void FileManager::MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
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