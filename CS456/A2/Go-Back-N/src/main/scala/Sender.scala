import java.net.{InetAddress, DatagramPacket, DatagramSocket}
import java.io.{File, PrintWriter}
import java.util.concurrent.{TimeUnit, TimeoutException, Future}
import javax.swing.{Timer, AbstractAction}
import java.awt.event.ActionEvent
import scala.io.Source

object Sender extends App {
  require(args.length == 4)

  // Constants
  val MAX_READ_SIZE = 500
  val WINDOW_SIZE = 10
  val TIMEOUT = 100

  // Initialization
  val seqNumFile = new PrintWriter(new File("seqnum.log"))
  val ackFile = new PrintWriter(new File("ack.log"))

  val nEmulatorAddress = InetAddress.getByName(args(0))
  val nEmulatorPort = Integer.parseInt(args(1))
  val senderPort = Integer.parseInt(args(2))
  val fileName = args(3)

  val fileContents = Helper.getFileContents(fileName)
  val packetsToSend = Helper.buildPackets(fileContents, MAX_READ_SIZE)
  val socket = new DatagramSocket(senderPort)

  var base = 0
  var nextSeqNum = 0
  var end = false

  // Main program
  new Thread(new PacketSender(seqNumFile, packetsToSend)).start()
  new Thread(new AckReceiver(ackFile)).start()
}

class PacketSender(log: PrintWriter, packets: Array[packet]) extends Runnable {
  val timeOut = new AbstractAction() {
    def actionPerformed(e : ActionEvent): Unit = timeout()
  }
  val timer = new Timer(Sender.TIMEOUT, timeOut)

  def run(): Unit = {
    try {
      log.print("")

      while(!Sender.end) {
        if(Sender.nextSeqNum < Sender.base + Sender.WINDOW_SIZE) {
          udtSend(Sender.nextSeqNum)

          if(Sender.base == Sender.nextSeqNum)
            timer.restart()

          Sender.nextSeqNum += 1
        }
      }
    } catch {
      case e: Exception => println("Packet sender aborted: " + e.toString)
      Sender.end = true
    } finally {
      log.close()
    }
  }

  def udtSend(i: Int): Unit = {
    val bytes = packets(i).getUDPdata
    val packet = new DatagramPacket(bytes, bytes.length, Sender.nEmulatorAddress, Sender.nEmulatorPort)

    Sender.socket.send(packet)

    println("SENT packet " + i)
    log.println(packets(i).getSeqNum)
  }

  def timeout(): Unit = {
    timer.start()

    println("TIMEOUT! resending packets " + Sender.base + " to " + (Sender.nextSeqNum-1).toString)

    (Sender.base until Sender.nextSeqNum).foreach(udtSend)
  }
}

class AckReceiver(log: PrintWriter) extends Runnable {
  var end = false

  def run(): Unit = {
    try {
      log.print("")

      var udpPacket = new DatagramPacket(new Array[Byte](Sender.MAX_READ_SIZE), Sender.MAX_READ_SIZE)

      while(!Sender.end) {
        Sender.socket.receive(udpPacket)
        val p = packet.parseUDPdata(udpPacket.getData())

        p.getType() match {
          case 0 => {
            println("RECEIVED ACK " + p.getSeqNum())

            if(p.getSeqNum() >= Sender.base)
              Sender.base = p.getSeqNum() + 1

            log.println(p.getSeqNum())
          }
          case 2 => {
            println("RECEIVED EOT")

            Sender.end = true
          }
          case x => {
            println("RECEIVED invalid packet of type " + x)
          }
        }
      }

    } catch {
      case e: Exception => println("Ack receiver aborted: " + e.toString)
      Sender.end = true
    } finally {
      log.close()
    }
  }
}

object Helper {
  def getFileContents(fileName: String): String =
    Source.fromFile(fileName).getLines.mkString

  def buildPackets(content: String, packetSize: Int): Array[packet] = {
    content.grouped(packetSize).toArray.zipWithIndex.map {
      case (chunk, i) => packet.createPacket(i, chunk)
    }
  }
}