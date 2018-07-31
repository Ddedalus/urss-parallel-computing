package hubert.akka
import akka.actor.ActorRef

trait SendingCorrections{
    var announced : Double = 0
    def getCorrection(newVal : Double) : Double = {
        val ret = newVal - announced
        announced = newVal
        return ret
    }

    def shouldSendCorrection(newVal : Double) : Boolean = {
        newVal - announced != 0
    }

    def resetCorrections(){
        announced = 0
    }
}