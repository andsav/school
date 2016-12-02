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
    decoded = random_bytes(BLOCK_SIZE)
    i = 0

    (0..0xff).to_a.shuffle.each do |x|
      decoded[-1] = x
      break if test_helper(decoded, block)
    end

    (1..LAST_BYTE).to_a.reverse.each do |n|
      r = decoded
      r[LAST_BYTE-n+1] ^= 1

      return decoded[LAST_BYTE-n+1..LAST_BYTE].collect { |x| x^n } unless test_helper(r, block)
    end

    [ decoded[-1] ^ 1 ]
  end

  def decrypt_block(block)
    decoded = last_word(block)

    debug "Decrypted: #{decoded}"

    until decoded.length == BLOCK_SIZE
      j = BLOCK_SIZE - decoded.length
      r = random_bytes(j).concat(decoded.collect { |x| x^(LAST_BYTE-j+2) })

      (0..0xff).to_a.shuffle.each do |x|
        r[j-1] = x
        break if test_helper(r, block)
      end

      decoded.unshift r[j-1]^(LAST_BYTE-j+2)
    end

    decoded.pack('C*')
  end

  def decrypt(blocks)
    blocks[0] = xor(IV, decrypt_block(blocks[0]))
    (1..blocks.size-1).each do |b|
      blocks[b] = xor(blocks[b-1], decrypt_block(blocks[b]))
    end
    blocks
  end
end