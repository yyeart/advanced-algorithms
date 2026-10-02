// Учебный ввод: n, затем n неотрицательных int. n <= 1000000.
#include <iostream>
#include <vector>
using namespace std;

void radixSort(vector<int>& a) {
    int n = int(a.size());
    int maximum = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i] > maximum) maximum = a[i];
    }
    vector<int> b(n);

    // exp = 1: единицы; 10: десятки; 100: сотни.
    // long long нужен, чтобы exp не переполнился на обычном 32-битном int.
    for (long long exp = 1; exp <= maximum; exp *= 10) {
        vector<int> count(10, 0);
        for (int i = 0; i < n; ++i) {
            int digit = int((a[i] / exp) % 10);
            ++count[digit];
        }
        for (int digit = 1; digit < 10; ++digit) {
            count[digit] += count[digit - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            int digit = int((a[i] / exp) % 10);
            --count[digit];
            b[count[digit]] = a[i]; // Переносим число целиком.
        }
        a.swap(b);
    }
}

int main() {
    int n;
    if (!(cin >> n) || n < 0 || n > 1000000) return 1;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> a[i]) || a[i] < 0) return 1;
    }
    radixSort(a);
    for (int i = 0; i < n; ++i) cout << a[i] << ' ';
    cout << '\n';
}
