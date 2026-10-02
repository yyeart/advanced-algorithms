// Ввод: n, затем n пар: key value. value — слово без пробелов.
// key из [-1000000000, 1000000000], n <= 1000000.
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Record {
    int key;
    string value;
};

// Прибавляем одну константу ко всем ключам: порядок не меняется.
long long sortKey(const Record& item) {
    return item.key + 1000000000LL;
}

void radixSort(vector<Record>& a) {
    int n = int(a.size());
    long long maximum = 0;
    for (int i = 0; i < n; ++i) {
        if (sortKey(a[i]) > maximum) maximum = sortKey(a[i]);
    }
    vector<Record> b(n);
    for (long long exp = 1; exp <= maximum; exp *= 10) {
        vector<int> count(10, 0);
        for (int i = 0; i < n; ++i) {
            int digit = int((sortKey(a[i]) / exp) % 10);
            ++count[digit];
        }
        for (int digit = 1; digit < 10; ++digit) count[digit] += count[digit - 1];
        for (int i = n - 1; i >= 0; --i) {
            int digit = int((sortKey(a[i]) / exp) % 10);
            --count[digit];
            // Ключ и значение перемещаются вместе. digit вычислен до move.
            b[count[digit]] = std::move(a[i]);
        }
        a.swap(b);
    }
}

int main() {
    int n;
    if (!(cin >> n) || n < 0 || n > 1000000) return 1;
    vector<Record> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> a[i].key >> a[i].value) || a[i].key < -1000000000 || a[i].key > 1000000000) return 1;
    }
    radixSort(a);
    for (int i = 0; i < n; ++i) cout << a[i].key << ' ' << a[i].value << '\n';
}
