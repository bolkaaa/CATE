import matplotlib.pyplot as plt

"""Prototyping envelope generation algorithms to verify their correctness
through observing their plots."""

def ASR(attack, sustain, release, samples):
    """Trapezoidal ASR envelope."""
    amp = 0
    attack_samples = samples * attack
    release_samples = samples * release
    points = []

    for i in range(samples):
        if i < attack_samples:
            amp_incr = sustain / attack_samples

        elif attack_samples <= i <= (samples - release_samples):
            amp_incr = 0

        else:
            amp_incr = -(sustain / release_samples)

        amp += amp_incr
        points.append(amp)

    return points


def plot_points(points):
    """Plot points of an envelope generator."""
    x = range(len(points))
    y = points
    plt.plot(x, y)
    plt.xlabel("Sample")
    plt.ylabel("Level")


if __name__ == '__main__':
    points = ASR(attack=0.25, sustain=0.5, release=0.25, samples=1024)
    plot_points(points)
    plt.show()
