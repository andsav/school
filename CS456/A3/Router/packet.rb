module Packet
  def to_s
    ret = "#{self.class.name} ("
    self.each_pair{|k,v| ret += "#{k}: #{v}, "}
    ret.chomp(', ') + ')'
  end

  def udp_send
    UDP_Socket.send self.to_a.pack('L*'), 0, NSE_HOSTNAME, NSE_PORT
    Log.write_action('sends', self)
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

  def receive
    packet = self.make(UDP_Socket.recv(64).unpack('L*'))
    Log.write_action('receives', packet)
    packet
  end
end

class PacketInit < Struct.new(:router_id)
end

class PacketHello < Struct.new(:router_id, :link_id)

  # Reply to HELLO with LSDPDU's
  def deliver
    Topology.flood(self['link_id'])
  end

end

class PacketLSPDU < Struct.new(:sender, :router_id, :link_id, :cost, :via)

  # Update topology and send updates to neighbours if there were changes
  def deliver
    Topology.update(self) && Topology.flood_neighbours
  end

end

PACKET_TYPES = [PacketInit, PacketHello, PacketLSPDU]
