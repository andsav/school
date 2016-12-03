load 'oracle.rb'

raise RuntimeError, 'Expected cookie as argument' unless ARGV.length == 1

COOKIE = Base64.strict_decode64(ARGV[0])
blocks = COOKIE.chars.each_slice(BLOCK_SIZE).map(&:join)

puts Oracle.decrypt(blocks).join