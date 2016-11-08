import java.io.{File, PrintWriter}
import java.net.{InetAddress, DatagramPacket, DatagramSocket}

object Receiver extends App {
  require(args.length == 4)

  // Helpers
  def deliver(p: packet): Unit = {
    println("DELIVERED packet " + p.getSeqNum)
    file.append(p.getData.toString)
  }

  def sendAck(i: Int): Unit = {
    val bytes = packet.createACK(i).getUDPdata
    val udpPacket = new DatagramPacket(bytes, bytes.length, nEmulatorAddress, ackPort)

    socket.send(udpPacket)

    println("SENT ACK " + i)
    log.println(i)
  }

  // Initialization
  val log = new PrintWriter(new File("arrival.log"))

  val nEmulatorAddress = InetAddress.getByName(args(0))
  val ackPort = Integer.parseInt(args(1))
  val receiverPort = Integer.parseInt(args(2))
  val fileName = args(3)

  val file = new PrintWriter(new File(fileName))
  file.print("")

  val socket = new DatagramSocket(receiverPort)

  var expectedSeq = -1
  var end = false

  var udpPacket = new DatagramPacket(new Array[Byte](Sender.MAX_READ_SIZE), Sender.MAX_READ_SIZE)

  // Main program
  try {
    while(!end) {
      socket.receive(udpPacket)
      val p = packet.parseUDPdata(udpPacket.getData())

      if(p.getSeqNum == expectedSeq) {
        println("RECEIVED packet " + p.getSeqNum)
        deliver(p)
        expectedSeq += 1
      } else {
        println("RECEIVED out-of-order packet " + p.getSeqNum)
      }

      sendAck(expectedSeq)
    }
  } catch {
    case e: Exception => println(e.toString)
  } finally {
    log.close()
    file.close()
  }
}