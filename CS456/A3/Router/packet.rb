module Packet
  def to_s
    ret = "#{self.class.name} ("
    self.each_pair{|k,v| ret += "#{k}: #{v}, "}
    ret.chomp(', ') + ')'
  end

  def udp_send
    UDP_Socket.send self.to_a.pack('L*'), 0, NSE_HOSTNAME, NSE_PORT
    Log.write('sends', self)
  end

  module_function

  def init
    PACKET_TYPES.each do |p|
      p.include self
    end
  end

  def make(data)
    PACKET_TYPES.each do |p|
      return p.new(*data) if p.new.size == data.size
    end

    raise RuntimeError "Attempting to create packet of size #{data.size}"
  end
end

class PacketInit < Struct.new(:router_id)
end

class PacketHello < Struct.new(:router_id, :link_id)
  def deliver

  end
end

class PacketLSPDU < Struct.new(:sender, :router_id, :link_id, :cost, :via)
  def deliver

  end
end

PACKET_TYPES = [PacketInit, PacketHello, PacketLSPDU]
