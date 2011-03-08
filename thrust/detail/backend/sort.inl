/*
 *  Copyright 2008-2011 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


/*! \file sort.inl
 *  \brief Inline file for sort.h
 */

#include <thrust/copy.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/iterator/detail/minimum_space.h>

#include <thrust/detail/trivial_sequence.h>

#include <thrust/detail/backend/dispatch/sort.h>
#include <thrust/detail/backend/generic/sort.h>

namespace thrust
{
namespace detail
{
namespace backend
{
namespace dispatch
{


template<typename RandomAccessIterator,
         typename StrictWeakOrdering,
         typename Backend>
  void sort(RandomAccessIterator first,
            RandomAccessIterator last,
            StrictWeakOrdering comp,
            Backend)
{
  thrust::detail::backend::generic::sort(first,last,comp);
} // end sort()


template<typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering,
         typename Backend>
  void sort_by_key(RandomAccessIterator1 keys_first,
                   RandomAccessIterator1 keys_last,
                   RandomAccessIterator2 values_first,
                   StrictWeakOrdering comp,
                   Backend)
{
  thrust::detail::backend::generic::sort_by_key(keys_first,keys_last,values_first,comp);
} // end sort_by_key()


} // end dispatch


template<typename RandomAccessIterator,
         typename StrictWeakOrdering>
  void sort(RandomAccessIterator first,
            RandomAccessIterator last,
            StrictWeakOrdering comp)
{
    thrust::detail::backend::dispatch::sort(first, last, comp,
        typename thrust::iterator_space<RandomAccessIterator>::type());
} // end sort()


template<typename RandomAccessIterator,
         typename StrictWeakOrdering>
  void stable_sort(RandomAccessIterator first,
                   RandomAccessIterator last,
                   StrictWeakOrdering comp)
{
    // ensure sequence has trivial iterators
    thrust::detail::trivial_sequence<RandomAccessIterator> keys(first, last);

    // dispatch on space
    thrust::detail::backend::dispatch::stable_sort(keys.begin(), keys.end(), comp,
            typename thrust::iterator_space<RandomAccessIterator>::type());

    // copy results back, if necessary
    if(!thrust::detail::is_trivial_iterator<RandomAccessIterator>::value)
        thrust::copy(keys.begin(), keys.end(), first);
}

template<typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
  void sort_by_key(RandomAccessIterator1 keys_first,
                   RandomAccessIterator1 keys_last,
                   RandomAccessIterator2 values_first,
                   StrictWeakOrdering comp)
{
    thrust::detail::backend::dispatch::sort_by_key(keys_first, keys_last, values_first, comp,
        typename thrust::detail::minimum_space<
          typename thrust::iterator_space<RandomAccessIterator1>::type,
          typename thrust::iterator_space<RandomAccessIterator1>::type,
          typename thrust::iterator_space<RandomAccessIterator2>::type
        >::type());
}

template<typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename StrictWeakOrdering>
  void stable_sort_by_key(RandomAccessIterator1 keys_first,
                          RandomAccessIterator1 keys_last,
                          RandomAccessIterator2 values_first,
                          StrictWeakOrdering comp)
{
    // ensure sequences have trivial iterators
    RandomAccessIterator2 values_last = values_first + (keys_last - keys_first);
    thrust::detail::trivial_sequence<RandomAccessIterator1> keys(keys_first, keys_last);
    thrust::detail::trivial_sequence<RandomAccessIterator2> values(values_first, values_last);

    // dispatch on space
    thrust::detail::backend::dispatch::stable_sort_by_key(keys.begin(), keys.end(), values.begin(), comp,
            typename thrust::iterator_space<RandomAccessIterator1>::type(),
            typename thrust::iterator_space<RandomAccessIterator2>::type());

    // copy results back, if necessary
    if(!thrust::detail::is_trivial_iterator<RandomAccessIterator1>::value)
        thrust::copy(keys.begin(), keys.end(), keys_first);
    if(!thrust::detail::is_trivial_iterator<RandomAccessIterator2>::value)
        thrust::copy(values.begin(), values.end(), values_first);
}

} // end namespace backend
} // end namespace detail
} // end namespace thrust

