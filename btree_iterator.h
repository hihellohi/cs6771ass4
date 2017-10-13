#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>

/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)

template<typename T> class btree;

template<typename T, typename RetVal>
class btree_iterator {
	private:
		using Node = typename btree<T>::Node;

		Node *cur_;
		size_t index_;

		btree_iterator() : cur_{nullptr} { }
		btree_iterator(Node *cur, size_t index) : cur_{cur}, index_{index} { }
		btree_iterator(std::pair<Node*, size_t> pair) : btree_iterator{pair.first, pair.second} { }

	public:
		typedef std::ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef RetVal value_type;
		typedef RetVal* pointer;
		typedef RetVal& reference;

		template <typename U, typename RetVal2>
		friend class btree_iterator;
		friend class btree<T>;

		reference operator*() const { return cur_.values[index_]; }
		pointer operator->() const { return &(operator*()); }

		template <typename U>
		bool operator==(const btree_iterator<T, U> &other) const {
			if(!cur_ && !other.cur_) return true;
			return (other.cur_ == cur_) && (other.index_ == index_);
		}

		template <typename U>
		bool operator!=(const btree_iterator<T, U> &other) const {
			return !(*this == other);
		}
};

#endif
