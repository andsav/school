require 'base64'
load 'oracle.rb'

raise RuntimeError, 'Expected cookie as argument' unless ARGV.length == 1

URL = 'http://ugster21.student.cs.uwaterloo.ca:4555'

cookie = Base64.decode64(ARGV[0])
blocks = cookie.unpack('s*')

puts Oracle.test_cookie(cookie)