require 'socket'

raise RuntimeError, 'No <req_code> specified' if ARGV.length < 1

server = TCPServer.open(0)
udp_server = UDPSocket.new

udp_server.bind '', 0

UDP_PORT = udp_server.addr[1]
N_PORT = server.addr[1]
REQ_CODE = ARGV[0].strip
HOSTNAME = Socket.gethostname.split('.')[0]

puts "SERVER_PORT=#{N_PORT}"
puts "REQ_CODE=#{REQ_CODE}"
puts "UDP_PORT=#{UDP_PORT}\n\n"

Thread.abort_on_exception = true

loop do
  Thread.start(server.accept) do |client|
    puts "Client #{client} - TCP connection opened"

    req_code = client.gets.strip

    if req_code != REQ_CODE
      puts "Client #{client} - Invalid <req_code>: #{req_code}"
      puts "Client #{client} - Terminated\n\n"
      client.close
    else
      puts "Client #{client} - <req_code> accepted!"
      client.puts UDP_PORT

      message = udp_server.recv(1024)
      client_udp_port = udp_server.recv(10)
      client_hostname = udp_server.recv(512)
      client_hostname = '127.0.0.1' if client_hostname == HOSTNAME

      puts "Client #{client} - Received <message>: #{message}"
      puts "Client #{client} - Received callback port: #{client_udp_port}"
      puts "Client #{client} - Received callback hostname: #{client_hostname}"

      udp_server.send message.reverse, 0, client_hostname, client_udp_port

      puts "Client #{client} - Returned reversed message"
      puts "Client #{client} - Terminated\n\n"

      client.close
    end

  end
end