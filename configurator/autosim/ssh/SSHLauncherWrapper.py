__author__ = 'Andreas M. Wahl'


import sys
import os

runCommand = "chmod +x " + sys.argv[1]
os.system(runCommand)
runCommand = 'test "$SSH_AUTH_SOCK" || exec ssh-agent $SHELL -c "ssh-add; exec $SHELL -c "./' + sys.argv[1] +  '"' + '"'

print runCommand
os.system(runCommand)
