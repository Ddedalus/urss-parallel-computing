package hubert.akka;

import java.util.ArrayList;
import java.util.HashMap;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;
import akka.event.Logging;
import akka.event.LoggingAdapter;

import hubert.akka.Node.Neighbours;
import hubert.akka.Node.DistanceEstimate;

public class Master extends AbstractActor {
	static public Props props(String filename) {
		return Props.create(Master.class, () -> new Master(filename));
	}

	// Available Messages //
	static public class BuildGraph {
	};

	static public class RequestASP {
		public final Integer source;

		public RequestASP(Integer source) {
			this.source = source;
		}
	};

	static public class UpdateEstimate {
		public final double dist;

		public UpdateEstimate(double new_estimate) {
			this.dist = new_estimate;
		}
	};

	static public class NotFinished {
	};

	static public class GatherResults {
	};

	static private class NodeInfo {
		public boolean finished;
		public Double distance;
		public Integer id;

		public NodeInfo(Integer id) {
			this.distance = null;
			this.id = id;
			this.finished = false;

		}
	};

	// Members //
	public HashMap<ActorRef, NodeInfo> all_nodes = new HashMap<>();
	public HashMap<Integer, ActorRef> idToNode = new HashMap<>();
	public HashMap<Integer, ArrayList<Integer>> graph = new HashMap<>();
	private int finishedCount = 0, sourceID;
	private LoggingAdapter log = Logging.getLogger(getContext().getSystem(), this);

	// Worker functions //
	private void readGraph(String filename, boolean directed) {
		Scanner br;
		try {
			File f = new File(filename);
			br = new Scanner(f);
		} catch (FileNotFoundException e) {
			log.error("Cant access graph file " + filename);
			return;
		}

		Integer from, to;
		while (br.hasNextLine()) {
			if (br.hasNext("#")) {
				br.nextLine();
				continue;
			} // a comment line
			if (!br.hasNextInt()){
				log.error("No next int in the graph file!");
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
	}

	public Master(String filename) {
		readGraph(filename, false);
		log.warning("Graph: \n" + graph.toString());
		// this is not our graph...
	}

	@Override
	public void postStop() {
		log.warning("Master terminated!");
	}

	private void onBuildGraph() {
		log.warning("In BuildGraph");
		for (Integer n : graph.keySet()) { // create empty nodes
			final ActorRef n_actor = getContext().actorOf(Node.props(), "Node" + n);
			all_nodes.put(n_actor, new NodeInfo(n));
			idToNode.put(n, n_actor);
		}

		for (Integer n : graph.keySet()) { // pass info on graph
			ArrayList<Integer> neigh_ids = graph.get(n);
			ArrayList<ActorRef> neigh_a = new ArrayList<>();
			neigh_ids.forEach((id) -> neigh_a.add(idToNode.get(id)));
			idToNode.get(n).tell(new Neighbours(neigh_a), getSelf());
		}
		getSender().tell(idToNode, getSelf());
	}

	private void onRequestASP(RequestASP mes) {
		this.sourceID = mes.source;
		this.idToNode.get(sourceID).tell(new DistanceEstimate(0), getSelf());
	}

	private void onUpdateEstimate(UpdateEstimate mes) {
		NodeInfo nodeInfo = this.all_nodes.get(getSender());
		nodeInfo.distance = mes.dist;
		if (!nodeInfo.finished)
			finishedCount++;
		nodeInfo.finished = true;
		if (finishedCount == all_nodes.size())
			self().tell(new GatherResults(), getSelf());
	}

	private void onNotFinished() {
		if (this.all_nodes.get(getSender()).finished) {
			this.all_nodes.get(getSender()).finished = false;
			this.finishedCount--;
		}
	}

	private void onGatherResults() {
		Double sum = 0.;
		if (finishedCount == all_nodes.size())
			for (NodeInfo ni : all_nodes.values()) {
				sum += ni.distance;
			}
		log.warning("Sum of shortes paths: " + sum);
	}

	@Override
	public Receive createReceive() {
		return receiveBuilder() //
				.match(BuildGraph.class, mes -> onBuildGraph()) //
				.match(BuildGraph.class, mes -> onBuildGraph()) //
				.match(RequestASP.class, mes -> onRequestASP(mes)) //
				.match(UpdateEstimate.class, mes -> onUpdateEstimate(mes)) //
				.match(NotFinished.class, mes -> onNotFinished()) //
				.match(GatherResults.class, mes -> onGatherResults()) //
				.build();
	}
}
