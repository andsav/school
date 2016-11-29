require 'socket'

module UDP_Socket
  module_function

  def init
    @socket = UDPSocket.new
    @socket.bind '', ROUTER_PORT
  end

  def send(*args)
    @socket.send(*args)
  end

  def recv(*args)
    @socket.recv(*args)
  end
end