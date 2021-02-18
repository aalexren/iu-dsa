/*
 * Created by Artem Chernitsa B20-02
 */

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

template <typename T>
class ArraySortedList : public SortedList<T> {
public:
   ArraySortedList<T>() {
      _size = 0;
      _array = nullptr;
      _last = -1;
   }

   // add element to list keeping order of elements
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

   T least() override {
      return _array[0];
   }

   T greatest() override {
      return _array[_last];
   }

   T get(size_t i) override {
      return _array[i];
   }

   size_t indexOf(T item) override {
      for (size_t i = 0; i < size(); ++i) {
         if (_array[i].first < item.first < 0.003 && _array[i].second == item.second) {
            return i;
         }
      }
      return -1;
   }

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

   List<T> searchRange(double from, double to) {
      List<T> list;
//      size_t lb = 0, ub = _last; // lower_bound, upper_bound
//      size_t si, fi; // start index, finish index
//
//      while (true) {
//         size_t mid = (lb + ub) / 2;
//
//         if (lb == mid) {
//            if (_array[mid].first >= from) {
//               si = mid;
//               break; // mid
//            }
//         }
//
//         if (_array[mid].first < from) {
//            lb = mid + 1;
//            if (lb > ub) {
//               si = mid + 1;
//               break; // mid + 1
//            }
//         } else if (lb > ub) {
//            si = mid;
//            break; // mid
//         } else {
//            ub = mid - 1;
//         }
//      }
//
//      lb = 0, ub = _last;
//      while (true) {
//         size_t mid = (lb + ub) / 2;
//
//         if (lb == mid) {
//            if (_array[mid].first >= to) {
//               fi = mid;
//               break; // mid
//            }
//         }
//
//         if (_array[mid].first < to) {
//            lb = mid + 1;
//            if (lb > ub) {
//               fi = mid + 1;
//               break; // mid + 1
//            }
//         } else if (lb > ub) {
//            fi = mid;
//            break; // mid
//         } else {
//            ub = mid - 1;
//         }
//      }

//      for (size_t i = si; i < fi; ++i) {
      for (size_t i = 0; i < size(); ++i) {
         if (_array[i].first >= from && _array[i].first <= to) {
            list.push(_array[i]);
         }
      }

      return list;
   }

   List<T> searchRange(T from, T to) override {
//      ArraySortedList<T> list;
   }

   // return number of element actually added to the list
   size_t size() override {
      return _last + 1;
   }

   bool isEmpty() override {
      return _last == -1;
   }

private:
   size_t _size; // number of cells allocated in memory (not size like number of added elements)
   long long int _last; // index of the last element
   T* _array;

   // increase size of dynamic array factor times
   void _expand(size_t factor) {
      T* _p_temp = new T[_size * factor];//[_size * sizeof(T) * factor];

      for (size_t i = 0; i < _size; ++i) {
         _p_temp[i] = _array[i];
      }

      _array = _p_temp;
      _size = _size * factor;
   }

   // binary search for finding index where to put in
//   size_t _index_to_insert(T item) {
//      size_t l = 0, r = _last, mid = (l + r) / 2;
//
//      while (true) {
//
//         if (l == mid) {
//            if (_array[mid] > item) {
//               return mid; // first cell
//            }
//         }
//
//         if (_array[mid] < item) {
//            l = mid + 1; // in upper half
//            if (l > r) {
//               return mid + 1;
//            }
//         }
//         else if (l > r) {
//            return mid; // last cell
//         }
//         else {
//            r = mid - 1; // in lower half
//         }
//
//         mid = (l + r) / 2;
//      }
//   }

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


   // standard binary search
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
//   ArraySortedList<pair<double, string>> list;
//   for (int i = 0; i < 20; ++i) {
//      list.add(make_pair(double(i), "keks"));
////      list.remove(list.indexOf(make_pair(double((i-1)*2), "keks")));
//   }
//   for (int i = 0; i < 20; ++i) {
////      list.add(make_pair(double(i), "keks"));
//      list.remove(list.indexOf(make_pair(double((i-1)*2), "keks")));
//   }
//   list.searchRange(0.0, 10000.0).display();


//   pair<double, string> pp(make_pair(100.02, "Watch"));
//   pair<double, string> pw(make_pair(150.31, "Kettle"));
//   list.add(pp);
//   list.add(pw);
//   list.searchRange(101.0, 150.32).display();


//   list.add(pp);
//   list.add(pp);
//   list.add(pp);
//   list.add(pp);
//   ArraySortedList<int> list;
//   list.add(5);
//   list.add(8);
//   list.add(6);
//   list.add(-1);
//   list.add(10);
//   list.add(4);
////   for (int i = 0; i < list.size(); ++i) {
////      cout << list.get(i) << endl;
////      cout << "size: " << list.size() << endl;
////   }
//
//
//   list.remove(5);


//   for (int i = 0; i < list.size(); ++i) {
//      cout << list.get(i).first << endl;
//      cout << "size: " << list.size() << endl;
//   }

   return 0;
}
