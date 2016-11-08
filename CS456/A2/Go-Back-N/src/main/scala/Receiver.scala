import java.io.{File, PrintWriter}
import java.net.{InetAddress, DatagramPacket, DatagramSocket}

object Receiver extends App {
  require(args.length == 4)

  // Helpers
  def deliver(p: packet): Unit = {
    file.append(new String(p.getData))
    println("DELIVERED packet " + p.getSeqNum)
  }

  def udtSend(p: packet): Unit = {
    val bytes = p.getUDPdata
    val udpPacket = new DatagramPacket(bytes, bytes.length, nEmulatorAddress, nEmulatorPort)
    socket.send(udpPacket)
  }

  def sendAck(): Unit = {
    udtSend(packet.createACK(expectedSeq))
    println("SENT ACK " + expectedSeq)
    log.println(expectedSeq%32)
  }

  def sendEOT(): Unit = {
    udtSend(packet.createEOT(0))
    println("SENT EOT")
  }

  // Initialization
  val log = new PrintWriter(new File("arrival.log"))

  val nEmulatorAddress = InetAddress.getByName(args(0))
  val nEmulatorPort = Integer.parseInt(args(1))
  val receiverPort = Integer.parseInt(args(2))
  val fileName = args(3)

  val file = new PrintWriter(new File(fileName))
  file.print("")

  val socket = new DatagramSocket(receiverPort)

  var expectedSeq:Int = -1
  var end = false

  // Main program
  try {
    var udpPacket = new DatagramPacket(new Array[Byte](9999), 9999)

    while(!end) {
      socket.receive(udpPacket)
      val p = packet.parseUDPdata(udpPacket.getData())

      p.getType() match {
        case 1 => {
          if(p.getSeqNum == (expectedSeq+1)%32) {
            println("RECEIVED packet " + expectedSeq+1)
            deliver(p)
            expectedSeq += 1
          } else {
            println("RECEIVED out-of-order packet " + expectedSeq+1)
          }

          sendAck()
        }
        case 2 => {
          println("RECEIVED EOT")

          sendEOT()
          end = true
        }
        case x => {
          println("RECEIVED invalid packet of type " + x)
        }
      }
    }
  } catch {
    case e: Exception => {
      println(e.toString)
      throw e
    }
  } finally {
    log.close()
    file.close()
  }
}