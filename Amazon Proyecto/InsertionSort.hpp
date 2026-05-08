#pragma once
#include <vector>
#include <functional>

using namespace std;


template <typename T>
class InsertionSort {
public:
    static void ordenar(vector<T>& A, function<bool(T, T)> comp) {
        int n = (int)A.size();
        for (int i = 1; i < n; i++) {
            T aux = A[i];
            int k = i - 1;
            while (k >= 0 && comp(aux, A[k])) {
                A[k + 1] = A[k];
                k--;
            }
            A[k + 1] = aux;
        }
    }
};
