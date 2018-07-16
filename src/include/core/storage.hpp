#pragma once

#include <config.hpp>
#include <core/address.hpp>
#include <entity/record.hpp>

namespace storage {

    using namespace std;
    using namespace address;
    using namespace record;
    using namespace config;

    template <class T, class Cmp = less<T>>
    class BTree {
    private:

        struct Eq {
            Cmp cmp;

            bool operator()(const T& l, const T& r) {
                return !(comp(l, r) || comp(r, l));
            }
        };


        struct Node;
            
        /**
         * Address type is just like a pointer type, 
         * but it also stores an address to disk
         */
        struct Page {
            vector<T>             page;
            vector<Address<Node>> childs;

            struct SearchResult {
                bool   found;
                size_t index;
            };

            Page(size_t page_size): page(), childs() {
                page.reserve(page_size + 1);
                childs.reserve(page_size + 2);
                childs.emplace_back();
            }

            SearchResult search(const T& data) {
                static Eq  eq;
                static Cmp cmp;

                size_t i = 0UL;
                for (; i < page.size(); ++i) {
                    if (eq(page[i], data))
                        return SearchResult { true, i };

                    if (!cmp(page[i], data))
                        return SearchResult { true, i };
                }

                return SearchResult { false, i };
            }

            template <class U>
            void shift(vector<U>& vec, size_t start) {
                vec.emplace_back();
                for (size_t i = vec.size() - 1; i > start; --i)
                    vec[i] = vec[i - 1];
            }
        };

        struct Node {
            Page page;

            Node(size_t page_size): page(page_size) {}

            void insert(const T& data, size_t page_size) {
                if (page.page.size() < page_size) {
                    size_t index = page.search(data);
                    page.shift(page.page, index);
                    page.shift(page.childs, index);
                    page.page[index] = data;

                } else {
                }
            }

            void remove(const T& data, BTree* btree) {
            
            }
        };

        Cmp cmp;
        Eq  eq;

        Address<Node> root;
        size_t page_size;
        string nodes_file;
        
    public:

        BTree(string nodes_file, size_t page_size): 
            cmp(),
            eq(),
            root(), 
            page_size(page_size),
            nodes_file(nodes_file) {}

        void insert(const T& data) {
            auto rootptr = root.read(nodes_file);
            if (!rootptr) {
                root.set_memory_address(new Node);
                rootptr = root.get_memory_address();
                rootptr->insert(data, page_size);
            } else {
                            
            }
        }

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
