###Join request

**Terminal commands**

-   source \~/.virtualenvs/test-calvin/bin/activate (activate Calvin
    virtual enviroment)

-   csruntime --host localhost --controlport 5001 --port 5000
    --keep-alive (start a runtime)

-   ctrl+Z (stop current process)

-   bg (resume process in background)

-   cscontrol http://localhost:5001 nodes add
    calvinip://&lt;ip-address&gt;:&lt;port&gt; (invite arduino runtime
    to join a mesh)

-   sudo killall python (if something went wrong and have to start a
    new runtime)

-   deactivate (to exit Calvin virtual environment)
