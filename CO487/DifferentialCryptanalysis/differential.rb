require 'enumerator'

class Differential
    @@du4 = "0000011000000110"

    @@SBOX =[0xe, 0x4, 0xd, 0x1, 0x2, 0xf, 0xb, 0x8, 0x3, 0xa, 0x6, 0xc, 0x5, 0x9, 0x0, 0x7] 

    def initialize(chosen)
        @ciphertexts = @plaintexts = []
        
        chosen.each do |a|
            a = a.split(',')
            @plaintexts << xor(a[0], a[1])
            @ciphertexts << xor(a[2], a[3])
        end
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

    def matches(partial)
        count = 0
        
        @ciphertexts.each do |key|
            v = v(key, partial)
            u = u(v)
            
            target = [ key[0..3],  u[0..3], key[8..11],  u[4..7] ].join
            
            count += 1 if target == @@du4
        end

        count
    end

    def q3a()
        possible = (0..2**8-1).collect { |x| x.to_s(2).rjust(8, '0') }
        
        max = [0, 0]
        possible.each do |k|
            m = matches(k)
            max = [k, m] if m > max[1]
        end

        max
    end
end


analyzer = Differential.new(File.readlines('ciphertext88.txt'))

puts analyzer.q3a()
