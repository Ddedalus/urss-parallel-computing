package hubert.akka;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;


public class ReadGraph {

    public static HashMap<Integer, Integer[]> readGraph(String filename, boolean directed) {
        HashMap<Integer, ArrayList<Integer>> graph = new HashMap<>();
        Scanner br;
        try {
            File f = new File(filename);
            br = new Scanner(f);
        } catch (FileNotFoundException e) {
            System.out.println("Cant access graph file " + filename);
            return new HashMap<Integer, Integer[]>();
        }

        Integer from, to;
        while (br.hasNextLine()) {
            if (br.hasNext("#")) {
                br.nextLine();
                continue;
            } // a comment line
            if (!br.hasNextInt()) {
                System.out.println("No next int in the graph file!");
                break;
            }
            from = br.nextInt();
            to = br.nextInt();

            if (!graph.containsKey(from))
                graph.put(from, new ArrayList<>());
            graph.get(from).add(to);

            if (!directed) {
                if (!graph.containsKey(to))
                    graph.put(to, new ArrayList<>());
                graph.get(to).add(from);
            }

        }
        br.close();
        HashMap<Integer, Integer[]> ret = new HashMap<>();
        for(Integer key : graph.keySet()){
            ArrayList<Integer> val = graph.get(key);
            ret.put(key, val.toArray(new Integer[val.size()]));
        }
        graph.clear();
        return ret;
    }
}