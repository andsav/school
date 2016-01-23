def substitute(input)
  [0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7][input]
end

def permute(input)
  [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16][input-1]
end

class Heys
  def initialize(subkeys = [])

    @subkeys = subkeys

    if subkeys.length != 5
      until subkeys.length == 5 do
        puts "Enter Subkey ##{@subkeys.length+1} (4 hex digits)"

        sk = gets.split('')
        sk.pop
        sk = sk.collect { |x| x.hex }

        @subkeys.push(sk) if sk.length == 4
      end
    end

  end

  def print_bin(input)
    input.each { |i| print "#{i.to_s(2)} " }; print "\n"
  end

  def print_keys
    @subkeys.each { |sk| print_bin(sk) }
  end

  def encrypt(plaintext = [])
    if plaintext.length != 4
      plaintext = []
      puts 'Enter Plaintext (4 hex digits)'

      plaintext = gets.split('')
      plaintext.pop
      plaintext = plaintext.collect { |x| x.hex }

    end

    print 'P: '
    print_bin(plaintext)

    u1 = plaintext.map.with_index { |x, i| x ^ @subkeys[0][i] }
    print 'U1: '
    print_bin(u1)
    

  end

end

cipher = Heys.new
cipher.encrypt