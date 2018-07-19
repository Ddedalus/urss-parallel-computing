package hubert.akka;

import static org.junit.Assert.assertEquals;

import java.time.Duration;
import java.util.ArrayList;
import java.util.HashMap;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.testkit.javadsl.TestKit;
import hubert.akka.Master;

public class AkkaASPTest {
    static ActorSystem system;

    @BeforeClass
    public static void setup() {
        system = ActorSystem.create();
    }

    @AfterClass
    public static void teardown() {
        TestKit.shutdownActorSystem(system);
        system = null;
    }

    @Test
    public void testBuildGraph() {
        new TestKit(system) {
            {
                final ActorRef master = system
                        .actorOf(Master.props("/home/hubert/Code/Warwick/BSP/data/sample/sample.graph.txt"), "master");
                // within(Duration.ofSeconds(1), () -> {return null;});
                master.tell(new Master.BuildGraph(), getRef());
                HashMap<Integer, ActorRef> idmMap = (HashMap<Integer, ActorRef>) receiveN(1, Duration.ofSeconds(1)).get(0); 
                
                // test if second node has a correct neighbour
                idmMap.get(2).tell("show neighbours", getRef());
                ArrayList<ActorRef> neigh = (ArrayList<ActorRef>) receiveN(1, Duration.ofSeconds(1)).get(0);
                assert(neigh.contains(idmMap.get(78)));
                // test if graph is undriected:
                idmMap.get(78).tell("show neighbours", getRef());
                neigh = (ArrayList<ActorRef>) receiveN(1, Duration.ofSeconds(1)).get(0);
                assert(neigh.contains(idmMap.get(2)));
            }
        };
    }
}
