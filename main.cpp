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