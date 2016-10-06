require 'socket'

raise RuntimeError, 'No <req_code> specified' if ARGV.length < 1

server = TCPServer.open(0)

N_PORT = server.addr[1]
REQ_CODE = ARGV[0].strip

puts "SERVER_PORT=#{N_PORT}"
puts "REQ_CODE=#{REQ_CODE}"

loop do
  Thread.start(server.accept) do |client|
    puts "#{client} - connection opened"

    req_code = client.gets.strip

    if req_code != REQ_CODE
      puts "#{client} - Invalid <req_code>: #{req_code}"
      puts "#{client} - connection closed"
      client.close
    else
      puts "#{client} - <req_code> accepted!"
    end

    message = client.gets.strip

    puts "Client message: #{message}"

    client.close
  end
end