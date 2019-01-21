#ifndef _kyplukAlgorithm_H_
#define _kyplukAlgorithm_H_

#include <kyplukDefine.h>

template <class type>
void swap (type& raz, type& dva) {
	type temp = raz;
	raz = dva;
	dva = temp;
}

template <class type>
int compare (const type& raz, const type& dva)
{
	if (raz > dva)
		return 1;
	elif (raz < dva)
		return -1;
	else
		return 0;
}

template <class Iterator, class UnaryFunction>
void for_each(Iterator begin, Iterator end, UnaryFunction func) {
    for (auto it = begin; it != end; ++it) {
	func(*it);
    }
}

template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator begin, InputIterator end, OutputIterator c_begin) {
    while (begin != end) {
	*c_begin++ = *begin++;
    }
    return c_begin;
}

#endif
