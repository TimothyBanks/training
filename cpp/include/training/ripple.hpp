/*
2.  Consider *carefully* the following declaration, which
    is intended to allow the client to store a pair<T, U>, and
    look up the pair by either T or U.  We will start with a simple
    O(N) lookup:


    a. Finish the insert member function definition below the class.
    b. Add a definition for the size member function.
    c. Default construct a bi_map<std::string, int> in main()
    d. Print the size of the bi_map constructed in main()
    e. Insert an element to the bi_map constructed in main()


    Throughout this exercise, please feel free to add any API to the
    problem which aids you in completing the task.  However please
    work with the given data structure (the data members of bi_map).
*/

/*


3.  Add the ability to print the contents of the bi_map to a
    std::ostream. One possible signature for the stream function is
    this non-member function:


    // Print each pair in the bi_map to the ostream
    template <class T, class U>
    std::ostream&
    operator<<(std::ostream& os, bi_map<T, U> const& m);


    The format of the output is up to you.


    Change main() to print the bi_map to std::cout.
*/
/*


4.  Add the following class member functions to bi_map, and provide
    an implementation for each.


    const_iterator find(T const& t) const;
    const_iterator find(U const& u) const;


    Return an "end iterator" when the element does not exist

    Modify main() to exercise both of these functions and print the
    result.


    Please upload your code to the coderpad at least 24 hours prior
    to the next interview.


*/
/*


6.  Unlike std::map::find, our current implementation of bi_map::find
    executes in linear time. We’d like an implementation that runs in
    logarithmic time.


    To achieve this performance goal please modify the bodies of the
    existing insert and find functions, using the newly added members
    from step 5.


    Only one copy of T and U should be stored in the bi_map.


    Verify the output of the program produces correct results.
*/
/*


7.  Implement the following class member functions:


    // If the element exists, erase it, else do nothing
    void erase(T const& t);
    void erase(U const& u);


    Modify main to erase one of the inserted elements and then
    print the bi_map.


*/
/*


8.  Explain why bi_map’s destructor is correct or needs change.


*/
/*


9.  With the bi_map filled with a few items,

    a. Make a copy of the bi_map,
    b. Print out the original and the copy
    c. Erase an item from the copy.
    d. Print out the original and the copy


    e. Implement the following member function:[a]


       bi_map (bi_map const&); // copy constructor


*/
/*


11. Implement the move member functions and demonstrate that
    they are correct using some print statements:


    bi_map (bi_map&&);
    bi_map& operator= (bi_map&&);


*/

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

/* A bi-directional map.
   This functions similarly to std::map except that elements
   can be looked up by either T or U.

   T and U are never the same type.
   Can assume that T and U don't exist already in bi_map.
*/
template <class T, class U> class bi_map {
public:
  using pair = std::pair<T, U>;
  using iterator = typename std::list<pair>::iterator;
  using const_iterator = typename std::list<pair>::const_iterator;

  bi_map() = default;
  bi_map(const bi_map &bm);
  bi_map(bi_map &&bm) = default;

  bi_map &operator=(const bi_map &bm);
  bi_map &operator=(bi_map &&bm) = default;

  // returns the number of elements
  std::size_t size() const;

  void erase(T const &t);
  void erase(U const &u);

  // insert a copy of the given pair
  void insert(pair const &p);

  const_iterator begin() const;
  const_iterator end() const;

  const_iterator find(T const &t) const;
  const_iterator find(U const &u) const;

private:
  static_assert(!std::is_same<T, U>::value, "T and U can not be the same type");

  struct compare {
    template <class V> bool operator()(V const *x, V const *y) const {
      return *x < *y;
    }
  };

  std::map<T const *, iterator, compare> mapT_;
  std::map<U const *, iterator, compare> mapU_;
  std::list<pair> list_;
};

template <class T, class U>
std::ostream &operator<<(std::ostream &os, bi_map<T, U> const &m) {
  for (const auto &p : m) {
    os << p.first << ", " << p.second << std::endl;
  }
  return os;
}

template <class T, class U> bi_map<T, U>::bi_map(const bi_map &bm) {
  for (const auto &p : bm.list_) {
    insert(p);
  }
}

template <class T, class U>
bi_map<T, U> &bi_map<T, U>::operator=(const bi_map &bm) {
  if (&bm == this) {
    return *this;
  }

  list_.clear();
  mapT_.clear();
  mapU_.clear();

  for (const auto &p : bm.list_) {
    insert(p);
  }

  return *this;
}

template <class T, class U> void bi_map<T, U>::erase(T const &t) {
  auto it = mapT_.find(&t);
  if (it != std::end(mapT_)) {
    auto it2 = mapU_.find(&it->second->second);
    mapU_.erase(it2);
    list_.erase(it->second);
    mapT_.erase(it);
  }
}

template <class T, class U> void bi_map<T, U>::erase(U const &u) {
  auto it = mapU_.find(&u);
  if (it != std::end(mapU_)) {
    auto it2 = mapT_.find(&it->second->first);
    mapT_.erase(it2);
    list_.erase(it->second);
    mapU_.erase(it);
  }
}

template <class T, class U>
typename bi_map<T, U>::const_iterator bi_map<T, U>::find(T const &t) const {
  auto it = mapT_.find(&t);
  if (it == std::end(mapT_)) {
    return std::end(list_);
  }
  return it->second;
}

template <class T, class U>
typename bi_map<T, U>::const_iterator bi_map<T, U>::find(U const &u) const {
  auto it = mapU_.find(&u);
  if (it == std::end(mapU_)) {
    return std::end(list_);
  }
  return it->second;
}

template <class T, class U>
typename bi_map<T, U>::const_iterator bi_map<T, U>::begin() const {
  return std::begin(list_);
}

template <class T, class U>
typename bi_map<T, U>::const_iterator bi_map<T, U>::end() const {
  return std::end(list_);
}

// insert the pair
// if either key already exists, undefined behavior
template <class T, class U> void bi_map<T, U>::insert(pair const &p) {
  // Implement this function.
  // Assume that neither key is already present.
  list_.emplace_back(p);
  auto it = std::end(list_);
  --it;
  mapT_.emplace(&it->first, it);
  mapU_.emplace(&it->second, it);
}

// insert the pair
// if either key already exists, undefined behavior
template <class T, class U> std::size_t bi_map<T, U>::size() const {
  // Implement this function.
  // Assume that neither key is already present.
  return list_.size();
}

int main() {
  auto bm = bi_map<std::string, int>{};
  bm.insert({"hello", 1});
  bm.insert({"world", 2});
  bm.insert({"foo", 3});
  bm.insert({"bar", 4});
  bm.insert({"foobar", 5});
  std::cout << "Size: " << std::to_string(bm.size()) << std::endl;
  std::cout << bm << std::endl;

  // bm.erase("bar");
  auto it = bm.find("bar");
  if (it != std::end(bm)) {
    std::cout << it->first << ", " << it->second << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }

  // bm.erase(3);
  it = bm.find(3);
  if (it != std::end(bm)) {
    std::cout << it->first << ", " << it->second << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }

  auto bm2 = bi_map<std::string, int>{std::move(bm)};
  // auto bm2 = bi_map<std::string, int>{};
  // bm2 = std::move(bm);
  std::cout << "Original" << std::endl << bm << std::endl;
  std::cout << "Copy" << std::endl << bm2 << std::endl;
  bm2.erase("bar");
  std::cout << "Original" << std::endl << bm << std::endl;
  std::cout << "Copy" << std::endl << bm2 << std::endl;
}
