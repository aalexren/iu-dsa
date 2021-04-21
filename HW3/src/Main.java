import java.lang.reflect.Array;
import java.util.*;
import java.util.stream.Collectors;

class Vertex<E> {
    public E name;

    public Vertex(E value) {
        this.name = value;
    }
}

class Edge<E, V> {

    public Edge(Vertex<E> from, Vertex<E> to, V weight) {
        this.from = from;
        this.to = to;
        this.weight = weight;
    }

    public Vertex<E> from;
    public Vertex<E> to;
    public V weight;
}

interface Graph<E, V> {
    Vertex<E> addVertex(E value);
    Vertex<E> removeVertex(Vertex<E> v);
    Edge<E, V> addEdge(Vertex<E> from, Vertex<E> to, V weight);
    Edge<E, V> removeEdge(Edge<E, V> e);
    ArrayList<Edge<E, V>> edgesFrom(Vertex<E> v);
    ArrayList<Edge<E, V>> edgesTo(Vertex<E> v);
    Vertex<E> findVertex(E value);
    Edge<E, V> findEdge(E from_value, E to_value);
    boolean hasEdge(Vertex<E> u, Vertex<E> v);
}

class AdjacencyMatrixGraph<E, V> implements Graph<E, V> {

    public HashMap<Vertex<E>, HashMap<Vertex<E>, Edge<E, V>>> matrix;

    AdjacencyMatrixGraph() {
        matrix = new HashMap<>();
    }

    @Override
    public Vertex<E> addVertex(E value) {
        Vertex<E> v = new Vertex<>(value);

        matrix.put(v, new HashMap<>());
        return v;
    }

    @Override
    public Vertex<E> removeVertex(Vertex<E> v) {
        matrix.remove(v);

        // looking for that vertex like adjacent
        for (Vertex<E> k: matrix.keySet()) {
            matrix.get(k).remove(v);
        }

        return v;
    }

    @Override
    public Edge<E, V> addEdge(Vertex<E> from, Vertex<E> to, V weight) {
        Edge<E, V> e = new Edge<>(from, to, weight);
        matrix.get(from).put(to, e);

        return e;
    }

    @Override
    public Edge<E, V> removeEdge(Edge<E, V> e) {
        matrix.get(e.from).remove(e.to);

        return e;
    }

    /**
     * return a collection or edge objects that are going from vertex v;
     */
    @Override
    public ArrayList<Edge<E, V>> edgesFrom(Vertex<E> v) {
        return new ArrayList<>(matrix.get(v).values());
    }

    /**
     * return a collection or edge objects that are going into vertex v;
     */
    @Override
    public ArrayList<Edge<E, V>> edgesTo(Vertex<E> v) {
        ArrayList<Edge<E, V>> ret = new ArrayList<>();

        for (Vertex<E> k: matrix.keySet()) {
            if (v != k && matrix.get(k).containsKey(v)) {
                ret.add(matrix.get(k).get(v));
            }
        }

        return ret;
    }

    @Override
    public Vertex<E> findVertex(E value) {
        Vertex<E> ret = null;

        for (Vertex<E> k: matrix.keySet()) {
            if (value.equals(k.name)) {
                ret = k;
            }
        }

        return ret;
//        return matrix.keySet().stream().filter(v -> v.name == value).findFirst().get();
    }

    @Override
    public Edge<E, V> findEdge(E from_value, E to_value) {
//        Vertex<E> from = matrix.keySet().stream().filter(v -> v.name == from_value).findFirst().get();
//        Vertex<E> to = matrix.keySet().stream().filter(v -> v.name == to_value).findFirst().get();
        Vertex<E> from = findVertex(from_value);
        Vertex<E> to = findVertex(to_value);

        return matrix.get(from).get(to);
    }

    @Override
    public boolean hasEdge(Vertex<E> u, Vertex<E> v) {
        return matrix.get(u).containsKey(v);
    }
}

class UserInput<E, V> {
//    public AdjacencyMatrixGraph<E, V> graph;
    public AdjacencyMatrixGraph<String, Integer> graph;

    public UserInput() {
        graph = new AdjacencyMatrixGraph<>();
    }

    public void startLoop() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            String[] strs = scanner.nextLine().split(" ");
            getCommand(strs[0], Arrays.copyOfRange(strs, 1, strs.length));
        }
    }

    public void getCommand(String cmd, String[] args) {
        String ret = null;

        switch(cmd) {
            case "ADD_VERTEX": execAddVertex(args); break;
            case "REMOVE_VERTEX": execRemoveVertex(args); break;
            case "ADD_EDGE": execAddEdge(args); break;
            case "REMOVE_EDGE": execRemoveEdge(args); break;
            case "HAS_EDGE": ret = execHasEdge(args); break;
        }

        if (ret != null) System.out.println(ret);
    }

    private void execAddVertex(String[] args) {
//        E name = (E) args[0];
        String name = args[0];
        graph.addVertex(name);
    }

    private void execRemoveVertex(String[] args) {
//        E name = (E) args[0];
        String name = args[0];
//        Vertex<E> v = graph.findVertex(name);
        Vertex<String> v = graph.findVertex(name);
        graph.removeVertex(v);
    }

    private void execAddEdge(String[] args) {
//        E from = (E) args[0];
//        E to = (E) args[1];
//        V weight = (V) args[2];
        String from = args[0];
        String to = args[1];
        Integer weight = Integer.parseInt(args[2]);

//        Vertex<E> from_v = graph.findVertex(from);
//        Vertex<E> to_v = graph.findVertex(to);
        Vertex<String> from_v = graph.findVertex(from);
        Vertex<String> to_v = graph.findVertex(to);
        graph.addEdge(from_v, to_v, weight);
    }

    private void execRemoveEdge(String[] args) {
//        E from = (E) args[0];
//        E to = (E) args[1];
        String from = args[0];
        String to = args[1];

//        Edge<E, V> e = graph.findEdge(from, to);
        Edge<String, Integer> e = graph.findEdge(from, to);
        graph.removeEdge(e);
    }

    private String execHasEdge(String[] args) {
//        E from = (E) args[0];
//        E to = (E) args[1];
        String from = args[0];
        String to = args[1];

//        Vertex<E> from_v = graph.findVertex(from);
//        Vertex<E> to_v = graph.findVertex(to);
        Vertex<String> from_v = graph.findVertex(from);
        Vertex<String> to_v = graph.findVertex(to);

        return graph.hasEdge(from_v, to_v) ? "TRUE" : "FALSE";
    }
}

public class Main {

    public static void main(String[] args) {
        UserInput<String, Integer> userInput = new UserInput<>();

        userInput.startLoop();
    }
}
