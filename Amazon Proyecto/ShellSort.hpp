#pragma once
#include <vector>
#include <functional>

using namespace std;


template <typename T>
class ShellSort {
public:
    static void ordenar(vector<T>& A, function<bool(T, T)> comp) {
        int n = (int)A.size();
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                T temp = A[i];
                int j = i;
                while (j >= gap && comp(temp, A[j - gap])) {
                    A[j] = A[j - gap];
                    j -= gap;
                }
                A[j] = temp;
            }
        }
    }
};
