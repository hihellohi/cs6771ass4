#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>

/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)

template<typename T> btree;

template<typename T, typename RetVal>
class btree_iterator {
	public:
		friend btree<T>;

		btree_iterator() : cur_{nullptr} { }
		btree_iterator(Node *cur, size_t index) : cur_{cur}, index_{index} { }
		btree_iterator(std::pair<Node, size_t> pair) : btree_iterator{pair.first, pair.second} { }

		Retval& operator*() {
			return cur_.values[index_];
		}
	private:
		size_t index_;
		btree<T>::Node *cur_;
}

#endif
