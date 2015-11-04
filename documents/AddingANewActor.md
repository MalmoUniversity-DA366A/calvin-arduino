#How to add a new actor

New actors can easily be implemented to in calvin arduino. This is a step by step guide for adding new actors.

1. Write your new calvin arduino actor in the **Actor.cpp** file. Along with the function header in the **Actors.h** and if necessary include any libraries used by the actor.

2. When your actor function is done you need to initiate it. This is done in the `actorInit()` function.
3. Create your Calvin-base actor in the appropriate folder in the `X:\...\calvin-base\calvin\actorstore\systemactors` folder. The "io." actor in the io folder and the "std" actor in the std folder. If needed have a look in the *std/std.Counter.py* or *io/io.StndardOut.py* files. The name of the actor must be tha same as the Calvin-arduino actor but should not contain the "io." or "std." prefix. For further information about actors please visit [Ericssons Github Wiki](https://github.com/EricssonResearch/calvin-base/wiki/Actors).
4. Create your Calvin script. For further information about scripts please visit [Ericssons Github Wiki](https://github.com/EricssonResearch/calvin-base/wiki/Applications). 

Here is an example how to initiate the StandardOut actor. The string "io.StandardOut" is the name of the actor that Calvin base sends to Calvin mini on the Arduino. 


    if(!strcmp(inputActor->type.c_str(),"io.StandardOut"))
    {
    	inputActor->fire = &actorStdOut;
    }

The following lines should be added to the `actorInit()` function. Where **`"io.MyActorName"`** corresponds to the name of your actor and **`&MyActorFunction`** to the function to be called upon fire actor. 

    else if(!strcmp(inputActor->type.c_str(),"io.MyActorName"))
    {
    	inputActor->fire = &MyActorFunction;
    }


There are **two** types of supported actors in calvin-arduino these are defined by their prefix. **Source actors** that is used to interact with sensors and produce tokens should have the **std prefix**. **Output actors** should recive tokens from an source actor and do something with them, thier prefix should be **io**. Example:

`io.StandardOut` - Source actor

`std.Counter` - Output actor

Where the io.StandardOut prints tokens from the std.Counter actor.