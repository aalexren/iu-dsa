import javafx.util.Pair;

import java.util.ArrayList;
import java.util.PriorityQueue;

public class Main {

    public static int networkDelayTime(int[][] times, int n, int k) {
        PriorityQueue<Pair<Integer, Integer>> pq = new PriorityQueue<Pair<Integer, Integer>>();
        Integer[] d = new Integer[n + 1];

        // MAKE the D distances
        d[k] = 0;
        for (int i = 1; i < d.length; ++i) {
            if (i != k) d[i] = Integer.MAX_VALUE;
        }

        // FILL the PQ
//        for (int i = 1; i < d.length; ++i) {
//            pq.add(new Pair<>(d[i], i));
//        }

        pq.add(new Pair<>(0, k));
        while (!pq.isEmpty()) {
            Pair<Integer, Integer> u = pq.poll();
            for (int i = 0; i < times.length; ++i) {
                if (times[i][0] == u.getValue()) {
                    if (d[u.getValue()] + times[i][2] < d[times[i][1]]) {
                        d[times[i][1]] = d[u.getValue()] + times[i][2];
//                        pq.remove(p);
                        pq.add(new Pair<>(d[i + 1], i));
                    }
                }
            }
        }

        return d[2];
    }

    public static void main(String[] args) {
        int[][] times = {{2,1,1},{2,3,1},{3,4,1}};
        int k = 2;
        int n = 4;

        System.out.println(networkDelayTime(times, n, k));
    }
}
