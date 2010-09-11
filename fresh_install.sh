# This is an untested script to install Pidgin Plus! from your SVN checkout.
# I have never successfully built Pidgin Plus! on Windows.
# Technically, it should be possible, but this script is obviously for Linux, aptitude based.

if test "$gname" == "";
then
  gname="apt-get";
fi

sudo $gname install g++
sudo $gname install subversion
sudo $gname install python
sudo $gname install scons
sudo $gname build-dep pidgin
sudo $gname install libpurple-dev
sudo $gname install libc6-dev-i386

export GCC_VERSION=44

archspec=""
if test `uname -m` == 'x86_64'
then archspec="arch=x64"
elif test `uname -m` == 'x64'
then archspec="arch=x64"
fi

cd google_v8
svn checkout http://v8.googlecode.com/svn/trunk/ v8-read-only
cd v8-read-only
scons mode=release library=static snapshot=on
cd ../../

make Release
mkdir ~/.purple/plugins/
mv pidgin_plus.so ~/.purple/plugins/pidgin_plus.so

