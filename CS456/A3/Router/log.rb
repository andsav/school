module Log
  module_function

  def init
    @log_file = open("router#{ROUTER_ID}.log", 'w')
  end

  def write(content)
    @log_file.write("#{content}")
    @log_file.flush
  end

  def write_action(action, argument)
    write("R#{ROUTER_ID} #{action} #{argument}\n")
  end

  def section
    write("\n")
  end
end