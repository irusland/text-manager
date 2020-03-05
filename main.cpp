#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

using namespace std;


class Pair {
public:
    Pair(const char *key, int count) : key(key), count(count) {}
    void Increase() {
        this->count += 1;
    }
    const char *key;
    mutable int count;
};

struct Comparer {
    typedef std::true_type is_transparent;
    bool operator()(const Pair& lhs, const char *rhs) const {
        return strcmp(lhs.key, rhs) < 0;
    }
    bool operator()(const char *lhs, const Pair& rhs) const {
        return strcmp(lhs, rhs.key) < 0;
    }
    bool operator()(const Pair& lhs, const Pair& rhs) const {
        return strcmp(lhs.key, rhs.key) < 0;
    }
};

int main() {
    ifstream in("/Users/irusland/Desktop/UrFU/C++/task1/test.txt");

    if (!in) {
        cout << "Cannot open input file" << std::endl;
        return 1;
    }

    in.seekg (0, ios::end);
    const int size = in.tellg();

    in.seekg (0, ios::beg);
    cout << "File size " << size << "B" << endl;

    char *Text = new char[size + 1];
    in.read(Text, size);

    if (!in) {
        std::cout << "error: only " << in.gcount() << " could be read" << endl;
        return 1;
    }
    in.close();

    Text[size] = '\0';

    set<Pair, Comparer> s;

    auto start = chrono::steady_clock::now();

//    todo tokenizing the book here
    size_t wordlen = 0;                  // initial room for string
    char x;
    char *word = (char*)malloc(wordlen);
    for (int i = 0; i <= size; ++i) {
        x = Text[i];
        cout << x << endl;
        if (x == ' ' | x == '\0') {
            auto it = s.find(word);
            if (it == s.end()) {
                cout << "insert: '" << word << "' count = " << 1 << endl;
                s.insert(Pair(word, 1));
            } else {
                it->count++;
                cout << "update: '" << word << "' count = " << it->count << endl;
            }
            wordlen = 0;
            word = (char*)malloc(wordlen);
        }
        else {
            wordlen++;
            word = (char*)realloc(word, wordlen);
            word[wordlen - 1] = x;
        }
    }

    auto end = chrono::steady_clock::now();
    cout << "Elapsed "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}


