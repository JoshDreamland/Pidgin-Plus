sudo apt-get install subversion python scons || sudo yaourt -s subversion python scons || sudo yum install subversion python scons
svn checkout http://v8.googlecode.com/svn/trunk/ v8-read-only
cd v8-read-only
scons mode=debug library=shared snapshot=on

