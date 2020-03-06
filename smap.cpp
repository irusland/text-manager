//
// Created by Ruslan Sirazhetdinov on 07.03.2020.
//

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <chrono>
#include <string>

using namespace std;

struct CountComparator {
    typedef std::true_type is_transparent;
    bool operator()(const pair<string, int>& lhs, const int rhs) const {
        return lhs.second < rhs;
    }
    bool operator()(const int lhs, const pair<string, int>& rhs) const {
        return lhs < rhs.second;
    }
    bool operator()(const pair<string, int>& lhs, const pair<string, int>& rhs) const {
        return lhs.second < rhs.second;
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

    map<string, int> word_counter;
    multiset<pair<string, int>, CountComparator> count_sorted; // sorting by count

    auto start = chrono::steady_clock::now();

    size_t wordlen = 0;
    string word;
    for (int i = 0; i <= size; ++i) {
        if (!isalpha(Text[i])) {
            if (wordlen == 0) {
                continue;
            }
            auto it = word_counter.find(word);
            if (it == word_counter.end()) {
                word_counter[word] = 1;
            } else {
                it->second++;
            }
            wordlen = 0;
            word = "";
        } else {
            wordlen++;
            word += (char)tolower((int)Text[i]);
        }
    }

    for(auto ab : word_counter) {
        count_sorted.insert(ab);
    }

    auto end = chrono::steady_clock::now();

    for (auto element : count_sorted)
        std::cout << element.first << " - " << element.second << std::endl;
    cout << "File size " << size << " B" << endl;
    cout << "Elapsed " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}


