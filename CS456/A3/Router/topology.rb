module Topology
  module_function

  def init(circuit_db)
    @link_state = LinkStateDB.new(circuit_db)
    @rib = RIB.new
  end

  def update(lspdu)

    false
  end

  def rib
    @rib
  end

  def link_state
    @link_state
  end

  def flood(link_id)
    @link_state.each do |router, state|
      state.each do |link_cost|
        link, cost = link_cost
        PacketLSPDU.new(ROUTER_ID, router, link, cost, link_id).udp_send unless link == link_id
      end
    end
  end

  def flood_neighbours
    @link_state[ROUTER_ID].each do |x|
      flood(x['link'])
    end
  end
end

#
# LinkStateDB: { router => [(link, cost)] }
#
class LinkStateDB < Hash

  class LinkCost < Struct.new(:link, :cost)
    def to_s
      "link #{self[0]} cost #{self[1]}"
    end
  end

  def initialize(circuit_db)
    self[ROUTER_ID] = Array.new unless self.has_key?(ROUTER_ID)

    circuit_db.each do |link, cost|
      self[ROUTER_ID].push LinkCost.new(link, cost)
    end

    Log.write(self.group(ROUTER_ID))
  end

  def group(router)
    ret = "R#{ROUTER_ID} -> R#{router} nbr link #{self[router].size}\n"
    self[router].each do |link_cost|
      ret += "R#{ROUTER_ID} -> R#{router} #{link_cost}\n"
    end
    ret
  end

  def to_s
    ret = "# Topology Database\n"
    self.keys.sort do |router|
      ret += self.group(router)
    end
    ret
  end
end

#
# RIB: { dest => (path, cost) }
#
class RIB < Hash

  class PathCost < Struct.new(:path, :cost)
    def to_s
      case self[0]
        when ROUTER_ID
          'Local, 0'
        else
          "R#{self[0]}, #{self[1]}"
      end
    end
  end

  def initialize
    self[ROUTER_ID] = PathCost.new(ROUTER_ID, 0)
  end

  def to_s
    ret = "# RIB\n"
    self.sort.map do |dest, path_cost|
      ret += "R#{ROUTER_ID} -> R#{dest} -> #{path_cost}\n"
    end
    ret
  end

end