# Compile project, then move binary and compile_commands.json to root.
# If there is an error building, stop everything.

if ! [[ -d build ]]
then
	mkdir build
fi

if ! [[ -d executable ]]
then
	mkdir executable
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..

if ! [[ -e compile_commands.json ]] 
then
	cmake -DCMAKE_BUILD_TYPE=Debug ..
fi


if make
then
	mv compile_commands.json ../
	# mv GPU ../executable
	mv CPUU ../executable
	echo "Compilation done."
fi
