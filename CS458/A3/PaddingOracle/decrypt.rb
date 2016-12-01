require 'base64'
load 'oracle.rb'

raise RuntimeError, 'Expected cookie as argument' unless ARGV.length == 1

URL = 'http://ugster21.student.cs.uwaterloo.ca:4555'

COOKIE = Base64.strict_decode64(ARGV[0])
blocks = COOKIE.chars.each_slice(16).map(&:join)
IV = blocks.shift

puts Oracle.decrypt(blocks).join
