require 'socket'

#
# Parsing arguments
#
raise RuntimeError, 'Expects arguments: <router_id>, <nse_hostname>, <nse_port>, <router_port>' unless ARGV.length == 4

ROUTER_ID = ARGV[0].to_i
NSE_HOSTNAME = ARGV[1]
NSE_PORT = ARGV[2].to_i
ROUTER_PORT = ARGV[3].to_i

raise RuntimeError, 'Invalid router_id' if ROUTER_ID == 0
raise RuntimeError, 'Invalid nse_port' if NSE_PORT == 0 or NSE_PORT > 0xffff
raise RuntimeError, 'Invalid router_port' if ROUTER_PORT == 0 or ROUTER_PORT > 0xffff

#
# Initialization
#
socket = UDPSocket.new
socket.bind '', ROUTER_PORT

log_file = open("router#{ROUTER_ID}.log", 'w')

#
# Main program
#
load 'packet.rb'
load 'circuit.rb'
load 'graph.rb'

# 1 - Send INIT
PacketInit.new(ROUTER_ID).udp_send(socket,log_file)


# 2 - Receive circuit DB from nse
rcd = socket.recv(64).unpack('L*')
circuit_db = CircuitDB.new

(0..rcd[0]-1).each do |i|
  circuit_db[rcd[1+(i*2)]] = rcd[2+(i*2)]
end

log_file.write("\nR#{ROUTER_ID} receives circuit_db #{circuit_db}\n\n")


# 3 - Send HELLO to all neighbours
circuit_db.keys.each do |link|
  PacketHello.new(ROUTER_ID, link).udp_send(socket, log_file)
end
log_file.write("\n")

# 4 - Listen for HELLO and LSPDU
loop do
  packet = make_pkt(socket.recv(64).unpack('L*'))
  log_file.write("R#{ROUTER_ID} receives #{packet}\n")
  packet.deliver
end
