require 'enumerator'

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
    input.each { |i| print "#{i.to_s(2).rjust(4, '0')} " }; print "\n"
  end

  def print_keys
    @subkeys.each { |sk| print_bin(sk) }
  end

  def hex_to_bin_map(input)
    input.collect { |x| x.to_s(2).rjust(4, '0').split('') }.join.split('')
  end

  def bin_map_to_hex(input)
    ret = []
    input.join.split('').each_slice(4) { |x| ret.push x.join.to_i(2) }
    ret
  end

  def substitute_1(input)
    [0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7][input]
  end

  def permute_1(input)
    [0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15][input]
  end

  def subkey_xor(input, round)
    input.map.with_index { |x, i| x ^ @subkeys[round-1][i] }
  end

  def substitute(input)
    input.collect { |x| substitute_1(x) }
  end

  def permute(input)
    out_array = Array.new(16)
    in_array = hex_to_bin_map(input)

    in_array.each_with_index { |x, i| out_array[ permute_1(i) ] = x }

    bin_map_to_hex(out_array)
  end

  def round(input, round)
    # XOR subkey
    u1 = subkey_xor(input, round)
    print "U#{round}: "
    print_bin(u1)

    # Substitute
    v1 = substitute(u1)
    print "V#{round}: "
    print_bin(v1)

    v1
  end

  def encrypt(plaintext = [])
    if plaintext.length != 4
      plaintext = []
      puts 'Enter Plaintext (4 hex digits)'

      plaintext = gets.split('')
      plaintext.pop
      plaintext = plaintext.collect { |x| x.hex }

    end

    print ' P: '
    print_bin(plaintext)

    # 4 Rounds
    input = round(plaintext, 1)
    round = 2
    3.times do
      input = round(permute(input), round)
      round += 1
    end

    # Key Whitening
    c = subkey_xor(input, 5)
    print ' C: '
    print_bin(c)

  end

end

cipher = Heys.new
cipher.encrypt