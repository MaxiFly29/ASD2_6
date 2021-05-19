#include <iostream>
#include <fstream>
#include <random>
#include "Profiler.h"
#include "AVLT.h"
#include "RBT.h"
#include "Array.h"
#include "TreeSet.h"
#include "DoublyLinkedList.h"

using namespace std;

void printResults(const unordered_map<string, vector<size_t>>& results, ostream&& out) {
    out << "\t";
    for (
        string ind : {
            "1e3", "2e3", "3e3", "4e3", "5e3", "6e3", "7e3", "8e3", "9e3",
            "1e4", "2e4", "3e4", "4e4", "5e4", "6e4", "7e4", "8e4", "9e4",
            "1e5", "2e5", "3e5", "4e5", "5e5", "6e5", "7e5", "8e5", "9e5",
            "1e6", "2e6", "3e6", "4e6", "5e6", "6e6", "7e6", "8e6", "9e6",
            "1e7"
        }
    )
        out << ind << "\t";
    out << "\n";
    for (const auto& p : results) {
        out << p.first << ":\t";
        size_t ind = 0;

        for (auto val : p.second) {
            out << val / 1e3 << "ms\t";
        }
        out << "\n";
    }
}

int main()
{
    //ofstream fout("AVLT.txt");
    printResults(
        Profiler<AVLT<int>>()
            .setInsert(&AVLT<int>::insert)
            .setErase(&AVLT<int>::erase)
            .setFind(&AVLT<int>::contains)
            .setSort(&AVLT<int>::getSorted)
            .run(),
        ofstream("AVLT.txt")
    );
    
    printResults(
        Profiler<RBT<int>>()
            .setInsert(&RBT<int>::insert)
            .setErase(&RBT<int>::erase)
            .setFind(&RBT<int>::contains)
            .setSort(&RBT<int>::getSorted)
            .run(),
        ofstream("RBT.txt")
    );
    printResults(
        Profiler<Tree<int>>()
        .setInsert(&Tree<int>::insert)
        .setErase(&Tree<int>::erase)
        .setFind(&Tree<int>::contains)
        .setSort(&Tree<int>::getSorted)
        .run(),
        ofstream("TreeSet.txt")
    );
    printResults(
        Profiler<DoublyLinkedList<int>>()
            .setInsert(&DoublyLinkedList<int>::push_back)
            .setErase(&DoublyLinkedList<int>::eraseByValue)
            .setFind(&DoublyLinkedList<int>::getIndexOf)
            .setSort(&DoublyLinkedList<int>::getSorted)
            .run(),
        ofstream("LinkedList.txt")
    );
    printResults(
        Profiler<Array<int>>()
        .setInsert(&Array<int>::push_back)
        .setErase(&Array<int>::eraseByValue)
        .setFind(&Array<int>::getIndexOf)
        .setSort(&Array<int>::getSorted)
        .run(),
        ofstream("Array.txt")
    );
    return 0;
}