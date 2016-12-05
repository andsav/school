module Topology
  module_function

  def init(circuit_db)
    @link_state = LinkStateDB.new(circuit_db)
    @rib = RIB.new
  end

  def update(lspdu)
    if @link_state.has_key? lspdu['router_id']
      return false if @link_state[lspdu['router_id']].has_key?(lspdu['link_id'])
    end

    @link_state.update(lspdu['router_id'], lspdu['link_id'], lspdu['cost'])

    Log.write(@link_state)


    Log.write(@rib)

    true
  end

  def rib
    @rib
  end

  def link_state
    @link_state
  end

  def flood(link_id)
    @link_state.each do |router, state|
      state.each do |link, cost|
        PacketLSPDU.new(ROUTER_ID, router, link, cost, link_id).udp_send unless link == link_id
      end
    end
  end

  def flood_neighbours
    @link_state[ROUTER_ID].keys.each { |link| flood(link) }
  end
end

#
# LinkStateDB: { router => [(link, cost)] }
#
class LinkStateDB < Hash

  class LinkCosts < Hash
    def initialize(router)
      @router = router
    end

    def to_s
      "R#{ROUTER_ID} -> R#{@router} nbr link #{self.size}\n" + self.keys.sort.collect do |link|
        "R#{ROUTER_ID} -> R#{@router} link #{link} cost #{self[link]}"
      end.join("\n")
    end
  end

  def initialize(circuit_db)
    self[ROUTER_ID] = LinkCosts.new(ROUTER_ID) unless self.has_key?(ROUTER_ID)

    circuit_db.each do |link, cost|
      self[ROUTER_ID][link] = cost
    end

    Log.write(self[ROUTER_ID])
  end

  def update(router, link, cost)
    self[router] = LinkCosts.new(router) unless self.has_key?(router)
    self[router][link] = cost

    self.keys.each do |r|
      self[r][link] = cost if self[r].has_key?(link)
    end
  end

  def to_s
    "\n# Topology Database updated!\n" + self.keys.sort.collect { |k| self[k] }.join("\n") + "\n"
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
    ret = "\n# RIB updated!\n"
    self.sort.map do |dest, path_cost|
      ret += "R#{ROUTER_ID} -> R#{dest} -> #{path_cost}\n"
    end
    ret + "\n"
  end

end