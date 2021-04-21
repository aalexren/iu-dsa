/* -> Created by Artem Chernitsa B20-02 <- */

#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class Node {
public:
   T data;
   Node* next;
};

template <typename T>
class IList {
public:
   virtual void push(T item) = 0; // add a new element to the list
   virtual T get(size_t index) = 0; // return an element by the index
   virtual size_t size() = 0;
};

/*
 * Simple linked list.
 * This used to add elements, get elements and display whole list.
 * Time complexity of push is O(1), get is O(n), display is O(n).
 */
template <typename T>
class List : public IList<T> {
public:
   List() {
      _size = 0;
      head = NULL;
      tail = NULL;
   }

   void push(T item) override {
      auto* node = new Node<T>;
      node->data = item;
      node->next = NULL;

      if (head == NULL) {
         head = node;
         tail = node;
         node = NULL;
      }
      else {
         tail->next = node;
         tail = node;
      }

      _size++;
   }

   T get(size_t index) override {
      auto* iter = new Node<T>;
      iter = head;
      size_t i = 0;
      while (iter != NULL) {
         if (i == index) return iter->data;
         iter = iter->next;
      }

      return iter->data;
   }

   size_t size() override {
      return _size;
   }

   // special method following the task's format
   void display() {
      auto* iter = new Node<T>;
      iter = head;
      if (iter == NULL) {
         cout << endl;
         return;
      }
      while (iter->next != NULL) {
         cout << "$" << setprecision(2) << fixed << iter->data.first << " " << iter->data.second << ", ";
         iter = iter->next;
      }
      if (size() != 0)
         cout << "$" << setprecision(2) << fixed << iter->data.first << " " << iter->data.second << endl;
      else cout << endl;
   }

private:
   Node<T>* head;
   Node<T>* tail;
   size_t _size;
};

template <typename T>
class  SortedList {
public:
   virtual void add(T item) = 0; // add a new item to the list
   virtual T least() = 0; // return the least element
   virtual T greatest() = 0; // return the greatest element
   virtual T get(size_t i) = 0; // return i-th least element
   virtual size_t indexOf(T item) = 0; // return the index of an element (in a sorted element)
   virtual void remove(size_t i) = 0; // remove i-th least element from the list
   virtual List<T> searchRange(T from, T to) = 0; // find all items between from and to
   virtual size_t size() = 0; // return the size of the list
   virtual bool isEmpty() = 0; // return whether the list is empty
};

/*
 * Implementation of Sorted list ADT using arrays.
 * Elements sorted in ascending order. For the certain task first values are compared.
 */
template <typename T>
class ArraySortedList : public SortedList<T> {
public:
   ArraySortedList<T>() {
      _size = 0;
      _array = nullptr;
      _last = -1;
   }

   /* add element to list keeping order of elements.
    * method add the new element in average case for a linear time O(n) - cause we need to shift elements,
    * in case there is no place for the new element we should expand it:
    * that takes O(n) time; _index_to_insert() executes for a logarithmic time O(logn),
    * so worst case whole method gets also linear time.
    */
   void add(T item) override {
      if (isEmpty()) {
         _initialize(item);
         return;
      }

      if (_last + 1 == _size) {
         _expand(2);
      }

      size_t mid = _index_to_insert(item);
      if (mid <= _last) {
         size_t shift = _last - mid + 1; // count of elements have to shift
         for (size_t i = _last + 1; i > mid; --i) { // shift cycle starting from end of the array
            _array[i] = _array[i - 1];
         }
      }

      _last++;
      _array[mid] = item;
   }

   /*
    * Time complexity for worst case is O(1).
    */
   T least() override {
      return _array[0];
   }

   /*
    * Time complexity for worst case is O(1).
    */
   T greatest() override {
      return _array[_last];
   }

   /*
    * Time complexity for worst case is O(1).
    */
   T get(size_t i) override {
      // there can be exception out_of_range
      if (i >= _size) throw out_of_range("out of range");
      return _array[i];
   }

   /* This method works for a linear time in average O(n) */
   size_t indexOf(T item) override {
      for (size_t i = 0; i < size(); ++i) {
         if (_array[i].first < item.first < 0.003 && _array[i].second == item.second) {
            return i;
         }
      }
      return -1;
   }

   /* This method works for a linear time in average O(n) */
   void remove(size_t i) override {
      if (i <= _last) {
         for (size_t j = i; j < _last; ++j) {
            _array[j] = _array[j + 1];
         }
      }
      else {
         return;
      }
      _last--;
   }

