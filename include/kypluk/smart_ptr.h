#pragma once

#include "define.h"
#include <utility>

namespace kypluk {

template <class type>
class unique_ptr {
	public:
		using pointer_t = type*;
		using reference_t = type&;
        unique_ptr(const unique_ptr&) = delete;
        void operator = (const unique_ptr&) = delete;

		unique_ptr() = default;
        explicit unique_ptr(pointer_t ptr) {
			this->pointer = ptr;
		}

		unique_ptr(unique_ptr&& ptr) noexcept {
            this->move(ptr);
        }

        unique_ptr& operator = (unique_ptr&& ptr) noexcept {
            this->move(ptr);
        }

		~unique_ptr() noexcept {
			if (pointer) delete pointer;
		}
		
		pointer_t release() {
			pointer_t temp = pointer;
            pointer = nullptr;
			return temp;
		}

		void reset(pointer_t ptr = nullptr) {
			if (this->pointer) delete this->pointer;
			this->pointer = ptr;
		}
		
		void move(unique_ptr &other) noexcept {
            std::swap(this->pointer, other.pointer);
		}
		
		pointer_t get() {
			return pointer;
		}
		
		operator bool () noexcept {
			return pointer != nullptr;
		}
		
		reference_t operator * () noexcept {
			return *pointer;
		}
		
        pointer_t operator -> () noexcept {
			return get();
		}
		
        explicit operator pointer_t () {
			return pointer;
		}
    private:
        pointer_t pointer = nullptr;
};

template <class type>
class reference_counter {
    public:
		using pointer_t = type*;
		using reference_t = type&;
	private:
		struct Node {
			mutable size_t size = 0;
			pointer_t value = nullptr;

			Node() = default;
			Node(pointer_t ptr) : value(ptr), size(1) {}
		};
	public:
		reference_counter(type *value = nullptr) {
			node = new Node(value);
		}
		
		reference_counter(const reference_counter & other) {
			node = other.node;
			other.node->size++;
		}
		
		~reference_counter() {
			node->size--;
			if (node->size == 0) {
				if (node->value) delete node->value;
				delete node;
			}
		}
		 
		void reset(pointer_t ptr = nullptr) {
			if (node->size == 1) {
				if (node->value) delete node->value;
				node->value = ptr;
			} else {
				node->size--;
				node = new Node(ptr);
			}
		}
		
		bool unique() {
			return node->value >= 1;
		}
		
		void swap(reference_counter &other) {
			swap(this->node, other->node);
		}
		
		pointer_t get() {
			return node->value;
		}
		
		reference_counter& operator = (const reference_counter &other) {
			node->size--;
			if (node->size == 0) {
				if (node->value) delete node->value;
				delete node;
			}
			this->node = other.node;
			this->node->size++;
			return *this;
		}
		
		reference_counter& operator = (pointer_t other) {
			this->reset(other);
			return *this;
		}
		
		operator bool () {
			return node->value != nullptr;
		}
		
		reference_t operator * () {
			return *get();
		}
		
		pointer_t operator -> () {
			return get();
		}

    private:
        Node * node;
};

}
