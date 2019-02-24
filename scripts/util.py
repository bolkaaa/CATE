import sys

"""Utility helper functions for the plot and analysis scripts."""


def show_usage(name: str, modes: dict):
    """Show argument format and which commands are available."""
    mode_list = '\n'.join([m for m in modes.keys()])
    command = 'python3 {0}.py <{0}_type> <file_path>'.format(name)
    usage = 'Usage: {}\n{} modes:\n{}'.format(command, name, mode_list)
    sys.exit(usage)


def check_args(num_expected: int) -> (str, str):
    """Show usage information if invalid number of arguments."""
    if (len(sys.argv)-1 != num_expected):
        raise ValueError
    else:
        mode = sys.argv[1]
        file_path = sys.argv[2]
        return (mode, file_path)
