class PacketInit < Struct.new(:router_id); end

class PacketHello < Struct.new(:router_id, :link_id); end

class PacketLSPDU < Struct.new(:sender, :router_id, :link_id, :cost, :via); end

def udp_send(socket, packet)
  socket.send packet.to_a.pack('C*'), 0, NSE_HOSTNAME, NSE_PORT
end