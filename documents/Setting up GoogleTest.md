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
Type make to build the project. Then create an archived library out of the gtest-all.o file:ar -rv libgtest.a gtest-all.o


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

