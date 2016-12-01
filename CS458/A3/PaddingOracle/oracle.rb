require 'base64'
require 'open-uri'

def debug(msg)
  $stderr.puts msg
end

module Oracle
  module_function

  def test_cookie(cookie)
    encoded = Base64.strict_encode64(cookie)
    begin
      debug "Sending #{encoded} to server"

      open(URL, 'Cookie' => "user=\"#{encoded}\"")

      debug 'Server accepts the cookie'
      true
    rescue OpenURI::HTTPError => ex
      debug 'Server rejects the cookie'
      false
    end
  end

  def xor(a, b)
    [a, b].transpose.map { |x| x.reduce(:^) }
  end

  def test_helper(byte_array, block)
    test_cookie(byte_array.pack('C*') + block)
  end

  def last_word(block)
    decoded = Array.new(16) { rand(0xff) }

    (0..0xff).to_a.shuffle.each do |x|
      decoded[-1] = x
      break if test_helper(decoded, block)
    end

    decoded
  end

  def decrypt_block(block)
    decoded = last_word(block)

    r = Array.new(16) { rand(0xff) }
    (1..15).to_a.reverse.each do |j|
      (j..15).each do |k|
        r[k] = decoded[k] ^ (15 - j + 2)
      end
      (0..0xff).to_a.shuffle.each do |x|
        r[j-1] = x
        break if test_helper(r, block)
      end
      decoded[j-1] = r[j-1] ^ (15 - j + 2)
    end

    "L"
  end
end