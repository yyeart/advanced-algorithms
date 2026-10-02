// Ввод: n, затем n чисел double из [0, 1). n <= 1000000.
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<double>& a) {
    for (int i = 1; i < int(a.size()); ++i) {
        double x = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = x;
    }
}

void bucketSort(vector<double>& a) {
    int n = int(a.size());
    if (n == 0) return;
    vector<vector<double>> buckets(n);
    for (int i = 0; i < n; ++i) {
        int index = int(a[i] * n);
        if (index >= n) index = n - 1; // Защита от округления double.
        buckets[index].push_back(a[i]);
    }
    int pos = 0;
    for (int index = 0; index < n; ++index) {
        insertionSort(buckets[index]);
        for (int j = 0; j < int(buckets[index].size()); ++j) {
            a[pos] = buckets[index][j];
            ++pos;
        }
    }
}

int main() {
    int n;
    if (!(cin >> n) || n < 0 || n > 1000000) return 1;
    vector<double> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> a[i]) || !(a[i] >= 0 && a[i] < 1)) return 1;
    }
    bucketSort(a);
    cout << setprecision(17);
    for (int i = 0; i < n; ++i) cout << a[i] << ' ';
    cout << '\n';
}
