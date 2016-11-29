module Packet
  def to_s
    ret = "#{self.class.name} ("
    self.each_pair{|k,v| ret += "#{k}: #{v}, "}
    ret.chomp(', ') + ')'
  end

  def udp_send(socket, log_file)
    socket.send self.to_a.pack('L*'), 0, NSE_HOSTNAME, NSE_PORT
    log_msg = "R#{ROUTER_ID} sends #{self}"
    log_file.write("#{log_msg}\n")
  end
end

class PacketInit < Struct.new(:router_id)
  include Packet
end

class PacketHello < Struct.new(:router_id, :link_id)
  include Packet

  def deliver

  end
end

class PacketLSPDU < Struct.new(:sender, :router_id, :link_id, :cost, :via)
  include Packet

  def deliver

  end
end

def make_pkt(data)
  [PacketInit, PacketHello, PacketLSPDU].each do |p|
    if p.new.size == data.size
      return p.new(*data)
    end
  end

  raise RuntimeError "Attempting to create packet of size #{data.size}"
end