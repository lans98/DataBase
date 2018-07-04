#ifndef SCPPDB_STORAGE_HPP
#define SCPPDB_STORAGE_HPP

#include <record.hpp>

namespace storage {

    using namespace std;
    using namespace record;

    class BTree {
    private:

    public:

        // TODO
        class Iterator {
        private:

        public:
            Record /*& (reference)*/ operator*() { return Record(); }
            Record /*& (reference)*/ operator->() { return Record(); }
           
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
        Iterator search(Record) { return Iterator(); }
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

        BTree::Iterator begin();
        BTree::Iterator end();
        BTree::Iterator last();
        BTree::Iterator search();
    };
}

#endif
