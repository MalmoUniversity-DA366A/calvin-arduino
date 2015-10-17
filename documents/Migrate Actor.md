##Migrate Actor

###Terminal commands

-   source \~/.virtualenvs/test-calvin/bin/activate (activate Calvin
    virtual enviroment)

-   csruntime --host localhost --controlport 5001 --port 5000
    --keep-alive (start a runtime)

-   ctrl+Z (stop current process)

-   bg (resume process in background)

-   cscontrol http://localhost:5001 nodes add
    calvinip://&lt;ip-address&gt;:&lt;port&gt; (invite arduino runtime
    to join a mesh)
    
-   Step into calvin-base folder
    
-   cscontrol http://localhost:5001 deploy &lt;scriptname.calvin&gt;

-   Start a new command window and repeat step 1

-   cscontrol http://localhost:5001 actor list (list all active actors)

-   cscontrol http://localhost:5001 actor &lt;actor-id&gt; (info about actor)

-   cscontrol http://localhost:5001 actor migrate
    &lt;actor-id&gt; &lt;arduino-id&gt;
