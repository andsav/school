require 'socket'

# Validate parameters
raise RuntimeError, 'No <req_code> specified' if ARGV.length < 1
raise RuntimeError, 'Parameter <req_code> must be a number' if not ARGV[0] =~ /^[0-9]+$/

# Start TCP server at a free port
server = TCPServer.open(0)

# Constants
N_PORT = server.addr[1]
REQ_CODE = ARGV[0].strip
HOSTNAME = Socket.gethostname.split('.')[0]

puts "SERVER_PORT=#{N_PORT}"
puts "REQ_CODE=#{REQ_CODE}\n\n"

# Abort threads if exception is caught
Thread.abort_on_exception = true

# Server loop
loop do
  Thread.start(server.accept) do |client|
    puts "Client #{client} - TCP connection opened"

    # Get the <req_code> supplied by client and verify it is valid
    req_code = client.gets.strip

    if req_code != REQ_CODE
      puts "Client #{client} - Invalid <req_code>: #{req_code}"
      puts "Client #{client} - Terminated\n\n"
      client.close
    else
      puts "Client #{client} - <req_code> accepted!"

      # Open an UDP socket and bind to a free port
      udp_server = UDPSocket.new
      udp_server.bind '', 0
      udp_port = udp_server.addr[1]

      puts "Server - Listening on UDP port #{udp_port}"

      # Send the UDP port to client
      client.puts udp_port

      # Receive message and callback instructions from client
      message = udp_server.recv(1024)
      client_udp_port = udp_server.recv(10)
      client_hostname = udp_server.recv(512)
      client_hostname = '127.0.0.1' if client_hostname == HOSTNAME
      puts "Client #{client} - Received <message>: #{message}"
      puts "Client #{client} - Received callback port: #{client_udp_port}"
      puts "Client #{client} - Received callback hostname: #{client_hostname}"

      # Reply back to client
      udp_server.send message.reverse, 0, client_hostname, client_udp_port
      puts 'Server - Replied with reversed message'

      # Close UDP socket and TCP connection with client
      udp_server.close
      puts 'Server - Closed UDP connection'
      client.close
      puts "Client #{client} - Terminated\n\n"

    end

  end
end