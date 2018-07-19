package hubert.akka;

import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;

import java.util.ArrayList;

import hubert.akka.Master.NotFinished;
import hubert.akka.Master.UpdateEstimate;

public class Node extends AbstractActor {
  static public Props props() {
    return Props.create(Node.class, () -> new Node());
  }

  static public class Neighbours {
    public final ArrayList<ActorRef> neighs;

    public Neighbours(ArrayList<ActorRef> neighbours) {
      this.neighs = neighbours;
    }
  }

  static public class DistanceEstimate {
    public final int distance;

    public DistanceEstimate(Integer new_distance) {
      this.distance = new_distance;
    }
  }

  static public class AskDistance {
  };

  static public class Propagate {
  };

  private ArrayList<ActorRef> neigh;
  private int dist = Integer.MAX_VALUE;
  private int propagated = dist;
  private boolean has_propagated = true;
  private ActorRef supervisor;

  public Node() {
  }

  private void onDistanceEstimate(DistanceEstimate wtg) {
    if (wtg.distance >= this.dist)
      return;
    this.dist = wtg.distance;
    if (!this.has_propagated)
      return;
    // send the messages just for the first time, clear mailbox then
    this.supervisor.tell(new NotFinished(), getSelf());
    self().tell(new Propagate(), getSelf());
    this.has_propagated = false;
  }

  private void onPropagate() {
    for (ActorRef n : this.neigh) {
      n.tell(new DistanceEstimate(this.dist + 1), getSelf());
    }
    this.has_propagated = true;
    self().tell(new AskDistance(), getSelf());
  }

  private void onAskDistance() {
    if (has_propagated) // tell the supervisor only if no propagation is due
      supervisor.tell(new UpdateEstimate(this.dist), getSelf());
  }

  private void onNeighbours(Neighbours nb) {
    this.neigh = nb.neighs;
    this.supervisor = getSender();
  }

  @Override
  public Receive createReceive() {
    return receiveBuilder() //
        .match(DistanceEstimate.class, wtg -> onDistanceEstimate(wtg)) //
        .match(Propagate.class, mes -> onPropagate()) //
        .match(AskDistance.class, (mes) -> onAskDistance()) //
        .match(Neighbours.class, nb -> onNeighbours(nb)) //
        .matchEquals("show neighbours", mes -> {
          getSender().tell(neigh, getSelf());
        }).build();
  }
}
