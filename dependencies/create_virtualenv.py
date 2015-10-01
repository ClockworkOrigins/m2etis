import virtualenv
import textwrap
import subprocess
import os
import sys

######################

# Add required dependencies to the object dependenciesList.
# Syntax is equivalent to pip requirements files.
# If there are dependencies between packages mind that all packages will be installed in the specified order.

dependenciesList = [
	'paramiko==1.10.0', 
	'numpy==1.7.0',
	'matplotlib==1.2.1',
	'scipy==0.12.0',
	'scikit-learn==0.13.1',
	'pyyaml==3.10',
	'pymongo==2.5.1'
]

######################

# Get target directory
if len(sys.argv) != 2:
	print "Virtualenv creation failed: Target directory required as first argument!"
	sys.exit(1)
else:
	targetDirectory = sys.argv[1]

# Generate bootstrap script
pipCommand = ""
for dependency in dependenciesList:
#	pipCommand += "\t subprocess.call([join(home_dir, 'bin', 'pip'),'install', '--index-url=file://" + os.getcwd() + "/pypi_mirror/simple'," + "'" + dependency + "'])\n"
	pipCommand += "\t subprocess.call([join(home_dir, 'bin', 'pip'),'install'," + "'" + dependency + "'])\n"

afterInstallFunctionText = "import subprocess\ndef after_install(options, home_dir):\n" + pipCommand
bootstrapContent = virtualenv.create_bootstrap_script(textwrap.dedent(afterInstallFunctionText))
open('_bootstrap_virtualenv.py', 'w').write(bootstrapContent)

# Run bootstrap script
subprocess.call(['python','_bootstrap_virtualenv.py', '--system-site-packages', targetDirectory])

# Cleanup
#os.remove('_bootstrap_virtualenv.py')
