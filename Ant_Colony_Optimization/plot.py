from matplotlib import pyplot as plt
import sys

filename = sys.argv[1]

with open(filename+"/ans_"+filename+".txt", 'r') as fin:
    order = fin.read().splitlines()[1:]

result = [None] * len(order)
for i, j in enumerate(order):
    result[int(j)-1] = i

with open(filename+"/point.txt", 'r') as fin:
    points = list(zip(*[line.split(' ') for line in fin.read().splitlines()]))

xs = [int(x[1]) for x in sorted(dict(zip(result, points[1])).items())]
ys = [int(y[1]) for y in sorted(dict(zip(result, points[2])).items())]
xs.append(xs[0])
ys.append(ys[0])
 
_, ax = plt.subplots(num="fig_"+filename)
ax.plot(xs, ys, linestyle = '-', marker = '')
ax.scatter(xs, ys)
for i, txt in enumerate(order):
    ax.annotate(txt, (xs[i], ys[i]))
ax.set_title(filename)
plt.show()