// Учебный ввод: n K, затем n чисел из [0, K]. n и K <= 1000000.
#include <iostream>
#include <vector>
using namespace std;

void countingSort(vector<int>& a, int K) {
    int n = int(a.size());
    vector<int> count(K + 1, 0);
    vector<int> b(n);

    // 1. Частоты: count[x] = сколько раз встретилось x.
    for (int i = 0; i < n; ++i) {
        ++count[a[i]];
    }

    // 2. Префиксные суммы: count[x] = сколько элементов <= x.
    for (int x = 1; x <= K; ++x) {
        count[x] += count[x - 1];
    }

    // 3. Идём справа налево: равные элементы сохраняют порядок.
    for (int i = n - 1; i >= 0; --i) {
        int x = a[i];
        --count[x];
        b[count[x]] = a[i];
    }

    a.swap(b); // a теперь содержит результат.
}

int main() {
    int n, K;
    if (!(cin >> n >> K) || n < 0 || n > 1000000 || K < 0 || K > 1000000) return 1;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> a[i]) || a[i] < 0 || a[i] > K) return 1;
    }
    countingSort(a, K);
    for (int i = 0; i < n; ++i) cout << a[i] << ' ';
    cout << '\n';
}
