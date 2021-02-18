#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include <algorithm>

#define ll long long

using namespace std;

template <typename K, typename V>
class Map {
public:
    vector< vector< pair<K, V> > > _map;
    ll _size;

    Map(ll size) {
        _size = 0;
        _map.resize(size);
    }

    V get(K key) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);

        if (_map[compression].size() > 0 && _map[compression][0].first != key) {
            for (ll i = 0; i < _map[compression].size(); ++i) {
                if (_map[compression][i].first == key) {
                    return _map[compression][i].second;
                }
            }
            return 0;
        }

        return _map[compression].empty() ? 0 : _map[compression][0].second;
    }

    void put(K key, V value) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);

        // COLLISION -> different keys, same hash
        if (_map[compression].size() > 0) {
            if (_map[compression][0].first != key && !_map[compression][0].first.empty()) {
                for (ll i = 0; i < _map[compression].size(); ++i) {
                    if (_map[compression][i].second == 0) {
                        _map[compression].push_back(make_pair(key, value));
                        _size++;
                        return;
                    } else if (_map[compression][i].first == key) {
                        _map[compression][i].second = value;
                        return;
                    }
                }
                _map[compression].push_back(make_pair(key, value));
                return;
            }
            else {
                ll index = 0;
                while (_map[compression][index].first != key) index++;
                _map[compression][0].second = value;
            }
        }
        else {
            _map[compression].push_back(make_pair(key, value));
        }

        _size++;
    }

    void remove(K key) {
        ll hash = _getHash(key);
        ll compression = _compression(hash);
        // TODO
        _map.erase(_map.begin() + compression, _map.begin() + compression + 1);
    }

    ll _getHash(K key) {
        const ll magic = 7;
        ll result = 0, i = 0;
        ll big_const = 35945003;

        for (auto it: key) {
            result += (long long)(31 * pow(magic, i) * (it - '0' + 1)) % big_const;
        }

        return result;
    }

    ll _getHash(ll hash) {
        return 31 * hash * 17 % _map.size();
    }

    ll _compression(ll hash) {
        ll a = 54095, b = 17, p = 76963;
        return ((a * hash + b) % p) % _map.size();
        return _map.empty() ? 0 : hash % _map.size();
    }
};

int main() {

    ll n; cin >> n;
    Map<string, ll> map(n);//137700+7);
    vector<string> unique;

    string s;
    cin.ignore();
    getline(cin, s);
    string buf;
    istringstream ss(s);
    while (ss >> buf) {
        map.put(buf, 1);
//        if (map.get(buf) == 0) {
//            map.put(buf, 1);
//        } else {
//            // put 2 is crutch
//            map.put(buf, 2);
//        }
    }

    cin >> n;
    cin.ignore();
    getline(cin, s);
    ss = istringstream(s);
    while (ss >> buf) {
        if (map.get(buf) == 0) {
            map.put(buf, 1);
            unique.push_back(buf);
        } else {
            // put 2 is crutch
//            map.put(buf, 2);
        }
    }

    cout << unique.size() << endl;
    for (int i = 0; i < unique.size(); ++i) {
        cout << unique[i] << " ";
    }

    return 0;
}
