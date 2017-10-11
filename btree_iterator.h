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
		btree_iterator(Node *cur, int index) : cur_{cur}, index_{index} { }

		Retval& operator*() {
			return cur_.values[index_];
		}
	private:
		int index_;
		Node *cur_;
		bool valid(){
			return (unsigned)index < cur_->values.size();
		}
}

#endif
