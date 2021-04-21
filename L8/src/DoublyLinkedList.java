import java.util.Iterator;

public class DoublyLinkedList<E> implements Iterable<E>{

    public class DoublyLinkedListIterator implements Iterator<E> {
        Node current;

        public DoublyLinkedListIterator(Node current) {
            this.current = current;
        }

        @Override
        public boolean hasNext() {
            return (current != null);
        }

        @Override
        public E next() {
            E value = current.value;
            current = current.next;
            return value;
        }
    }

    @Override
    public Iterator<E> iterator() {
        return new DoublyLinkedListIterator(this.head);
    }

    public class Node {
        E value;
        Node prev = null;
        Node next = null;

        public Node(E value) {
            this.value = value;
        }
    }

    private Node head;
    private Node tail;
    private int size;

    public DoublyLinkedList() {
        this.head = null;
        this.tail= null;
        this.size = 0;
    }

    public Node add(E value) {
        Node node = new Node(value);
        if (head == null) {
            head = node;
            tail = node;
        } else {
            node.next = head;
            head.prev = node;
            head = node;
        }
        size += 1;

        return node;
    }

    public int size() {
        return this.size;
    }

    public void remove(Node n) {
        if (n.prev == null) {
            head = n.next;
            if (n.next == null) {
                tail = null;
            } else {
                n.next.prev = null;
            }
        } else if (n.next == null) {
            tail = n.prev;
            n.prev.next = null;
        } else {
            n.next.prev = n.prev;
            n.prev.next = n.next;
        }
        size -= 1;
    }
}