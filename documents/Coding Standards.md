###Coding Standards

The coding standard for this project is inspired by the Arduino code standard. When naming functions and variables "Camel case" should be used. E.g. "myFunction" is allowed while "my_function" should be avoided.

When writing code related to communication one should avoid to hard code ports and addresses in order to make the platform more versatile and usable.

All code should be well commented in order to further help the readability and thus making sure other students can carry on the development. While writing comments one should describe each function above the functions itself, and if necessary proved additional one-line comments when needed. When naming functions and variables avoid using abbreviations.

Functions should be kept short to improve code readability, in the case of two or more functions containing the same lines of code one should strive to extract that code to a separate function.

All functions and "if-", "or-" and "while-" statements should have curly brackets on the line below. In the case of nestled "if-", "or-" and "while-" statements one should abide to use the proper indentation to further ease the readability. The opening and closing curly braces should always be on the same indentation. The code within the function or "if-", "or-" and "while-" statements should be indented one tab.

E.g.

if(foo>1)

{

/*Do something here*/

if(foo>2)

{

/*Do something here*/

}

}
