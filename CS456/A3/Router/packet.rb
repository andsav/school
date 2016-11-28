class PacketInit < Struct.new(:router_id); end

class PacketHello < Struct.new(:router_id, :link_id); end

class PacketLSPDU < Struct.new(:sender, :router_id, :link_id, :cost, :via); end

def udp_send(socket, packet, log_file)
  socket.send packet.to_a.pack('C*'), 0, NSE_HOSTNAME, NSE_PORT

  log_msg = "R#{ROUTER_ID} sends #{packet.class.name} ("
  packet.each_pair{|k,v| log_msg += "#{k}: #{v}, "}
  log_msg = log_msg.chomp(', ') + ')'

  log_file.write("#{log_msg}\n")
end