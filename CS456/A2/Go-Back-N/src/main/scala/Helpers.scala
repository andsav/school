import java.io.{File, PrintWriter}
import java.net.{InetAddress, DatagramPacket, DatagramSocket}
import scala.io.Source

object Helpers {
  def parseArgs(args: Array[String]): (InetAddress, Int, Int, String) = (
    InetAddress.getByName(args(0)),
    Integer.parseInt(args(1)),
    Integer.parseInt(args(2)),
    args(3)
  )

  def udpPacket(bytes: Array[Byte], dest: InetAddress, port: Int): DatagramPacket =
    new DatagramPacket(bytes, bytes.length, dest, port)

  def udtSend(p: packet, socket: DatagramSocket, dest: InetAddress, port: Int): Unit =
    socket.send(udpPacket(p.getUDPdata, dest, port))

  def emptyUdpPacket(): DatagramPacket =
    new DatagramPacket(new Array[Byte](9999), 9999)

  def getFileContents(fileName: String): String =
    Source.fromFile(fileName, "utf-8").getLines.mkString("\n")

  def buildPackets(content: String, packetSize: Int): Array[packet] = {
    // The end of packets is sometimes trimmed (?!), so we pad them with characters to be removed later
    content.grouped(packetSize-32).toArray.zipWithIndex.map {
      case (chunk, i) => packet.createPacket(i, chunk + "#" * 32)
    }
  }

  def writeLog(log: PrintWriter, n: Int): Unit = {
    try log.println((n%32))
    catch {
      case e: Exception => {
        println("writeLog() aborted: " + e.toString)
        throw e
      }
    }
  }

  def openFile(fileName: String): PrintWriter = {
    try {
      val f = new PrintWriter(new File(fileName))
      f.print("") // Erase previous content
      f
    }
    catch {
      case e: Exception => {
        println("openFile(" + fileName  +") aborted: " + e.toString)
        throw e
      }
    }
  }
}
