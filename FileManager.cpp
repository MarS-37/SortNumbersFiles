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