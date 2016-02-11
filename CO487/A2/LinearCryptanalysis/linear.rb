require 'enumerator'

class Linear
   
    @@SBOX =[0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7] 

    def initialize(ciphertexts, plaintexts)
        @ciphertexts = ciphertexts
        @plaintexts = plaintexts
    end

    # XOR 2 binary strings
    def xor(a, b)
        b = b.split('')

        a.split('')
            .map
            .with_index {|x, i| Integer(x) ^ Integer(b[i])}
            .join
    end
    
    def sbox(input); @@SBOX[input] end
        
    def sbox_inverse(input); @@SBOX.index(input) end
    
    def v(c, key)
        c = [ c[4..7], c[12..15] ].join
        xor(c, key)
    end

    def v2(c, key)
        xor(c[0..15], key)
    end

    def u(v)
        u = []
        v.split('').each_slice(4) do |vv|
            sbox_inverse(vv.join.to_i(2))
                .to_s(2)
                .rjust(4, '0')
                .split('')
                .each { |bit| u.push Integer(bit) }
        end
        u
    end

    def bias(key)
        ones = 0

        @ciphertexts.each_with_index do |c, i|
            v = v(c, key)
            u = u(v)
            p = @plaintexts[i]
            
            # Bits used : U6, U8, U14, U16, P5, P7, P8
            bits = [ u[1], u[3], u[5], u[7], p[4], p[6], p[7] ].collect { |x| Integer(x) }
                
            ones += 1 if bits.inject(:^) == 1
        end
        
        n = @ciphertexts.length
        diff = ones - n/2
        (Float(diff)/Float(n)).abs
    end

    def bias2(key)
        ones = 0

        @ciphertexts.each_with_index do |c, i|
            v = v2(c, key)
            u = u(v)
            p = @plaintexts[i]

            bits = [ u[1], u[5], u[9], u[13], p[0], p[3], p[8], p[11] ].collect { |x| Integer(x) }

            ones += 1 if bits.inject(:^) == 1
        end

        n = @ciphertexts.length
        diff = ones - n/2
        (Float(diff)/Float(n)).abs
    end
    
    # Q2A: Compute the magnitude of the bias over one subkey
    def q2a(partial_subkey = '01110110')
        return bias(partial_subkey)
    end
    
    # Q2B: Compute the bias for all possible partial subkeys, return the key with highest bias
    def q2b()
        possible_subkeys = (0..2**8-1).collect {|x| x.to_s(2).rjust(8, '0') } 
        max_bias = [0, 0]
        
        possible_subkeys.each do |k|
            b = bias(k)
            max_bias = [k, b] if max_bias[1] < b
        end
        
        max_bias
    end
    
    # Q2D: Compute the entire subkey K5 based on the linear approximation derived in c
    def q2d()
        a = (0..2**4-1).collect {|x| x.to_s(2).rjust(4, '0') }
        
        max_bias = [0, 0]
        a.each do |k1|
            a.each do |k2|
                k = k1 + "1110" + k2 + "0111" #partial subkey derived in q2b
                b = bias2(k)
                max_bias = [k, b] if max_bias[1] < b
            end
        end

       max_bias 
    end
end

analyzer = Linear.new(File.readlines('ciphertext88.txt'), 
                     File.readlines('plaintexts.txt'))


puts "2a) Bias for 0110110: #{analyzer.q2a()}"

q2b = analyzer.q2b()
puts "2b) Partial subkey: #{q2b[0]} (bias: #{q2b[1]})"

q2d = analyzer.q2d()
puts "2d) Complete subkey: #{q2d[0]} (bias: #{q2d[1]})"

