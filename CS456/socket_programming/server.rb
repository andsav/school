require 'socket'

raise RuntimeError, 'No <req_code> specified' if ARGV.length < 1

server = TCPServer.open(0)
udp_server = UDPSocket.new

udp_server.bind '127.0.0.1', 0

UDP_PORT = udp_server.addr[1]
N_PORT = server.addr[1]
REQ_CODE = ARGV[0].strip

puts "SERVER_PORT=#{N_PORT}"
puts "REQ_CODE=#{REQ_CODE}"

Thread.abort_on_exception = true

loop do
  Thread.start(server.accept) do |client|
    puts "Client #{client} - connection opened"

    req_code = client.gets.strip

    if req_code != REQ_CODE
      puts "#{client} - Invalid <req_code>: #{req_code}"
      puts "#{client} - connection closed"
      client.close
    else
      puts "Client #{client} - <req_code> accepted!"
    end

    puts "Listening for UDP messages at #{UDP_PORT}"
    client.puts UDP_PORT

    message = udp_server.recv(1024)
    client_udp_port = udp_server.recv(10)

    puts "Received <message>: #{message}"
    puts "Received client callback port: #{client_udp_port}"

    udp_server.send message.reverse, 0, '127.0.0.1', client_udp_port

    #udp_server.close
    client.close
  end
end