object Constants {
  val MAX_READ_SIZE = 500
  val WINDOW_SIZE = 10
  val TIMEOUT_DELAY = 100

  val PACKET_TYPE_ACK = 0
  val PACKET_TYPE_PACKET = 1
  val PACKET_TYPE_EOT = 2

  val SEQ_NUM_LOG = "seqnum.log"
  val ACK_LOG = "ack.log"
  val ARRIVAL_LOG = "arrival.log"
}