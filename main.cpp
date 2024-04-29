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
#define LINE_IN_FILE 2'000'000
#define TMP_BLOCK    500'000


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
        // файловая переменная
        std::fstream fs;

        // инициализация генератора
        srand(static_cast<unsigned int>(time(NULL)));

        // константы диапазона
        const int low = -1'000;
        const int high = 4'000;

        // открытие фала
        fs.open(filename, std::fstream::out | std::ofstream::trunc);

        // проверка открытия файла
        if (fs.is_open()) {
            std::cout << "File " << filename << " created\n";

            // цикл наполнения файла числами
            for (int i = 0; i < LINE_IN_FILE; ++i) {
                fs << (low + rand() % high) << std::endl;
            }

            // закрытие файла
            fs.close();

            std::cout << "File " << filename << " full\n";
        }
    }
    static void MergeToFile(const int* arr1, const int* arr2, int elements1, int elements2)
    {

    }
    static void MergeFiles(const std::string& resultfilename)
    {

    }
    static int ReadTempBlock(std::fstream& fs, std::unique_ptr<int[]>& arr)
    {

    }
    static void DeletedFiles()
    {

    }
};


class SortingManager
{
public:
    static void RunSort(const std::string& filename, std::string& resultfilename)
    {

    }
};


int main()
{
    std::string sourceFileName = "unsorted_file.txt";
    std::string resultFileName = "sorted_file.txt";

    auto start = std::chrono::high_resolution_clock::now();

    // creating a file
    FileManager::CreateRandFile(sourceFileName);

    // time tracking
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> durations = finish - start;
    std::cout << "File creation time: " << (durations.count()) / 60 << " min" << std::endl;

    std::fstream res;
    res.open(resultFileName, std::fstream::out | std::ofstream::trunc);
    res.close();

    // file sorting
    // time tracking
    start = std::chrono::high_resolution_clock::now();
    SortingManager::RunSort(sourceFileName, resultFileName);

    // time tracking
    finish = std::chrono::high_resolution_clock::now();
    durations = finish - start;
    std::cout << "Sorting time: " << (durations.count()) / 60 << " min" << std::endl;

    // deletion request
    char comm;
    std::cout << "Delete temporary files? (y/n): ";
    std::cin >> comm;
    if (comm == 'y' or comm == 'Y') {
        // Delete temporary files
        FileManager::DeletedFiles();
    }

    return 0;
}