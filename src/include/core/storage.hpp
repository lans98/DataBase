#pragma once

#include <entity/record.hpp>

namespace storage {

    using namespace std;
    using namespace record;

    template <class T>
    class BTree {
    private:

    public:

        class Iterator {
        private:

        public:
            T& operator*() { /*return Record();*/ }
            T& operator->() { /*return Record();*/ }
           
            bool operator==(const Iterator& rhs) { return false; }
            bool operator!=(const Iterator& rhs) { return false; }
            
            Iterator operator++() { return *this; }
            Iterator operator++(int) { return *this; }
        };

        // TODO
        Iterator begin() {  return Iterator(); }

        // TODO
        Iterator end() { return Iterator(); }

        // TODO
        Iterator last() { return Iterator(); }

        // TODO 
        Iterator search(T data) { return Iterator(); }
    };

    // abstraction for manipulate rows in disk
    class RecordStorage {
    private:
        // B+ object (impl is in btree.hpp)
        // BTree btree;
        
    public:
        // TODO
        bool insert(Record) {
            return false;
        }

        // TODO
        bool remove(Record) {
            return false;
        }

        // TODO
        bool is_empty() {
            return true;
        }

        // TODO
        bool flush() {
            return true;
        }

        BTree<Record>::Iterator begin();
        BTree<Record>::Iterator last();
        BTree<Record>::Iterator end();
        BTree<Record>::Iterator search();
    };
}
