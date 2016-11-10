import java.io.{File, PrintWriter}
import java.net.{InetAddress, DatagramPacket, DatagramSocket}

object Receiver extends App {
  require(args.length == 4)

  // Initialization
  val (nEmulatorAddress, nEmulatorPort, receiverPort, fileName) = Helpers.parseArgs(args)

  val file = Helpers.openFile(fileName)
  val log = Helpers.openFile(Constants.ARRIVAL_LOG)

  val socket = new DatagramSocket(receiverPort)

  var expectedSeq = -1
  var end = false

  // Main program
  try {
    var udpPacket = Helpers.emptyUdpPacket()

    while(!end) {
      socket.receive(udpPacket)
      val p = packet.parseUDPdata(udpPacket.getData())

      p.getType() match {
        case Constants.PACKET_TYPE_PACKET => {

          if(p.getSeqNum == (expectedSeq+1)%32) {

            println("RECEIVED packet " + (expectedSeq+1)%32)
            deliver(p)
            expectedSeq += 1
          }
          else println("RECEIVED out-of-order packet " + (expectedSeq+1)%32)

          sendAck()
        }

        case Constants.PACKET_TYPE_EOT  => {
          println("RECEIVED EOT")

          sendEOT()
          end = true
        }

        case x => println("RECEIVED invalid packet of type " + x)
      }
    }
  } catch {
    case e: Exception => println("Receiver aborted: " + e.toString)
  } finally {
    log.close()
    file.close()
  }

  // Helpers
  def deliver(p: packet): Unit = {
    file.append(new String(p.getData))
    println("DELIVERED packet " + p.getSeqNum)
  }

  def udtSend(p: packet): Unit = {
    Helpers.udtSend(p, socket, nEmulatorAddress, nEmulatorPort)
  }

  def sendAck(): Unit = {
    udtSend(packet.createACK(expectedSeq))
    println("SENT ACK " + expectedSeq)
    Helpers.writeLog(log, expectedSeq)
  }

  def sendEOT(): Unit = {
    udtSend(packet.createEOT(0))
    println("SENT EOT")
  }
}
