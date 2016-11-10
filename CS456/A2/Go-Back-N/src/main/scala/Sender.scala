import java.net.{InetAddress, DatagramPacket, DatagramSocket}
import java.io.{File, PrintWriter}
import javax.swing.{Timer, AbstractAction}
import java.awt.event.ActionEvent
import scala.io.Source

object Sender extends App {
  require(args.length == 4)

  // Initialization
  val seqNumFile = Helpers.openFile(Constants.SEQ_NUM_LOG)
  val ackFile = Helpers.openFile(Constants.ACK_LOG)

  val (nEmulatorAddress, nEmulatorPort, senderPort, fileName) = Helpers.parseArgs(args)

  val fileContents = Helpers.getFileContents(fileName)
  val packetsToSend = Helpers.buildPackets(fileContents, Constants.MAX_READ_SIZE)
  val socket = new DatagramSocket(senderPort)

  var currentMod = -1
  var base = 0
  var nextSeqNum = 0
  var end = false

  // Main program
  try {
    new Thread(new PacketSender(seqNumFile, packetsToSend)).start()
    new Thread(new AckReceiver(ackFile)).start()
  }
  catch {
    case e: Exception => println("Sender aborted: " + e.toString)
  }

}

class PacketSender(log: PrintWriter, packets: Array[packet]) extends Runnable {
  val timeOut = new AbstractAction() {
    def actionPerformed(e : ActionEvent): Unit = timeout()
  }
  val timer = new Timer(Constants.TIMEOUT_DELAY, timeOut)

  def run(): Unit = {
    try {
      println(packets.length + " packets to send")

      while(!Sender.end) {
        if(Sender.nextSeqNum < Sender.base + Constants.WINDOW_SIZE) {
          sendPacket(Sender.nextSeqNum)

          if(Sender.base == Sender.nextSeqNum)
            timer.restart()

          Sender.nextSeqNum += 1
        }
      }

      timer.stop()

    } catch {
      case e: Exception => {
        println("Packet sender aborted: " + e.toString)
        Sender.end = true
      }
    } finally {
      log.close()
    }
  }

  def udtSend(p: packet): Unit = {
    Helpers.udtSend(p, Sender.socket, Sender.nEmulatorAddress, Sender.nEmulatorPort)
  }

  def sendPacket(i: Int): Unit = {
    if(i < packets.length) {
      udtSend(packets(i))
      println("SENT packet " + i)
      Helpers.writeLog(log, i)
    }
    else sendEOT()
  }

  def sendEOT() : Unit = {
    udtSend(packet.createEOT(0))
    println("SENT EOT")
  }

  def timeout(): Unit = {
    if(Sender.base == Sender.nextSeqNum) {
      timer.stop()
    }
    else {
      timer.start()

      println("TIMEOUT! resending packets " + Sender.base + " to " + (Sender.nextSeqNum-1).toString)

      (Sender.base until Sender.nextSeqNum).foreach(sendPacket)
    }

  }
}

class AckReceiver(log: PrintWriter) extends Runnable {
  def run(): Unit = {
    try {
      log.print("")

      var udpPacket = Helpers.emptyUdpPacket()

      while(!Sender.end) {
        Sender.socket.receive(udpPacket)
        val p = packet.parseUDPdata(udpPacket.getData())

        p.getType() match {
          case Constants.PACKET_TYPE_ACK => {
            if(p.getSeqNum%32 == 0)
              Sender.currentMod += 1

            val realSeqNum = Sender.currentMod*32 + p.getSeqNum

            println("RECEIVED ACK " + realSeqNum)

            if(realSeqNum >= Sender.base)
              Sender.base = realSeqNum + 1

            Helpers.writeLog(log, p.getSeqNum)
          }

          case Constants.PACKET_TYPE_EOT => {
            println("RECEIVED EOT")
            Sender.end = true
          }

          case x => println("RECEIVED invalid packet of type " + x)
        }
      }

    } catch {
      case e: Exception => {
        println("Ack receiver aborted: " + e.toString)
        Sender.end = true
      }
    } finally {
      log.close()
    }
  }
}
