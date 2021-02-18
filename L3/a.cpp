#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <algorithm>

#define ll long long

using namespace std;

template <typename K, typename V>
struct pair_t : pair<K, V> {
//    bool operator ()(const pair_t<K, V>& p1, const pair_t<K, V>& p2) {
//        return p1.second == p2.second ? p1.first < p2.first : p1.second > p2.second;
//    }
};

template <typename K, typename V>
bool cmp(const pair_t<K, V>& p1, const pair_t<K, V>& p2) {
    return p1.second == p2.second ? p1.first < p2.first : p1.second > p2.second;
}

template <typename K, typename V>
class Map {
public:
    vector<pair_t<K, V>> _map;
    ll _size;

    Map(ll size) {
        _size = 0;
        _map.resize(size);
    }

    V get(K key) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);

        if (_map[compression].first != key) {
            for (ll i = compression; i < _map.size(); ++i) {
                if (_map[i].first == key) {
                    return _map[i].second;
                }
            }
            for (ll i = 0; i < compression; ++i) {
                if (_map[i].first == key) {
                    return _map[i].second;
                }
            }
            return 0;
        }

        return _map[compression].second;
    }

    void put(K key, V value) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);

        // COLLISION -> different keys, same hash
        if (_map[compression].first != key && !_map[compression].first.empty()) {
            for (ll i = compression + 1; i < _map.size(); ++i) {
                if (_map[i].second == 0 || _map[i].first == key) {
                    _map[i].first = key;
                    _map[i].second = value;
                    _size++;
                    return;
                }
            }
            for (ll i = 0; i < compression; ++i) {
                if (_map[i].second == 0 || _map[i].first == key) {
                    _map[i].first = key;
                    _map[i].second = value;
                    _size++;
                    return;
                }
            }
            return;
        }

        _map[compression].first = key;
        _map[compression].second = value;
        _size++;
    }

    void remove(K key) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);
        // TODO
        _map.erase(_map.begin() + compression, _map.begin() + compression + 1);
    }

    ll _getHash(K key) {
        const ll magic = 17;
        ll result = 0, i = 0;
        ll big_const = 1000000000;

        for (auto it: key) {
            result += (long long)(pow(magic, i) * (it - '0' + 1)) % big_const;
        }

        return result;
    }

    ll _compression(ll hash) {
        return _map.empty() ? 0 : hash % _map.size();
    }

//    bool operator <(const pair<K, V>& p1) const {
//        return p1.second == p2.second ? p1.first < p2.first : p1.second > p2.second;
//    }
};

int main() {

    ll n; cin >> n;
    Map<string, ll> map(10000);

    string s;
    cin.ignore();
    getline(cin, s);
    string buf;
    istringstream ss(s);
    while (ss >> buf) {
        if (map.get(buf) == 0) {
            map.put(buf, 1);
        } else {
            map.put(buf, map.get(buf) + 1);
        }
    }

    sort(map._map.begin(), map._map.end(), cmp<string, ll>);
    for (int i = 0; i < map._size; ++i) {
        if (map._map[i].second != 0)
            cout << map._map[i].first << " " << map._map[i].second << endl;
    }

//    map.put("123", 123);
//    map.put("124", 124);
//    map.put("321", 321);
//    map.put("132", 132);
//    map.put("142", 142);
//    map.put("312", 312);
//    map.put("231", 231);
//    map.put("421", 421);
//    map.put("213", 213);
//
//    cout << map.get("123") << endl;
//    cout << map.get("124") << endl;
//    cout << map.get("321") << endl;
//    cout << map.get("132") << endl;
//    cout << map.get("142") << endl;
//    cout << map.get("312") << endl;
//    cout << map.get("231") << endl;
//    cout << map.get("421") << endl;
//    cout << map.get("213") << endl;

//    for (int i = 22; i < 1000; ++i) {
//        string s = to_string(char(i*2)) + to_string(char(i*3)) + to_string(char(i));
//        map.put(s, i);
//        cout << s << " " << i << endl;
//    }
//    cout << "----------------------" << endl;
//    for (int i = 22; i < 1000; ++i) {
//        string s = to_string(char(i*2)) + to_string(char(i*3)) + to_string(char(i));
//        cout << s << " " << map.get(s) << endl;
//    }
//    map.put("rodion", 2000);
//    cout << map.get("rodion");

    return 0;
}
