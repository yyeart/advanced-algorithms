// Ввод: n, затем n слов из строчных латинских букв, длины могут различаться.
// n и длина каждого слова <= 1000000. Пустая строка через cin >> не читается.
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

int digitAt(const string& s, int pos) {
    if (pos >= int(s.size())) return 0; // Конец строки меньше любой буквы.
    return s[pos] - 'a' + 1; // a -> 1, ..., z -> 26 (ASCII).
}

void radixSort(vector<string>& a) {
    int n = int(a.size());
    int length = 0;
    for (int i = 0; i < n; ++i) {
        if (int(a[i].size()) > length) length = int(a[i].size());
    }
    vector<string> b(n);
    // Индекс позиции от начала строки; проходы — справа налево.
    for (int pos = length - 1; pos >= 0; --pos) {
        vector<int> count(27, 0);
        for (int i = 0; i < n; ++i) ++count[digitAt(a[i], pos)];
        for (int digit = 1; digit < 27; ++digit) count[digit] += count[digit - 1];
        for (int i = n - 1; i >= 0; --i) {
            int digit = digitAt(a[i], pos);
            --count[digit];
            b[count[digit]] = std::move(a[i]);
        }
        a.swap(b);
    }
}

int main() {
    int n;
    if (!(cin >> n) || n < 0 || n > 1000000) return 1;
    vector<string> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> a[i]) || a[i].size() > 1000000) return 1;
        for (int j = 0; j < int(a[i].size()); ++j) {
            if (a[i][j] < 'a' || a[i][j] > 'z') return 1;
        }
    }
    radixSort(a);
    for (int i = 0; i < n; ++i) cout << a[i] << '\n';
}
