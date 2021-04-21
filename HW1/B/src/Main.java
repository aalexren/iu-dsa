/* -> Created by Artem Chernitsa B20-02 <- */

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        HashMap<String, Value<String>> map = new HashMap<>();

        int N;
        Scanner scanner = new Scanner(System.in);
        N = scanner.nextInt();
        for (int i = 0; i < N; ++i) {
            String date = scanner.next().trim(); // YYYY-MM-DD
            String time = scanner.next(); // HH:MM:SS
            String ID = scanner.next(); // #000-AA
            String price = scanner.next(); // $00.00
            scanner.useDelimiter("\n");
            String name = scanner.next(); // Cappucino
            scanner.useDelimiter(" ");

            price = price.substring(1); // remove $ sign in the beginning of string
            double price_ = Double.parseDouble(price);

            Value<String> val = map.get(date);
            if (val != null) { // update existing value for the date
                val.add(ID, price_);
                map.put(date, val);
            }
            else { // add new value for the date
                val = new Value<>();
                val.add(ID, price_);
                map.put(date, val);
            }
        }

        List<Pair<String, Value<String>>> list = map.entrySet(); // get all not null <dates, values>
        ListIterator<Pair<String, Value<String>>> iter = new ListIterator<>(list);

        while (iter.hasNext()) {
            System.out.print(iter.current.data.key + " ");
            System.out.print("$" + iter.current.data.value.getSum() + " ");
            System.out.println(iter.current.data.value.size());
            iter.next();
        }
    }
}

class Node<T> {
    Node<T> next;
    T data;
}

/**
 * There is just a simple linked list with two pointers:
 * to the head and tail of the list.
 * All methods here run for a constant time (O(1)).
 */
class List<T> {
    List() {
        head = null;
        tail = null;
        size_= 0;
    }

    void pushBack(T item) {
        Node<T> node = new Node<>();
        node.data = item;
        node.next = null;

        if (head == null) {
            head = node;
            tail = node;
        }
        else {
            tail.next = node;
            tail = node;
        }

        size_++;
    }

    Node<T> getHead() {
        return head;
    }

    int size() {
        return size_;
    }

    private Node<T> head;
    private Node<T> tail;
    private int size_;
}

/**
 * Simple iterator for linked list.
 */
class ListIterator<T> {
    Node<T> current;

    public ListIterator(List<T> list) {
        current = list.getHead();
    }

    public boolean hasNext() {
        return current != null;
    }

    public T next() {
        T data = current.data;
        current = current.next;
        return data;
    }
}

class Pair<K, V> {
    public K key;
    public V value;

    public Pair(K key_, V value_) {
        this.key = key_;
        this.value = value_;
    }
}

interface MapADT<K, V> {
    V get(K key);
    void put(K key, V value);
    int size();
    boolean isEmpty();
    List<Pair<K, V>> entrySet();
}

/**
 * HashMap using Linear Probing and Open Addressing.
 */
class HashMap<K, V> implements MapADT<K, V> {

    // We can create it with a zero size
    HashMap() {
        size_ = 0;
        memsize_ = 0;
        ratio_ = 0.0;
        keys = null;
        values = null;
    }

    // We can create it with a certain size
    HashMap(int memsize) {
        size_ = 0;
        memsize_ = memsize;
        ratio_ = 0.0;
        keys = (K[]) new Object[memsize_];
        values = (V[]) new Object[memsize_];
    }

    @Override
    public V get(K key) {
        if (isEmpty()) return null;

        int index = mad(Math.abs(key.hashCode()));

        if (keys[index] != null && keys[index].equals(key)) {
            return values[index];
        }

        for (int i = index + 1; i < memsize_; ++i) { // from index + 1 to end
            if (keys[i] != null && keys[i].equals(key)) {
                return values[i];
            }
        }
        for (int i = 0; i < index; ++i) { // from beginning to index
            if (keys[i] != null && keys[i].equals(key)) {
                return values[i];
            }
        }

        return null;
    }

    @Override
    public void put(K key, V value) {
        if (memsize_ != 0) {
            ratio_ = (double) size_ / (double) memsize_;
        }
        if (ratio_ >= 0.51 || memsize_ == 0) {
            resize();
        }

        int index = mad(Math.abs(key.hashCode()));

        if (keys[index] != null && keys[index].equals(key)) { // if the same key, then just update value
            keys[index] = key;
            values[index] = value;
            return;
        }

        if (keys[index] == null) { // if cell is free
            keys[index] = key;
            values[index] = value;
            size_++; // successfully added a new element
            return;
        }

        // finite cycle guarantee, because size of the table almost twice bigger than number of elements
        while (keys[index] != null && !keys[index].equals(key)) { // while cell is occupied
            index++;
            if (index >= memsize_) index = 0;
            if (keys[index] == null) {
                keys[index] = key;
                values[index] = value;
                size_++; // successfully added a new element
                break;
            }
        }

        ratio_ = (double) size_ / (double) memsize_; // memsize_ not a zero, shouldn't be an exception
        if (ratio_ >= 0.51) {
            resize();
        }
    }

    @Override
    public int size() {
        return size_;
    }

    @Override
    public boolean isEmpty() {
        return size_ == 0;
    }

    @Override
    public List<Pair<K, V>> entrySet() {
        if (isEmpty()) return null;

        List<Pair<K, V>> entries = new List<>();
        int it = 0;

        for (int i = 0; i < memsize_; ++i) {
            if (keys[i] != null) { // if it's not free
                entries.pushBack(new Pair<K, V>(keys[i], values[i]));
                it++;
            }
        }

        return entries;
    }

    // reduced version of MAD compression function
    private int mad(int hash) {
        // hash mod N
        if (memsize_ == 0) return 0;
        return hash % memsize_;
    }

    private void resize() {
        if (memsize_ == 0) {
            memsize_ = 1;
            keys = (K[]) new Object[1];
            values = (V[]) new Object[1];
            return;
        }

        memsize_ *= 2;
        K[] tmp_keys = (K[]) new Object[memsize_];
        V[] tmp_values = (V[]) new Object[memsize_];

        // copy all values to the new arrays
        for (int i = 0; i < memsize_ / 2; ++i) {
            if (keys[i] != null) {
                int index = mad(Math.abs(keys[i].hashCode()));
                if (tmp_keys[index] == null) {
                    tmp_keys[index] = keys[i];
                    tmp_values[index] = values[i];
                } else {
                    while (tmp_keys[index] != null) {
                        index++;
                        if (index >= memsize_) index = 0;
                    }
                    tmp_keys[index] = keys[i];
                    tmp_values[index] = values[i];
                }
            }
        }

        // assign to the original fields
        keys = tmp_keys;
        values = tmp_values;
    }

    private int size_; // size of not null elements
    private int memsize_; // capacity of allocated memory (size of arrays)
    private double ratio_; // load factor of hash map
    K[] keys;
    V[] values;
}

/**
 * This class created special for this task.
 * It keeps the summary profit for a certain date,
 * also to keep unique ID's we use HashMap as a field with a K key and "fake" value.
 */
class Value<K> {
    Value() {
        summary = 0;
        id = new HashMap<K, Object>();
    }

    double getSum() {
        return summary;
    }

    int size() {
        return id.size();
    }

    void add(K key, double value) {
        if (id.get(key) == null) {
            id.put(key, null);
        }

        summary += value;
    }

    private HashMap<K, Object> id;
    private double summary;
}
