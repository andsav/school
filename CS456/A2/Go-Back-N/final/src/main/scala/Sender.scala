import java.net.{InetAddress, DatagramPacket, DatagramSocket}
import java.io.{File, PrintWriter}
import scala.io.Source

object Sender extends App {
  require(args.length == 4)

  val MAX_READ_SIZE = 500
  val WINDOW_SIZE = 10
  val TIMEOUT = 100

  var baseNum = 0
  var nextSeqNum = 0

  val nEmulatorAddress = InetAddress.getByName(args(0))
  val nEmulatorPort = Integer.parseInt(args(1))
  val senderPort = Integer.parseInt(args(2))
  val socket = new DatagramSocket(senderPort)


  (new AckReceiver).run()

}

class ChunksSender extends Runnable {
  val seqNumWriter = new PrintWriter(new File("seqnum.log"))

  def run(): Unit ={
    try {


    } catch {
      case e: Exception => println("Chunks sender aborted: " + e.toString)
    } finally {
      seqNumWriter.close()
    }
  }
}

class AckReceiver extends Runnable {

  val ackWriter = new PrintWriter(new File("ack.log"))

  def run(): Unit = {
    try {



    } catch {
      case e: Exception => println("Ack receiver aborted: " + e.toString)
    } finally {
      ackWriter.close()
    }
  }
}