require 'socket'

raise RuntimeError, 'Invalid argument count' if ARGV.length != 4

SERVER_ADDRESS, N_PORT, REQ_CODE, MESSAGE = ARGV

server = TCPSocket.open(SERVER_ADDRESS, N_PORT)

server.puts REQ_CODE

server.puts MESSAGE

server.close