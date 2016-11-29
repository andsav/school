load 'log.rb'
load 'udp_socket.rb'
load 'packet.rb'
load 'circuit_db.rb'
load 'topology.rb'

#                   #
# Parsing arguments #
#                   #

raise RuntimeError, 'Expects arguments: <router_id>, <nse_hostname>, <nse_port>, <router_port>' unless ARGV.length == 4

ROUTER_ID = ARGV[0].to_i
NSE_HOSTNAME = ARGV[1]
NSE_PORT = ARGV[2].to_i
ROUTER_PORT = ARGV[3].to_i

raise RuntimeError, 'Invalid router_id' if ROUTER_ID == 0
raise RuntimeError, 'Invalid nse_port' if NSE_PORT == 0 or NSE_PORT > 0xffff
raise RuntimeError, 'Invalid router_port' if ROUTER_PORT == 0 or ROUTER_PORT > 0xffff

#                #
# Initialization #
#                #

Log.init
UDP_Socket.init
Packet.init

#              #
# Main program #
#              #

# 1 - Send INIT
PacketInit.new(ROUTER_ID).udp_send

# 2 - Receive circuit DB from nse
Log.section
circuit_db = CircuitDB.new
Topology.init(circuit_db)

# 3 - Send HELLO to neighbours
Log.section
circuit_db.keys.each do |link|
  PacketHello.new(ROUTER_ID, link).udp_send
end

# 4 - Listen for HELLO and LSPDU
Log.section
loop do
  Packet.receive.deliver
end