   // special overriding for this task
   /* This method works for a linear time in average O(n):
    * since there is only one cycle from 0 to end of array,
    * we watch every element and check if it satisfies the condition,
    * so it takes O(1) in the best case, O(n) in the worst case,
    * push method takes constant time, so whole method takes O(n) in average case.
    */
   List<T> searchRange(double from, double to) {
      List<T> list;
      for (size_t i = 0; i < size(); ++i) {
         if (_array[i].first >= from && _array[i].first <= to) {
            list.push(_array[i]);
         }
      }

      return list;
   }

   // general implementation of searchRange()
   /* time complexity same as previous one */
   List<T> searchRange(T from, T to) override {
      List<T> list;
      for (size_t i = 0; i < size(); ++i) {
         if (_array[i] >= from && _array[i] <= to) {
            list.push(_array[i]);
         }
      }

      return list;
   }

   /*
    * Return number of element actually added to the list.
    * Time complexity is O(1).
    */
   size_t size() override {
      return _last + 1;
   }

   /*
    * Time complexity is O(1).
    */
   bool isEmpty() override {
      return _last == -1;
   }

private:
   size_t _size; // number of cells allocated in memory (not size like number of added elements)
   long long int _last; // index of the last element
   T* _array;

   // increase size of dynamic array factor times
   /* This method works for a linear time in average O(n),
    * since we need to copy all the elements to the new array.
    */
   void _expand(size_t factor) {
      T* _p_temp = new T[_size * factor];

      for (size_t i = 0; i < _size; ++i) {
         _p_temp[i] = _array[i];
      }

      _array = _p_temp;
      _size = _size * factor;
   }

   /*
    * This method works for a logarithmic time O(logn),
    * because it uses binary search.
    */
   size_t _index_to_insert(pair<double, string> item) {
      size_t l = 0, r = _last, mid = (l + r) / 2;

      while (true) {

         if (l == mid) {
            if (_array[mid].first >= item.first) {
               return mid; // first cell
            }
         }

         if (_array[mid].first < item.first) {
            l = mid + 1; // in upper half
            if (l > r) {
               return mid + 1;
            }
         }
         else if (l > r) {
            return mid; // last cell
         }
         else {
            r = mid - 1; // in lower half
         }

         mid = (l + r) / 2;
      }
   }


   /*
    * standard binary search
    * Time complexity is O(logn)
    */
   size_t _binary_search(T item) {
      size_t l = 0, r = _last, mid = (l + r) / 2;
      while (l <= r) {

         if (_array[mid] == item) {
            break;
         }
         if (_array[mid] < item) {
            l = mid + 1; // in upper half
         }
         else {
            r = mid - 1; // in lower half
         }

         mid = (l + r) / 2;
      }

      return mid;
   }

   /*
    * same binary search, overrided for the certain task
    * Time complexity is O(logn)
    */
   template <class K, class V>
   long long int _binary_search(pair<K, V> item) {
      size_t l = 0, r = _last, mid = (l + r) / 2;
      while (l <= r) {

         if (_array[mid].second == item.second) {
            return mid;
         }
         if (_array[mid].first - item.first < 10e-3) { // accuracy is 2 digits
            l = mid + 1; // in upper half
         }
         else {
            r = mid - 1; // in lower half
         }

         mid = (l + r) / 2;
      }

      return -1;
   }

   // time complexity is O(1)
   void _initialize(T item) {
      _size = 1;
      _array = new T[1];
      _last = 0;
      _array[0] = item;
   }

};

void pawn_shop_items() {
   size_t cmd_count; // number of commands
   cin >> cmd_count;
   ArraySortedList<pair<double, string>> list;

   string cmd; // command: ADD, REMOVE or LIST
   for (size_t i = 0; i < cmd_count; ++i) {
      cin >> cmd;
      if (cmd == "ADD") {
         string p;
         cin >> p;
         p.erase(p.begin()); // skip $ sign
         double price = stod(p); string name;
         cin.ignore(); // do not read spaces before name
         getline(cin, name);
         list.add(make_pair(price, name));
      }
      else if (cmd == "REMOVE") {
         string p;
         cin >> p;
         p.erase(p.begin()); // skip $ sign
         double price = stod(p); string name;
         cin.ignore(); // do not read spaces before name
         getline(cin, name);
         if (!list.isEmpty()) {
            size_t index = list.indexOf(make_pair(price, name));
            if (index <= 10000) list.remove(index);
         }
      }
      else if (cmd == "LIST") {
         double from, to;
         string fs, ts;
         cin >> fs >> ts;
         fs.erase(fs.begin()); ts.erase(ts.begin());
         from = stod(fs); to = stod(ts);
         if (from <= to && !list.isEmpty()) {
            list.searchRange(from, to).display();
         }
         else {
            cout << endl;
         }
      }
   }
}

int main() {
   pawn_shop_items();

   return 0;
}
