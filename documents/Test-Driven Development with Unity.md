###Test-Driven Development with Unity

Test-Driven development is a technique for building software incrementally with the unit testing. We implemented Unity for our software based unit testing. Unity is a lightweight unit testing framework for testing units in C.

Units of source code must pass the tests before it will be transferred to the release branch.

Unity uses a system that the testing code is isolated from the source code, so when the unit of source code passed the tests the code related to testing can be removed from the project.

Code for the unit testing will be separated from the src folder by being placed in separated folders named test and unity. The unity folder contains the files for the Unity testing framework. The test folder contains a runnerfile and code for testing each unit in the src folder.

####How to setup Unity in Eclipse

1. Create a new source folder by right click on the project. Name the source folder "unity".
2. Create a new source folder by right click on the project. Name the source folder "test".
3. Download Unity from <https://github.com/ThrowTheSwitch/Unity.git>
4. Copy unity.c, unity.h and unity_internals.h from the Unity/src folder to your unity folder in the Eclipse project.
5. In the test folder you put c and h files containing unit test code for the source code in the src folder. Files in the test folder MUST include "unity.h".
6. Also in the test folder you put your TestRunner file, which contain the main file for the project.
7. For further information look at the Readme.md in <https://github.com/ThrowTheSwitch/Unity.git>

####Unity Example

To get things started an Unity exampel can be found in <https://github.com/jesperhansen17/UnityExampel.git> 

####Test documentation

A link to out test documentation <https://onedrive.live.com/redir?resid=5546A4196DE3A96C!334764&authkey=!AKSEN0zjymWxiZw&ithint=file%2cxlsx>
