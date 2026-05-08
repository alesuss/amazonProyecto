#pragma once
#include <vector>
#include <functional>

using namespace std;


template <typename T>
class SelectionSort {
public:
    static void ordenar(vector<T>& A, function<bool(T, T)> comp) {
        int n = (int)A.size();
        for (int i = 0; i < n - 1; i++) {
            int k = i;
            for (int j = i + 1; j < n; j++) {
                if (comp(A[j], A[k])) k = j;
            }
            swap(A[i], A[k]);
        }
    }
};
