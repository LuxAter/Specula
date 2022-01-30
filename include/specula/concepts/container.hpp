#ifndef SPECULA_CONCEPTS_CONTAINER_HPP
#define SPECULA_CONCEPTS_CONTAINER_HPP

#include <concepts>
#include <type_traits>
#include <utility>

namespace specula {
template <typename C>
concept container = requires(C& cont, C const& const_cont) {
  typename C::value_type;
  typename C::reference;
  typename C::const_reference;

  requires std::same_as<typename C::reference, typename C::value_type&>;
  requires std::same_as<typename C::const_reference,
                        typename C::value_type const&>;

  typename C::iterator;
  typename C::const_iterator;
  requires std::forward_iterator<typename C::iterator>;
  requires std::forward_iterator<typename C::const_iterator>;
  requires std::convertible_to<typename C::iterator,
                               typename C::const_iterator>;
  requires std::same_as < std::iter_value_t<typename C::iterator>,
  typename C::value_type > ;
  requires std::same_as < std::iter_value_t<typename C::const_iterator>,
  typename C::value_type > ;
  requires std::same_as < std::iter_reference_t<typename C::iterator>,
  typename C::reference > or
      std::same_as<std::iter_reference_t<typename C::iterator>,
                   typename C::const_reference>;
  requires std::same_as < std::iter_reference_t<typename C::const_iterator>,
  typename C::const_reference > ;

  typename C::difference_type;
  typename C::size_type;
  requires std::signed_integral<typename C::difference_type>;
  requires std::unsigned_integral<typename C::size_type>;
  requires std::in_range<typename C::size_type>(
      std::numeric_limits<typename C::difference_type>::max());
  requires std::same_as<
      typename C::difference_type,
      typename std::iterator_traits<typename C::iterator>::difference_type>;
  requires std::same_as<typename C::difference_type,
                        typename std::iterator_traits<
                            typename C::const_iterator>::difference_type>;

  requires not std::equality_comparable<typename C::value_type> or
      std::equality_comparable<C>;

  requires not std::movable<typename C::value_type> or std::movable<C>;
  requires not std::copyable<typename C::value_type> or std::copyable<C>;
  requires not std::semiregular<typename C::value_type> or std::semiregular<C>;
  requires not std::regular<typename C::value_type> or std::regular<C>;

  { cont.begin() } -> std::same_as<typename C::iterator>;
  { cont.end() } -> std::same_as<typename C::iterator>;
  { const_cont.begin() } -> std::same_as<typename C::const_iterator>;
  { const_cont.end() } -> std::same_as<typename C::const_iterator>;
  { cont.cbegin() } -> std::same_as<typename C::const_iterator>;
  { cont.cend() } -> std::same_as<typename C::const_iterator>;

  { const_cont.max_size() } -> std::same_as<typename C::size_type>;
  { const_cont.empty() } -> std::convertible_to<bool>;
};
}  // namespace specula

#endif  // SPECULA_CONCPET_CONTAINER_HPP