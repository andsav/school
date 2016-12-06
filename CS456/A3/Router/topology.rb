module Topology
  module_function

  def init(circuit_db)
    @link_state = LinkStateDB.new(circuit_db)
  end

  def update(lspdu)
    if @link_state.has_key? lspdu['router_id']
      return false if @link_state[lspdu['router_id']].has_key?(lspdu['link_id'])
    end

    @link_state.update(lspdu['router_id'], lspdu['link_id'], lspdu['cost'])

    Log.write(@link_state)

    Log.write(@link_state.rib)

    true
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

  def edges
    ret = {}
    self.each do |v1, v2_cost|
      v2_cost.each do |v2, cost|
        ret[[v1, v2]] = ret[[v2, v1]] = cost
      end
    end
    ret
  end

  def dijkstra(target)
    vertices = []
    neighbours = {}
    dist = {}
    prev = {}

    self.each do |v1, v2_cost|
      vertices << v1
      v2_cost.each do |v2, cost|
        vertices << v2
        neighbours[v1] = [] unless neighbours.has_key? v1
        neighbours[v2] = [] unless neighbours.has_key? v2

        neighbours[v1] << [ v2, cost ]
        neighbours[v2] << [ v1, cost ]
      end
    end

    vertices.uniq!

    vertices.each do |v|
      prev[v] = nil
      dist[v] = Float::INFINITY
    end

    dist[ROUTER_ID] = 0
    q = vertices

    until q.empty?
      min = Float::INFINITY

      u = q[0]

      q.each do |v|
        if dist[v] < min
          min = dist[v]
          u = v
        end
      end

      q.delete(u)

      break if dist[u] == Float::INFINITY || u == target

      if neighbours.has_key?(u)
        neighbours[u].each do |x|
          alt = dist[u] + x[1]

          if alt < dist[x[0]]
            dist[x[0]] = alt
            prev[x[0]] = u
          end
        end
      end

    end

    path = []

    while prev.has_key?(u)
      path.unshift u
      u = prev[u]
    end

    [path, dist[target]]

  end

  def rib
    ret = "\n# RIB\n"

    self.keys.sort.each do |dest|
      path, cost = dijkstra(dest)

      next unless path[0] == ROUTER_ID
      via = path.length == 1 ? '(Local)' : "R#{path[1]}"

      ret += "R#{ROUTER_ID} -> R#{dest} -> #{via}, #{cost}\n"
    end
    ret + "\n"
  end

  def to_s
    "\n# Topology Database\n" + self.keys.sort.collect { |k| self[k] }.join("\n") + "\n"
  end
end

