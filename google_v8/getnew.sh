packages="git gcc g++ make subversion git-svn python"
sudo apt-get install $packages || sudo yaourt -s $packages || sudo yum install $packages
git clone git://github.com/v8/v8.git v8-read-only
cd v8-read-only
make dependencies CXXFLAGS=-fPIC
make native
