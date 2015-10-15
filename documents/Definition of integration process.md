#Definition of integration process

For testing C code we will use the unity test harness, these test will run on the
due board with their output on the console. When we move our code in to the library 
unity can no longer be used as test framework because of the library is written in cpp files.
Therefore cppUTest will be used to test the code in the integration tests. 
These test will compile and run locally on the developers computer, using mocks to mock due functionality. 