package hubert.akka;

import java.io.IOException;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;

import hubert.akka.Master.BuildGraph;
import hubert.akka.Master.RequestASP;


public class AkkaASP {


  public static void main(String[] args) {
    String filename = "/home/hubert/Code/Warwick/BSP/data/newcastle/n5.edges";
    final ActorSystem system = ActorSystem.create("asp");
    try{
      final ActorRef master = system.actorOf(Master.props(filename), "master");
      System.out.println(master);
      master.tell(new BuildGraph(), ActorRef.noSender());
      master.tell(new RequestASP(2), ActorRef.noSender());
      System.out.println("Press any key to continue...");
      System.in.read();
    }catch(IOException e){
      System.out.println("Interrupted!");
    }finally{
      system.terminate();
    }
    return;
  }
}
