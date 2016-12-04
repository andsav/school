load 'oracle.rb'

raise RuntimeError, 'Expected plaintext as argument' unless ARGV.length == 1

start = Time.now

plaintext = ARGV[0].unpack('C*')

# Pad the plaintext
plaintext << BLOCK_SIZE - plaintext.length%BLOCK_SIZE
plaintext << 0 until plaintext.length%BLOCK_SIZE == 0

blocks = plaintext.each_slice(BLOCK_SIZE).to_a.collect { |x| x.pack('C*')}

puts Base64.strict_encode64 Oracle.encrypt(blocks).join

debug (Time.now - start)