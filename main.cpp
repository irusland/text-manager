#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

using namespace std;


class Pair {
public:
    Pair(const char *key, int count) : key(key), count(count) {}
    const char *key;
    mutable int count;
};

struct LexicalComparator {
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

struct CountComparator {
    typedef std::true_type is_transparent;
    bool operator()(const Pair& lhs, const int rhs) const {
        return lhs.count < rhs;
    }
    bool operator()(const int lhs, const Pair& rhs) const {
        return lhs < rhs.count;
    }
    bool operator()(const Pair& lhs, const Pair& rhs) const {
        return lhs.count < rhs.count;
    }
};

int main(int argc, char **argv) {
    ifstream in(argv[1]);
    if (!in) {
        cout << "Cannot open input file" << std::endl;
        return 1;
    }

    in.seekg (0, ios::end);
    const int size = in.tellg();

    in.seekg (0, ios::beg);

    char *Text = new char[size + 1];
    in.read(Text, size);

    if (!in) {
        std::cout << "error: only " << in.gcount() << " could be read" << endl;
        return 1;
    }
    in.close();

    Text[size] = '\0';

    set<Pair, LexicalComparator> word_counter; // word sorting
    multiset<Pair, CountComparator> count_sorted; // sorting by count

    auto start = chrono::steady_clock::now();

    size_t wordlen = 0;
    char *word = (char*)malloc(wordlen);
    for (int i = 0; i <= size; ++i) {
        if (!isalpha(Text[i])) {
            if (wordlen == 0) {
                continue;
            }
            auto it = word_counter.find(word);
            if (it == word_counter.end()) {
                word_counter.insert(Pair(word, 1));
            } else {
                it->count++;
            }
            wordlen = 0;
            word = (char*)malloc(wordlen);
        } else {
            wordlen++;
            word = (char*)realloc(word, wordlen);
            word[wordlen - 1] = (char)tolower((int)Text[i]);
        }
    }

    for(auto ab : word_counter) {
        count_sorted.insert(ab);
    }

    auto end = chrono::steady_clock::now();

    for (auto element : count_sorted)
        std::cout << element.key << " - " << element.count << std::endl;
    cout << "File size " << size << " B" << endl;
    cout << "Elapsed " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}


