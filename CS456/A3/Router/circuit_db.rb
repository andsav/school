class CircuitDB < Hash

  def initialize
    rcd = UDP_Socket.recv(64).unpack('L*')

    (0..rcd[0]-1).each do |i|
      self[rcd[1+(i*2)]] = rcd[2+(i*2)]
    end

    Log.write_action('receives', self)
  end


  # Override the HashMap pretty print (for logging)
  def to_s
    'circuit_db ' + super.sub('{', '(L').sub('}', ')').gsub('=>', ': ').gsub('"', '').gsub(', ', ', L')
  end

end