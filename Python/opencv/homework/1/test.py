import numpy as np

f = np.array([[0, 1, 0, 2], [0, 3, 0, 4], [0, 5, 0, 6], [0, 7, 0, 8]])
F = np.fft.fft2(f)
F_shifted = np.fft.fftshift(F)
magnitude_spectrum = np.abs(F_shifted)

print(F_shifted)
print(magnitude_spectrum)

import matplotlib.pyplot as plt

plt.imshow(magnitude_spectrum, cmap='gray')
plt.title('Magnitude Spectrum')
plt.colorbar()
plt.show()

