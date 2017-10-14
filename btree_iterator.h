#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include <utility>
#include <cassert>

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
		using node = typename btree<T>::node;

		node *cur_;
		size_t index_;

		btree_iterator(node *cur, size_t index) : cur_{cur}, index_{index} { }
		btree_iterator(std::pair<node*, size_t> pair) : btree_iterator{pair.first, pair.second} { }

	public:
		typedef std::ptrdiff_t difference_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef RetVal value_type;
		typedef RetVal* pointer;
		typedef RetVal& reference;

		template <typename U, typename RetVal2>
		friend class btree_iterator;
		friend class btree<T>;

		operator btree_iterator<T, typename std::add_const<RetVal>::type>() const {
			return {cur_, index_};
		}

		reference operator*() const { return cur_->values_.at(index_); }
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

		btree_iterator& operator++(){
			const auto &ptr = cur_->children_.at(index_ + 1);
			if(ptr){
				for(cur_ = ptr.get(); *cur_->children_.begin(); cur_ = cur_->children_.begin()->get());
				index_ = 0;
			}
			else{
				++index_;
				while(index_ == cur_->values_.size() && cur_->parent_){
					index_ = cur_->index_;
					cur_ = cur_->parent_;
				}
			}
			return *this;
		}

		btree_iterator operator++(int a){
			auto copy = *this;
			operator++();
			return copy;
		}

		btree_iterator& operator--(){
			const auto &ptr = cur_->children_.at(index_);
			if(ptr){
				for(cur_ = ptr.get(); *cur_->children_.rbegin(); cur_ = cur_->children_.rbegin()->get());
				index_ = cur_->values_.size() - 1;
			}
			else{
				while(index_ == 0){
					index_ = cur_->index_;
					cur_ = cur_->parent_;
				}
				index_--;
			}
			return *this;
		}

		btree_iterator operator--(int a){
			auto copy = *this;
			operator--();
			return copy;
		}
};

#endif
