# Install binaries to /usr/local/bin
# If there is an error building, stop everything.

if ! [[ -d build ]]
then
	mkdir build
fi

cd build
cmake ..

if sudo make install
then
	echo "Installation complete."
fi
