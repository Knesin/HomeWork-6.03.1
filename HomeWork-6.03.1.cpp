#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <future>

void print_vec(const std::vector<int>& vec) { //печать вектора
    for (const auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void swap(int& a, int& b) {//перестановка элементов
    int tmp = a;
    a = b;
    b = tmp;
}

int min(std::vector<int>::iterator start, std::vector<int>::iterator end) { //поиск минимума
    int result = 0;
    int index = 0;
    while (start + index != end) {
        if (start[index] < start[result]) {
            result = index;
        };
        index++;
    }
    return result;
}

void sort(std::vector<int>& v) {//сортировка выбором
    int m = 0;
    for (int i = 0; i < v.size() - 1; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));// без задержки async работает до 100`000 медленее после одинаково
        m = min((v.begin() + i), v.end());
        if (m != 0) {
            swap(v[i], v[m + i]);
        }
    }
}

void min_async(std::vector<int>::iterator start, std::vector<int>::iterator end, std::promise<int> ff) {//поиск минимума для async сортировки
    int result = 0;
    int index = 0;
    while (start + index != end) {
        if (start[index] < start[result]) {
            result = index;
        };
        index++;   
    }
    ff.set_value(result);
}

void sort_async(std::vector<int>& v) {//сортировка выбором с async
    //std::future<int> ft_pr;
    int m = 0;
    for (int i = 0; i < v.size() - 1; i++) {
        std::promise<int> promi;
        std::future<int> ft_pr = promi.get_future();
        auto ft = std::async(std::launch::async ,min_async, (v.begin() + i), v.end(), std::move(promi));
        //ft_pr = std::async(min, (v.begin() + i), v.end());
        std::this_thread::sleep_for(std::chrono::milliseconds(40));// без задержки async работает до 100`000 медленее после одинаково

        m = ft_pr.get();
        if (m != 0) {
            swap(v[i], v[m + i]);
        }
    }
}


int main() {
    const int vector_length = 100;// Длина сортируемого вектора
    std::vector<int> vec1(vector_length);
    std::vector<int> vec2(vector_length);
    for (auto& i : vec1) {
        i = std::rand() % vector_length;  
    }
    vec2 = vec1;
    //print_vec(vec1);

    auto time_start = std::chrono::high_resolution_clock::now();
    sort(vec1);
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> t = (time_end - time_start);
    std::cout << "Time no async: " << t.count() << std::endl;

    time_start = std::chrono::high_resolution_clock::now();
    sort_async(vec2);
    time_end = std::chrono::high_resolution_clock::now();
    t = (time_end - time_start);
    std::cout << "Time async: " << t.count() << std::endl;

    //print_vec(vec1);
    //print_vec(vec2);
}