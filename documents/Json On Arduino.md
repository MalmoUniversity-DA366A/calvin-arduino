**How to install Json for Arduino**

-   Download Json for Arduino from Github
    (<https://github.com/bblanchon/ArduinoJson.git>)

-   After downloading the Json repo, either throw away the *test* folder
    and the *third-party* folder or delete them from the Eclipse
    workspace after import, to avoid build errors

-   Copy the whole Json folder into the *libraries* folder in
    *calvin-arduino*

-   Right-click on the *Project Explorer* and choose *Import -&gt;
    General -&gt; Existing Projects into Workspace*. Under *Select root
    directory*, browse to the *ArduinoJson* folder and click finish when
    done

-   In *main.cpp*, add the line to the top: *\#include "ArduinoJson.h"*

-   Right-click on the *project workspace* and choose *properties -&gt;
    C/C++ Build -&gt; Settings*. In *Preprocessor* under the compiler
    for C, C++ and under Assembler, add the line: *ARDUINO*

-   Also in *Settings*, add the path for the *libraries -&gt;
    ArduinoJson* -&gt; *src* folder to *Includes* under the compiler for
    C, C++ and under Assembler. Click Apply and OK when done
