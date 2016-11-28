class CircuitDB < Hash

  # Override the HashMap pretty print (for logging)
  def to_s
    super.sub('{', '(L').sub('}', ')').gsub('=>', ': ').gsub('"', '').gsub(', ', ', L')
  end

end