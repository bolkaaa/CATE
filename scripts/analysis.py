import matplotlib.pyplot as plt
import numpy as np
import librosa
import librosa.display
import sys
import util

"""Functions for outputting analysis data of audio files, for comparing results
with C++ implementations."""

def show_info(values: np.ndarray):
    """Show useful information about array of analysis data."""
    min_value = np.min(values)
    max_value = np.max(values)
    mean_value = np.mean(values)
    message = 'Minimum: {}\nMaximum: {}\nMean: {}'
    print(message.format(min_value, max_value, mean_value))


class Analysis:
    @staticmethod
    def spectral_centroid(file_path: str) -> np.ndarray:
        y, sr = librosa.load(librosa.util.example_audio_file())
        values = librosa.feature.spectral_centroid(y=y, sr=sr)
        return values

    def spectral_flatness(file_path: str) -> np.ndarray:
        y, sr = librosa.load(librosa.util.example_audio_file())
        values = librosa.feature.spectral_flatness(y=y)
        return values


if __name__ == '__main__':
    mode = None
    file_path = None
    analysis_modes = {
        'centroid': Analysis.spectral_centroid,
        'flatness': Analysis.spectral_flatness
    }

    try:
        (mode, file_path) = util.check_args(num_expected=2)
    except ValueError:
        util.show_usage('analysis', analysis_modes)

    try:
        values = analysis_modes[mode](file_path)
        show_info(values)
    except KeyError:
        util.show_usage('analysis', analysis_modes)
