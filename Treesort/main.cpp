#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include "treesort.h"

static constexpr size_t INTS = 1000 * 1000;
static constexpr int WIDTH = 10;

static int* createRandomIntegerArray(size_t size, int width)
{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist(1, width);
    
    int* arr = new int[size];
    
    for (auto i = 0; i < size; ++i)
    {
        arr[i] = dist(e);
    }
    
    return arr;
}

static void profileOnIntegerArray()
{
    int* arr1 = createRandomIntegerArray(INTS, WIDTH);
    int* arr2 = new int[INTS];
    
    std::copy(arr1, arr1 + INTS, arr2);
    
    auto begin = std::chrono::high_resolution_clock::now();
    std::sort(arr1, arr1 + INTS);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::cout << "C++ sort() in "
              << std::chrono
                    ::duration_cast<std
                    ::chrono::milliseconds>(end - begin).count()
              << " milliseconds."
              << std::endl;
    
    begin= std::chrono::high_resolution_clock::now();
    tree_sort(arr2, arr2 + INTS);
    end = std::chrono::high_resolution_clock::now();
    
    std::cout << "Treesort in "
              << std::chrono
                    ::duration_cast<std
                    ::chrono::milliseconds>(end - begin).count()
              << " milliseconds."
              << std::endl;
    
    std::cout << "Equal: "
              << std::boolalpha
              << std::equal(arr1, arr1 + INTS, arr2)
              << std::endl;
}

static void profileOnVector()
{
    
}

int main(int argc, const char * argv[]) {
    profileOnIntegerArray();
    std::cout << "Done!" << std::endl;
    return 0;
}
