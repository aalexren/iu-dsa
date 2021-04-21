import java.util.Iterator;

public class Main {

    public static class Graph<E, V> {
        DoublyLinkedList<Vertex> vertices;
        DoublyLinkedList<Edge> edges;

        public Graph() {
            this.vertices = new DoublyLinkedList<>();
            this.edges = new DoublyLinkedList<>();
        }

        public void print() {
            for (Edge e: edges) {
                System.out.println(e.from.value + "--" + e.weight + "--" + e.to.value);
            }
        }

        public class Vertex {
            V value;
            DoublyLinkedList<Vertex>.Node refVertex;

            DoublyLinkedList<Edge> incidentEdges;

            public Vertex(V value) {
                this.value = value;
                this.incidentEdges = new DoublyLinkedList<>();
            }
        }

        public class Edge {
            E weight;
            Vertex from;
            Vertex to;

            DoublyLinkedList<Edge>.Node refTo;
            DoublyLinkedList<Edge>.Node refFrom;
            DoublyLinkedList<Edge>.Node refEdge;
            public Edge(E weight, Graph<E, V>.Vertex from, Graph<E, V>.Vertex to) {
                this.weight = weight;
                this.from = from;
                this.to = to;
            }
        }

        public Vertex insertVertex(V value) {
            Vertex v = new Vertex(value);
            DoublyLinkedList<Vertex>.Node ref = vertices.add(v);
            v.refVertex = ref;

            return v;
        }

        public Edge insertEdge(E w, Vertex from, Vertex to) {
            Edge e = new Edge(w, from, to);
            DoublyLinkedList<Edge>.Node ref = edges.add(e);
            e.refEdge = ref;

            DoublyLinkedList<Edge>.Node refto = from.incidentEdges.add(e);
            DoublyLinkedList<Edge>.Node reffrom = to.incidentEdges.add(e);
            to.incidentEdges.add(e);

            return e;
        }

        // O(degree)
        public boolean areAdjacent(Vertex u, Vertex v) {
            if (v.incidentEdges.size() < u.incidentEdges.size()) {
                areAdjacent(v, u);
            }

            for (Edge e: u.incidentEdges) {
                if ((e.from == u && e.to == v) || (e.from == v && e.to == u)) {
                    return true;
                }
            }

            return false;
        }

        // O(1)
        public void removeEdge(Edge e) {
            edges.remove(e.refEdge);
            e.to.incidentEdges.remove(e.refTo);
            e.to.incidentEdges.remove(e.refFrom);
        }

        // O(degree(v))
        public void removeVertex(Vertex v) {
            vertices.remove(v.refVertex);

            for (Edge e: v.incidentEdges) {
                if (e.from == v || e.to == v) {
                    removeEdge(e);
                }
            }
        }

        // O(1)
        public int degree(Vertex v) {
            return v.incidentEdges.size();
        }

        public Iterator<Vertex> neighborhood(Vertex v, int n) {
            
        }
    }

    public static void main(String[] args) {
	    Graph<Integer, String> g = new Graph<>();

	    Graph.Vertex kazan = g.insertVertex("Kazan");
        Graph.Vertex moscow = g.insertVertex("Moscow");
        Graph.Vertex innopolis = g.insertVertex("Innopolis");
        Graph.Vertex samara = g.insertVertex("Samara");

        g.insertEdge(700, moscow, kazan);
        g.insertEdge(40, kazan, innopolis);
        Graph.Edge m_s = g.insertEdge(1071, moscow, samara);

        g.print();

        System.out.println(g.areAdjacent(moscow, innopolis));
        System.out.println(g.areAdjacent(moscow, kazan));

        System.out.println("Degree of moscow = " + g.degree(moscow));
       // g.removeEdge(m_s);

        g.print();
    }
}
