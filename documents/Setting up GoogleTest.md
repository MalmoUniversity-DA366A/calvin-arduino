#Eclipse with google test

###Step 1 Install testrunner
First you need to install a testrunner for eclipse. The by far esiest 
way to do so is to open Help->Install New Software and click on add
Name: Eclipse testrunner
Location: https://raw.githubusercontent.com/xgsa/cdt-tests-runner/tests_runner_demo/testsrunner/org.eclipse.cdt.testsrunner-updatesite/site.xml
Click finish and install.
Restart Eclipse

###Step 2 Install Cygwin
Download Cygwin from [here](https://www.cygwin.com/install.html) and run setup-x86_64.exe 
to install. Click on next untill you get to the Select package screen. You need the gcc 
package and the make package, best way to find them is to search.
gcc
![alt text](http://i.stack.imgur.com/wFapq.jpg "Logo Title Text 1")
make
![alt text](http://i.stack.imgur.com/2nxjA.jpg "Logo Title Text 1")

###Step 3 Download and build Google Test project

Download the latest relase of GoogleTest from [here](https://code.google.com/p/googletest/downloads/list) extract the files into C:\Development
(if u choose a diffrent directory don't forget to change the compiler include path in Eclipse )
Open the Cygwin terminal, cd to GoogleTest make directory on your drive (c:/<<yourpath>>/gtest-1.7.0/make/)
Type "make" to build the project. Then create an archived library out of the gtest-all.o file.
Type the following in the terminal:
ar -rv libgtest.a gtest-all.o


###Step 4 Add the Cygwin bin directory to the computers PATH
Follow [these](http://www.java.com/en/download/help/path.xml) instructions.


###Step 5 Include GoogleTest in project
Add GoogleTest's include directory to the Cygwin C++ compiler

![alt text](http://i.stack.imgur.com/Qs4Z2.png "Logo Title Text 1")

Under the Cygwin C++ linker folder select Miscellaneous and add libgtest.a that you created before.

![alt text](http://i.stack.imgur.com/WKi79.png "Logo Title Text 1")

### Step 6 Configure runtools.
Build the project in Mock mode
In Eclipse Run->Run Configurations.. 
Right click C/C++ Unit and select new.
All the filed in the main tab should now be filled. If not choose a name.
Under C/C++ application enter: Mock\Calvin.exe
Project should be Calvin. Click on the C/C++ Testing tap and choose Google Test Runner.
Click appaly and Run.
Now watch the automagic.

#Eclipse with google test for Mac os
This guide will describe how to install gtest for eclipse under Max OS,
for develop Arduino library. It will focus on a already existing project
and the Arduino Due board.

###Step 1 

First install the test runner as explained above.

Installing gtest-1.7.0 from
[here](https://code.google.com/p/googletest/downloads/list) and follow this instruction.

From terminal navigate to the download location
```
* unzip gtest-1.7.0

* cd ~/Downloads/gtest-1.7.0

* ./configure && make

* sudo cp ~/Downloads/gtest-1.7.0/lib/.libs/*.a /usr/local/lib/

* sudo cp -R ~/Downloads/gtest-1.7.0/include/ /usr/local/include/
```

###Step 2

Right click on your project and click *Properties* and navigate to
*“C/C++ Build” &gt; “Settings”. Under “Cross G++ Compiler” &gt;
“Includes”* add *”/usr/local/include”* as an include.

![Include](http://i.imgur.com/Lo2vV0K.jpg)


Under *“Cross G++ Linker” &gt; “Libraries”* add *gtest* as libraries
and *”/usr/local/lib”* as library search path.

![Libraries](http://i.imgur.com/AjPrUsO.jpg)


Well in properties go to *Tool Chain Editor* and change the toolchain to
*MacOS GCC,* then this is the one used to make gtest in the beginning.

![Toolchain](http://i.imgur.com/QhhQo8o.jpg)


###Step 3

With that sorted we’ll soon be ready to write some tests, but we still
miss two things: the build and run configurations.

To create a test build target right click on your project and navigate
to *“Build configurations” &gt; “Manage…”. Click “New”* and give it an
appropriate name, I’m calling mine *“UnitTest”* and choosing to copy
settings from *“Debug”*.

![Build configurations](http://i.imgur.com/n5ArzYk.jpg)

Before you do anything else we must exclude the system folder from
build, the system folder is used when flashing to the Arduino card. Just
right clicking the *system* folder and selecting *“Resource
Configurations” &gt; “Exclude from Build…”* and check *UnitTest*.

![Exclude system from UnitTest build](http://i.imgur.com/f8lbuwh.jpg)

Click *“Run” &gt; “Run Configurations…”* and click on *C/C++ Unit* in
the menu. Now create a new configuration named *“UnitTest”* and browse
to the location of your built binary under the *“UnitTest”* build
folder, for me the path was
*”\~/Documents/workspace/calvin-arduino/UnitTest/calvin-arduino.elf”*.
Select your project and under *“C/C++ Testing”* select *Google Tests
Runner* and press *Run*.

![Test runner](http://i.imgur.com/tFefTQj.jpg)
