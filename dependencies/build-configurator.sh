cd "$(readlink "$(dirname "${0}")")"
mkdir -p pyvirtenv
python create_virtualenv.py pyvirtenv
