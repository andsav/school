import java.net.{InetAddress, DatagramPacket, DatagramSocket}
import java.io.{File, PrintWriter}
import javax.swing.{Timer, AbstractAction}
import java.awt.event.ActionEvent
import scala.io.Source

object Sender extends App {
  require(args.length == 4)
  
  // Helpers
  def getFileContents(fileName: String): String =
    Source.fromFile(fileName).getLines.mkString("\n")

  def buildPackets(content: String, packetSize: Int): Array[packet] = {
    content.grouped(packetSize).toArray.zipWithIndex.map {
      case (chunk, i) => packet.createPacket(i, chunk)
    }
  }

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

  val fileContents = getFileContents(fileName)
  val packetsToSend = buildPackets(fileContents, MAX_READ_SIZE)
  val socket = new DatagramSocket(senderPort)

  var currentMod:Int = -1
  var base:Int = 0
  var nextSeqNum:Int = 0
  var end = false

  // Main program
  val packetSenderThread = new Thread(new PacketSender(seqNumFile, packetsToSend))
  val ackReceiverThread = new Thread(new AckReceiver(ackFile))

  packetSenderThread.start()
  ackReceiverThread.start()
}

class PacketSender(log: PrintWriter, packets: Array[packet]) extends Runnable {
  val timeOut = new AbstractAction() {
    def actionPerformed(e : ActionEvent): Unit = timeout()
  }
  val timer = new Timer(Sender.TIMEOUT, timeOut)

  def run(): Unit = {
    try {
      log.print("")

      println(packets.length + " packets to send")

      while(!Sender.end) {
        println("nextSeqNum: " + Sender.nextSeqNum + ", base: " + Sender.base)

        if(Sender.nextSeqNum >= packets.length-1) {
          sendEOT()
        }
        else if(Sender.nextSeqNum < Sender.base + Sender.WINDOW_SIZE) {
          sendPacket(Sender.nextSeqNum)

          if(Sender.base == Sender.nextSeqNum)
            timer.restart()

          Sender.nextSeqNum += 1
        }
      }

      timer.stop()

    } catch {
      case e: Exception => println("Packet sender aborted: " + e.toString)
      Sender.end = true
    } finally {
      log.close()
    }
  }

  def udtSend(p: packet): Unit = {
    val bytes = p.getUDPdata
    val udpPacket = new DatagramPacket(bytes, bytes.length, Sender.nEmulatorAddress, Sender.nEmulatorPort)
    Sender.socket.send(udpPacket)
  }

  def sendPacket(i: Int): Unit = {
    udtSend(packets(i))
    println("SENT packet " + i)
    log.println(i%32)
  }

  def sendEOT() : Unit = {
    udtSend(packet.createEOT(0))
    println("SENT EOT")
  }

  def timeout(): Unit = {
    timer.start()

    println("TIMEOUT! resending packets " + Sender.base + " to " + (Sender.nextSeqNum-1).toString)

    (Sender.base until Sender.nextSeqNum).foreach(sendPacket)
  }
}

class AckReceiver(log: PrintWriter) extends Runnable {
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

            if(p.getSeqNum%32 == 0)
              Sender.currentMod += 1

            if(p.getSeqNum%32 + Sender.currentMod*32 >= Sender.base)
              Sender.base = p.getSeqNum%32 + Sender.currentMod*32 + 1

            log.println(p.getSeqNum()%32)
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