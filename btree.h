/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node 
 * that stores m client elements partition the tree 
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

template <typename T> 
class btree {
	private:
		// The details of your implementation go here
		struct Node {
			Node(int size) : values(size), children(size + 1) {}

			std::vector<T> values;
			std::vector<unique_ptr<Node>> children;
			Node *parent;
		};

		size_t maxNodeElems_;
		unique_ptr<Node> head_;

		template<typename it>
		it lower_bound(Node *cur, const T& elem) const {
			const auto lower = std::lower_bound(cur->values.begin(), cur->values.end(), elem);
			int index = lower - cur->values.begin();
			if(*lower == elem || cur.values[index] == nullptr) {
				return (cur, index);
			}
			return lower_bound<it>(cur.values[index].get(), elem);
		}

		iterator make_node(unique_ptr<T> &ptr, const T& elem) {
			ptr.reset(new Node(maxNodeElems_));
			ptr->values.push_back(elem);

			ptr->children.push_back(nullptr);
			ptr->children.push_back(nullptr);
			return (head_.get(), 0);
		}

	public:
		/** Hmm, need some iterator typedefs here... friends? **/
		using iterator = btree_iterator<T, T>;
		using const_iterator = btree_iterator<T, std::add_const<T>::type>;

		friend iterator;
		friend const_iterator;

		/**
		 * Constructs an empty btree.  Note that
		 * the elements stored in your btree must
		 * have a well-defined copy constructor and destructor.
		 * The elements must also know how to order themselves
		 * relative to each other by implementing operator<
		 * and operator==. (These are already implemented on
		 * behalf of all built-ins: ints, doubles, strings, etc.)
		 * 
		 * @param maxNodeElems the maximum number of elements
		 *        that can be stored in each B-Tree node
		 */
		btree(size_t maxNodeElems = 40) : maxNodeElems_{maxNodeElems}, head_{nullptr} { }

		/**
		 * The copy constructor and  assignment operator.
		 * They allow us to pass around B-Trees by value.
		 * Although these operations are likely to be expensive
		 * they make for an interesting programming exercise.
		 * Implement these operations using value semantics and 
		 * make sure they do not leak memory.
		 */

		/** 
		 * Copy constructor
		 * Creates a new B-Tree as a copy of original.
		 *
		 * @param original a const lvalue reference to a B-Tree object
		 */
		btree(const btree<T>& original);

		/** 
		 * Move constructor
		 * Creates a new B-Tree by "stealing" from original.
		 *
		 * @param original an rvalue reference to a B-Tree object
		 */
		btree(btree<T>&& original) = default;


		/** 
		 * Copy assignment
		 * Replaces the contents of this object with a copy of rhs.
		 *
		 * @param rhs a const lvalue reference to a B-Tree object
		 */
		btree<T>& operator=(const btree<T>& rhs);

		/** 
		 * Move assignment
		 * Replaces the contents of this object with the "stolen"
		 * contents of original.
		 *
		 * @param rhs a const reference to a B-Tree object
		 */
		btree<T>& operator=(btree<T>&& rhs) = default;

		/**
		 * Puts a breadth-first traversal of the B-Tree onto the output
		 * stream os. Elements must, in turn, support the output operator.
		 * Elements are separated by space. Should not output any newlines.
		 *
		 * @param os a reference to a C++ output stream
		 * @param tree a const reference to a B-Tree object
		 * @return a reference to os
		 */
		friend std::ostream& operator<< <T> (std::ostream& os, const btree<T>& tree);

		/**
		 * The following can go here
		 * -- begin() 
		 * -- end() 
		 * -- rbegin() 
		 * -- rend() 
		 * -- cbegin() 
		 * -- cend() 
		 * -- crbegin() 
		 * -- crend() 
		 */
		iterator end() {
			return ();
		}

		const_iterator end() const {
			return ();
		}

		/**
		 * Returns an iterator to the matching element, or whatever 
		 * the non-const end() returns if the element could 
		 * not be found.  
		 *
		 * @param elem the client element we are trying to match.  The elem,
		 *        if an instance of a true class, relies on the operator< and
		 *        and operator== methods to compare elem to elements already 
		 *        in the btree.  You must ensure that your class implements
		 *        these things, else code making use of btree<T>::find will
		 *        not compile.
		 * @return an iterator to the matching element, or whatever the
		 *         non-const end() returns if no such match was ever found.
		 */
		iterator find(const T& elem) {
			if(head_ == nullptr){
				return end();
			}
			auto lower = lower_bound<iterator>(head_.get(), elem);
			return lower.valid() && *lower == elem ? lower : end();
		}

		/**
		 * Identical in functionality to the non-const version of find, 
		 * save the fact that what's pointed to by the returned iterator
		 * is deemed as const and immutable.
		 *
		 * @param elem the client element we are trying to match.
		 * @return an iterator to the matching element, or whatever the
		 *         const end() returns if no such match was ever found.
		 */
		const_iterator find(const T& elem) const {
			if(head_ == nullptr){
				return cend();
			}
			auto lower = lower_bound<const_iterator>(head_.get(), elem);
			return lower.valid() && *lower == elem ? lower : cend();
		}

		/**
		 * Operation which inserts the specified element
		 * into the btree if a matching element isn't already
		 * present.  In the event where the element truly needs
		 * to be inserted, the size of the btree is effectively
		 * increases by one, and the pair that gets returned contains
		 * an iterator to the inserted element and true in its first and
		 * second fields.  
		 *
		 * If a matching element already exists in the btree, nothing
		 * is added at all, and the size of the btree stays the same.  The 
		 * returned pair still returns an iterator to the matching element, but
		 * the second field of the returned pair will store false.  This
		 * second value can be checked to after an insertion to decide whether
		 * or not the btree got bigger.
		 *
		 * The insert method makes use of T's copy constructor,
		 * and if these things aren't available, 
		 * then the call to btree<T>::insert will not compile.  The implementation
		 * also makes use of the class's operator== and operator< as well.
		 *
		 * @param elem the element to be inserted.
		 * @return a pair whose first field is an iterator positioned at
		 *         the matching element in the btree, and whose second field 
		 *         stores true if and only if the element needed to be added 
		 *         because no matching element was there prior to the insert call.
		 */
		std::pair<iterator, bool> insert(const T& elem) {
			if(head_ == nullptr){
				return make_pair(make_node(head_, elem), true);
			}

			auto lower = lower_bound<iterator>(head_.get(), elem);
			if(lower.valid() && *lower == elem){
				return make_pair(lower, false);
			}
			if(lower.cur_->values.size() < maxNodeElems_){
				lower.cur_->values.push_back(elem);
			}
			return make_pair(make_node(lower.cur_->children[lower.index], elem), true);
		}
};

#endif
