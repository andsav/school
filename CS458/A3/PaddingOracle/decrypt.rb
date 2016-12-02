require 'base64'
load 'oracle.rb'

raise RuntimeError, 'Expected cookie as argument' unless ARGV.length == 1

URL = 'http://localhost:4555'

COOKIE = Base64.strict_decode64(ARGV[0])
blocks = COOKIE.chars.each_slice(16).map(&:join)
IV = blocks.shift

debug "#{blocks.length} blocks + IV"

puts Oracle.decrypt(blocks).join


=begin
case ARGV.length
  when 1
    puts Oracle.decrypt(blocks).join
  when 2
    KEY = ARGV[1]

    cipher = OpenSSL::Cipher::AES.new(128, :CBC)
    cipher.decrypt
    cipher.iv = IV
    cipher.key = KEY
    cipher.padding = 0
    puts cipher.update(blocks.join) + cipher.final
  else raise RuntimeError, 'Expected cookie as argument'
end
=end
