//
// Created by Артём Черница on 19.02.21.
//

#include <iostream>
#include <string>
#include <iomanip>

#define ll long long

using namespace std;

template <typename K, typename V>
class MapADT {
public:
   virtual V get(K key) = 0;
   virtual void put(K key, V value) = 0;
   virtual size_t size() = 0;
   virtual bool isEmpty() = 0;
//   virtual void remove(K key) = 0;
   virtual pair<K,V>* entrySet() = 0;
//   virtual K* keySet() = 0;
//   virtual List<V> values() = 0;
};

/*
 * This HashMap uses standard hash-function and linear probing approach.
 */
template <typename K, typename V>
class HashMap : public MapADT<K, V> {
public:
   HashMap() {
      size_ = 0;
      memsize_ = 0;
      ratio_ = 0.0;
      keys = nullptr;
      values = nullptr;
      occupied = nullptr;
   }

   HashMap(size_t memsize) {
      size_ = 0;
      ratio_ = 0.0;
      memsize_ = memsize;
      keys = new K[memsize_];
      values = new V[memsize_];
      occupied = new bool[memsize_];
   }

   V get(K key) override {
      if (isEmpty()) throw out_of_range("no elements");

      size_t index = mad(hash<K>()(key));

      // what if keys[index] cause exception?
      if (occupied[index] && keys[index] == key) {
         return values[index];
      }

      for (size_t i = index + 1; i < memsize_; ++i) { // from index + 1 to end
         if (occupied[i] && keys[i] == key) {
            return values[i];
         }
      }
      for (size_t i = 0; i < index; ++i) { // from beginning to index
         if (occupied[i] && keys[i] == key) {
            return values[i];
         }
      }

      // else we didn't find the key, so throw the exception
      throw out_of_range("not in map");
   } // get()

   void put(K key, V value) override {
      if (memsize_ != 0) {
         ratio_ = (double) size_ / (double) memsize_; // ---------------------> DANGEROUS <--------------------- size_t to double
      }
      if (ratio_ >= 0.51 || memsize_ == 0) {
         resize();
      }

      size_t index = mad(hash<K>()(key));

      if (keys[index] == key) { // if the same key, then just update value
         keys[index] = key;
         values[index] = value;
         return;
      }

      if (!occupied[index]) { // if cell is free
         keys[index] = key;
         values[index] = value;
         occupied[index] = true;
         size_++; // successfully added a new element
         return;
      }

      // finite cycle guarantee, because size of the table almost twice bigger than number of elements
      while (occupied[index] && keys[index] != key) { // while cell is occupied
         index++;
         if (index >= memsize_) index = 0;
         if (!occupied[index]) {
            keys[index] = key;
            values[index] = value;
            occupied[index] = true;
            size_++; // successfully added a new element
            break;
         }
      }

      ratio_ = (double) size_ / (double) memsize_; // memsize_ not a zero, shouldn't be an exception
      if (ratio_ >= 0.51) {
         resize();
      }
   } // put()

   pair<K, V>* entrySet() override {
      if (isEmpty()) throw out_of_range("size is 0");

      pair<K, V>* entries = new pair<K, V>[size()];
      size_t it = 0;

      for (size_t i = 0; i < memsize_; ++i) {
         if (occupied[i]) { // if it's not free
            entries[it] = make_pair(keys[i], values[i]);
            it++;
         }
      }

      return entries;
   }

   size_t size() override {
      return size_;
   }

   bool isEmpty() override {
      return size_ == 0;
   }

private:
   size_t memsize_; // size of memory blocks allocated for hash table. Not an actually number of elements.
   size_t size_; // number of elements
   double ratio_; // load factor
   K* keys; // array of keys
   V* values; // array of values
   bool* occupied; // this array used for check whether the cell in hash table occupied or free:
                   // true - occupied, false - free

   size_t mad(size_t hash_) { // compression function
      // [(ay + b) mod p] mod N
      // a, b in [0, p - 1], a > 0
      size_t a = 2411, b = 30139, p = 970031, N = memsize_;
      // ---------------------> DANGEROUS <---------------------
      // -> h returns size_t type, so it can cause overflow <-
      if (N == 0) throw out_of_range("size is zero");
      return hash_ % N;
      //return (a * hash_ + b) % p % N;
   }

   /* Doubling strategy – is to double the size of the available hash table cells */
   void resize() {
      if (memsize_ == 0) {
         memsize_ = 1;
         K* temp_keys = new K[1];
         V* temp_values = new V[1];
         bool* temp_occupied = new bool[1]; temp_occupied[0] = false;
         delete[] keys; // ---------------------> DANGEROUS <---------------------
         delete[] values; // ---------------------> DANGEROUS <---------------------
         delete[] occupied; // ---------------------> DANGEROUS <---------------------
         occupied = temp_occupied;
         keys = temp_keys;
         values = temp_values;
         return;
      }

      memsize_ *= 2;
      K* temp_keys = new K[memsize_];
      V* temp_values = new V[memsize_];

      bool* temp_occupied = new bool[memsize_];
      for (size_t i = 0; i < memsize_; ++i) {
         temp_occupied[i] = false;
      }

      for (size_t i = 0; i < memsize_ / 2; ++i) { // recalculate indexes for keys
         size_t index = mad(hash<K>()(keys[i])); // calculate hash, then call compression function
         if (!temp_occupied[index]) { // if cell is free
            temp_keys[index] = keys[i]; // ---------------------> DANGEROUS <---------------------
            temp_values[index] = values[i]; // ---------------------> DANGEROUS <---------------------
            temp_occupied[index] = true;
         }
         else { // find next empty cell
            while (temp_occupied[index]) { // while cell is occupied
               index++;
               if (index >= memsize_) index = 0;
            }
            temp_keys[index] = keys[i]; // ---------------------> DANGEROUS <---------------------
            temp_values[index] = values[i]; // ---------------------> DANGEROUS <---------------------
            temp_occupied[index] = true;
         }
      }

      delete[] keys; // ---------------------> DANGEROUS <---------------------
      delete[] values; // ---------------------> DANGEROUS <---------------------
      delete[] occupied; // ---------------------> DANGEROUS <---------------------
      occupied = temp_occupied;
      keys = temp_keys;
      values = temp_values;
   } // resize()
};

