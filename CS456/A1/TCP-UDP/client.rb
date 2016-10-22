require 'socket'

# Validate parameters
raise RuntimeError, 'Invalid argument count' if ARGV.length != 4
raise RuntimeError, 'Parameter <n_port> must be a number' if not ARGV[1] =~ /^[0-9]+$/
raise RuntimeError, 'Parameter <req_code> must be a number' if not ARGV[2] =~ /^[0-9]+$/

# Constants
SERVER_ADDRESS, N_PORT, REQ_CODE, MESSAGE = ARGV
HOSTNAME = Socket.gethostname.split('.')[0]

# Open an UDP socket and bind to a free port (for server replies)
udp_server = UDPSocket.new
udp_server.bind '', 0
UDP_PORT = udp_server.addr[1]

# Stage 1: Negotiation on TCP channel
server = TCPSocket.open(SERVER_ADDRESS, N_PORT)
puts "Server #{server} - TCP connection opened"

server.puts REQ_CODE

server_ret = server.gets
if server_ret.nil?
  server.close
  raise RuntimeError, 'Invalid <req_code>; Server closed connection'
end

SERVER_UDP_PORT = server_ret.strip
puts "Server #{server} - Returned UDP port: #{SERVER_UDP_PORT}"
puts "Server #{server} - Closing TCP connection\n\n"
server.close

# Stage 2: UDP sockets communication
udp_server.send MESSAGE, 0, SERVER_ADDRESS, SERVER_UDP_PORT
udp_server.send UDP_PORT.to_s, 0, SERVER_ADDRESS, SERVER_UDP_PORT
udp_server.send HOSTNAME, 0, SERVER_ADDRESS, SERVER_UDP_PORT

# Returned reverse message
puts udp_server.recv(1024)