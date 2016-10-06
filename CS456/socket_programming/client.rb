require 'socket'

raise RuntimeError, 'Invalid argument count' if ARGV.length != 4

SERVER_ADDRESS, N_PORT, REQ_CODE, MESSAGE = ARGV

server = TCPSocket.open(SERVER_ADDRESS, N_PORT)
udp_server = UDPSocket.new
udp_server.bind '127.0.0.1', 0

UDP_PORT = udp_server.addr[1]

puts "Server #{server} - connection opened"

server.puts REQ_CODE

SERVER_UDP_PORT = server.gets.strip

puts "Server #{server} - returned UDP port: #{SERVER_UDP_PORT}"
puts "Server #{server} - closing connection"

server.close

udp_server.send MESSAGE, 0, '127.0.0.1', SERVER_UDP_PORT
udp_server.send UDP_PORT.to_s, 0, '127.0.0.1', SERVER_UDP_PORT

puts udp_server.recv(1024)