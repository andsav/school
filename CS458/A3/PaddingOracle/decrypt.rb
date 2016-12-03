require 'base64'
load 'oracle.rb'

raise RuntimeError, 'Expected cookie as argument' unless ARGV.length == 1

COOKIE = Base64.strict_decode64(ARGV[0])
blocks = COOKIE.chars.each_slice(16).map(&:join)

debug "#{blocks.length-1} blocks + IV"

puts Oracle.decrypt(blocks).join