/*
 * We use this class to save a set of uniques ID's for a certain date.
 * id field – hashmap, keeping only unique ID's as a keys and fake value with <bool>,
 * summary – summary income/profit for a certain day.
 */
template <typename K, typename V>
class Value {
public:
   Value() {
      summary = 0;// ---------------------> DANGEROUS <---------------------
                  // it should be only the number type, can cause exception
   }

   V getSum() {
      return summary;
   }

   size_t size() {
      return id.size();
   }

   void add(K key, V value) { // add unique key
      try {
         id.get(key);
      }
      catch (const exception& e) {
         id.put(key, true);
      }

      summary += value;
   }

private:
   /* actually there is better to implement set, but we can use map this "empty" value */
   HashMap<K, bool> id; // map of unique id's (keys) and we mock value using <bool> type
   V summary; // summary value for a certain day
};

int main() {
   HashMap<string, Value<string, double>> map(50000);

   int N; // number of requests/inquiries <= 50k
   cin >> N;
   for (int i = 0; i < N; ++i) {
      string date; // YYYY-MM-DD
      string time; // HH:MM:SS
      string ID; // #000-AA
      string price; // $00.00
      string name; // Cappucino
      cin >> date >> time >> ID >> price;
      cin.ignore();
      getline(cin, name);

      price.erase(price.begin()); // delete $ sign
      double price_ = stod(price);

      try {
         // if we found it, we update it
         auto val = map.get(date);
         val.add(ID, price_);
         map.put(date, val);
      }
      catch (const exception& e) {
         Value<string, double> val;
         val.add(ID, price_);
         map.put(date, val);
      }
   }

   auto list = map.entrySet();
   for (size_t i = 0; i < map.size(); ++i) {
      cout << list[i].first << " ";
      cout << "$" << setprecision(2) << fixed << list[i].second.getSum() << " ";
      cout << list[i].second.size() << endl;
   }

   return 0;
}

//template <typename K, typename V>
//class Map {
//public:
//   vector< vector< pair<K, V> > > _map;
//   ll _size;
//
//   Map(ll size) {
//      _size = 0;
//      _map.resize(size);
//   }
//
//   V get(K key) {
//      ll hash = _getHash(key);
//      ll compression = _compression(hash);
//
//      if (_map[compression].size() > 0 && _map[compression][0].first != key) {
//         for (ll i = 0; i < _map[compression].size(); ++i) {
//            if (_map[compression][i].first == key) {
//               return _map[compression][i].second;
//            }
//         }
//         return 0;
//      }
//
//      return _map[compression].empty() ? 0 : _map[compression][0].second;
//   }
//
//   void put(K key, V value) {
////      ll hash = _getHash(key);
//      ll hash = std::hash(key);
//      ll compression = _compression(hash);
//
//      // COLLISION -> different keys, same hash
//      if (_map[compression].size() > 0) {
//         if (_map[compression][0].first != key && !_map[compression][0].first.empty()) {
//            for (ll i = 0; i < _map[compression].size(); ++i) {
//               if (_map[compression][i].second == 0) {
//                  _map[compression].push_back(make_pair(key, value));
//                  _size++;
//                  return;
//               } else if (_map[compression][i].first == key) {
//                  _map[compression][i].second = value;
//                  return;
//               }
//            }
//            _map[compression].push_back(make_pair(key, value));
//            return;
//         }
//         else {
//            ll index = 0;
//            while (_map[compression][index].first != key) index++;
//            _map[compression][0].second = value;
//         }
//      }
//      else {
//         _map[compression].push_back(make_pair(key, value));
//      }
//
//      _size++;
//   }
//
//   void remove(K key) {
//      ll hash = _getHash(key);
//      ll compression = _compression(hash);
//      // TODO
//      _map.erase(_map.begin() + compression, _map.begin() + compression + 1);
//   }
//
//   ll _getHash(K key) {
//      const ll magic = 7;
//      ll result = 0, i = 0;
//      ll big_const = 35945003;
//
//      for (auto it: key) {
//         result += (long long)(31 * pow(magic, i) * (it - '0' + 1)) % big_const;
//      }
//
//      return result;
//   }
//
//   ll _getHash(ll hash) {
//      return 31 * hash * 17 % _map.size();
//   }
//
//   ll _compression(ll hash) {
//      ll a = 54095, b = 17, p = 76963;
//      return ((a * hash + b) % p) % _map.size();
//   }
//};