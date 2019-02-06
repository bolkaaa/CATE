import matplotlib.pyplot as plt
import numpy as np
import librosa
import librosa.display
import sys


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


def show_usage():
    """Show argument format and which commands are available."""
    print('Usage: ./plot.py <plot_type> <file_path>.')
    print('PLOT TYPES:')
    print('waveform:\ntime domain representation\n')
    print('spectrum:\nshort-time Fourier transform representation')
    sys.exit()


def check_args():
    """Show usage information if number of provided command
    line arguments is not 2."""
    num_expected_args = 2
    if (len(sys.argv)-1 is not num_expected_args):
        show_usage()
        

if __name__ == '__main__':
    check_args()
    plot_mode = sys.argv[1]
    file_path = sys.argv[2]

    plot_modes = {
        'waveform': Plot.waveform,
        'spectrum': Plot.stft
    }

    try:
        plot_modes[plot_mode](file_path)

    except KeyError:
        show_usage()
