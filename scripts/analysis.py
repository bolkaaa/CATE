import matplotlib.pyplot as plt
import numpy as np
import librosa
import librosa.display
import sys
import util

"""Functions for outputting analysis data of audio files, for comparing results
with C++ implementations."""


class Analysis:
    @staticmethod
    def spectral_centroid(file_path: str) -> np.ndarray:
         y, sr = librosa.load(file_path)
         centroid = librosa.feature.spectral_centroid(y=y, sr=sr)
         print(centroid)


if __name__ == '__main__':
    analysis_modes = {
        'spectral_centroid': Analysis.spectral_centroid
    }

    try:
        (mode, file_path) = util.check_args(num_expected=2)
    except ValueError:
        util.show_usage('analysis', analysis_modes)

    try:
        analysis_modes[mode](file_path)
    except KeyError:
        util.show_usage('analysis', analysis_modes)

