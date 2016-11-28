require 'socket'

#
# Parsing arguments
#
raise RuntimeError, 'Expects arguments: <router_id>, <nse_hostname>, <nse_port>, <router_port>' if ARGV.length != 4

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

# 1 - Send INIT
udp_send(socket, PacketInit.new(ROUTER_ID), log_file)

# 2 - Send HELLO to neighbours


# 3 -