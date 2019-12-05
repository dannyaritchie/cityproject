#Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 2.7)

# Name of the project
project(Nproject)

# Add all the source files needed to build the executable
add_executable(Nproject src/main.cpp 
	src/helpers/functions.cpp
	src/models/ball.cpp 
    	src/models/frame.cpp 
    	src/models/frameobjects.cpp
    	src/models/game.cpp 
    	src/models/player.cpp 
    	src/models/possessionfilereader/possessionfileline.cpp
    	src/models/possessionfilereader/possessionfile.cpp 
    	src/models/possessionfilereader/possessionfilemaker.cpp
	#src/models/infogroups/infogroupa.cpp
	#src/models/infogroups/phase.cpp
	src/models/briantable/allclosest.cpp
	src/models/briantable/pressureprocessor.cpp
	src/models/idmap.cpp
	src/STATS/runningstats.cpp
	#src/models/wholeplayer.cpp 
    	#src/models/angmod/computeangle.cpp
	)

# Include the directory where MyLibrary project is. Otherwise, we can't use the
# library
target_include_directories(Nproject PRIVATE src/ReadTrackingData/include)
# Link the executable and the library together
