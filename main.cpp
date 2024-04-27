#include <iostream>
#include <fstream>


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

private:
	// size constants
	static constexpr int LINE_IN_FILE = 500'000;
	static constexpr int TMP_BLOCK = 500'000;
};


int main()
{
    std::string sourceFileName = "unsorted_file.txt";
    std::string resultFileName = "sorted_file.txt";

    FileManager::CreateRandFile(sourceFileName);

	return EXIT_SUCCESS;
}