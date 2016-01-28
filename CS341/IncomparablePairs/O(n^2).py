import fileinput

x = []
y = []
for line in fileinput.input():
    x.append(int(line.split(' ')[0]))
    y.append(int(line.split(' ')[1]))

inversions = 0
for i in range(x.__len__()):
    for j in range(x.__len__()):
        if not (x[i] >= x[j] and y[i] >= y[j]) or (x[i] < x[j] and y[i] < y[j]):
            inversions += 1

print inversions
