# Build directory
directory = ~/build

# Do all the things.
build_fileopen: $(directory)
	gcc src/*.h src/*.c -o build/g2fileopen
	@echo "Finished building."

# Check if the 'build' directory exists
$(directory):
	@echo "Creating build directory."
	mkdir $@
 