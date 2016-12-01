require 'base64'
require 'open-uri'

module Oracle
  module_function

  def test_cookie(cookie)
    encoded = Base64.strict_encode64(cookie)
    begin
      open(URL, 'Cookie' => "user=\"#{encoded}\"")
      true
    rescue OpenURI::HTTPError => ex
      false
    end
  end

  def last_word
    rs = (-0xffff..0xffff).to_a.shuffle

  end
end