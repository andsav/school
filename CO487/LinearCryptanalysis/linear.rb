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
        c = [ c[4], c[5], c[6], c[7], c[12], c[13], c[14], c[15] ].join
        xor(c, key)
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
        zeroes = ones = 0

        @ciphertexts.each_with_index do |c, i|
            v = v(c, key)
            u = u(v)
            p = @plaintexts[i]
            
            # Bits used : U6, U8, U14, U16, P5, P7, P8
            bits = [ u[1], u[3], u[5], u[7], p[4], p[6], p[7] ].collect { |x| Integer(x) }
                
            if bits.inject(:^) == 0
                zeroes += 1 
            else 
                ones += 1 
            end
        end
        
        n = @ciphertexts.length
        ((ones - (n / 2)) / n).abs
    end
    
    # Q2A: Compute the magnitude of the bias over one subkey
    def q2a(partial_subkey = '01110110')
        return bias(partial_subkey)
    end
    
    # Q2B: Compute the bias for all possible partial keys
    def q2b()

    end
end

analyzer = Linear.new(File.readlines('ciphertext88.txt'), 
                     File.readlines('plaintexts.txt'))

puts analyzer.q2a
