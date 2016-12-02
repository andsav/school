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

  def xor(a, b)
    [a.unpack('C*'), b.unpack('C*')].transpose.map { |x| x.reduce(:^) }.pack('C*')
  end

  def test_helper(byte_array, block)
    test_cookie(byte_array.pack('C*') + block)
  end

  def last_word(block)
    decoded = Array.new(LAST_BYTE+1) { rand(0xff) }

    (0..0xff).to_a.shuffle.each do |x|
      decoded[-1] = x
      break if test_helper(decoded, block)
    end

    (1..LAST_BYTE).to_a.reverse.each do |n|
      r = decoded
      r[LAST_BYTE-n] ^= 1
      unless test_helper(r, block)
        (LAST_BYTE-n..LAST_BYTE).each do |x|
          decoded[x] ^= n
        end
        return [decoded, LAST_BYTE - n]
      end
    end

    decoded[-1] ^= 1

    [decoded, LAST_BYTE]
  end

  def decrypt_block(block)
    decoded, last_byte_recovered = last_word(block)

    debug "Last byte decrypted: #{last_byte_recovered}"

    (1..last_byte_recovered).to_a.reverse.each do |j|
      r = Array.new(LAST_BYTE+1) { rand(0xff) }
      (j..LAST_BYTE).each do |k|
        r[k] = decoded[k] ^ (LAST_BYTE - j + 2)
      end
      (0..0xff).to_a.shuffle.each do |x|
        r[j-1] = x
        if test_helper(r, block)
          debug "Decrypted byte #{j}"
          break
        end
      end
      decoded[j-1] = r[j-1] ^ (LAST_BYTE - j + 2)
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