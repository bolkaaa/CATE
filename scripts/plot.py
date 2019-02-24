import matplotlib.pyplot as plt
import numpy as np
import librosa
import librosa.display
import util

"""Functions for plotting audio files, for comparing results with C++
implementations."""


class Plot:
    @staticmethod
    def waveform(file_path: str):
        """Time-domain plot of audio file."""
        y, sr = librosa.load(file_path)
        librosa.display.waveplot(y, sr=sr)
        plt.show()

    @staticmethod
    def stft(file_path: str):
        """Short-time Fourier transform plot of audio file."""
        y, sr = librosa.load(file_path)
        d = librosa.amplitude_to_db(np.abs(librosa.stft(y)), ref=np.max)
        librosa.display.specshow(d, y_axis='linear')
        plt.show()


if __name__ == '__main__':
    plot_modes = {
        'waveform': Plot.waveform,
        'spectrum': Plot.stft
    }

    try:
        (mode, file_path) = util.check_args(num_expected=2)
    except ValueError:
        util.show_usage('plot', plot_modes)

    try:
        plot_modes[mode](file_path)
    except KeyError:
        util.show_usage('plot', plot_modes)
