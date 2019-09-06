import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

DATA_DIR = 'DATA/'
IMAGE_DIR = 'IMG/'

# def plotHistogram(ifile, ofile):
#     data = np.loadtxt(DATA_DIR + ifile, delimiter = ',')
#     plt.figure()
#     plot = sns.distplot(data, norm_hist=True)
#     plot.figure.savefig(IMAGE_DIR + ofile, dpi=300, bbox_inches='tight')

# plotHistogram('testData.csv', 'testData.png')
# plotHistogram('sampleData.csv', 'sampleData.png')


i1, mu1, sigma1 = np.loadtxt(DATA_DIR + 'accepted.csv', delimiter = ',', unpack=True)
i2, mu2, sigma2 = np.loadtxt(DATA_DIR + 'rejected.csv', delimiter = ',', unpack=True)

print("Accepted:", len(i1))
print("Rejected:", len(i2))

fig = plt.figure(figsize=(24,20))

ax1 = plt.subplot(2, 2, 1)
plt.plot(i1, mu1, 'o-', markersize=0.5)
plt.scatter(i2, mu2, c='r', marker='x', s=0.5)
ax1.set_ylabel('$\mu$', fontsize=14)
ax1.set_xlabel('Iteration', fontsize=14)

ax2 = plt.subplot(2, 2, 3)
plt.plot(i1, sigma1, 'o-', markersize=0.5)
plt.scatter(i2, sigma2, c='r', marker='x', s=0.5)
ax2.set_ylabel('$\sigma$', fontsize=14)
ax2.set_xlabel('Iteration', fontsize=14)

ax3 = plt.subplot(2, 2, 2)
CUTOFF = 2000
a = np.where(i1>CUTOFF)[0][0]
ax3.set_xlabel('$\mu$', fontsize=14)
plot = sns.distplot(mu1[a:], norm_hist=True, bins=7)

ax4 = plt.subplot(2, 2, 4)
plot = sns.distplot(sigma1[a:], norm_hist=True, bins=7)
ax4.set_xlabel('$\sigma$', fontsize=14)

plot.figure.savefig(IMAGE_DIR + "Model_output", dpi=300, bbox_inches='tight')

plt.figure()
plot = plt.hist2d(mu1[a:], sigma1[a:], density='normed')
plt.savefig(IMAGE_DIR + 'paramHeatmap', dpi=300, bbox_inches='tight')

