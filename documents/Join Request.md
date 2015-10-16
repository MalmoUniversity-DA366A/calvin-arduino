#Join request

##Terminal commands

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

The program Wireshark
([*https://www.wireshark.org/\#download)*](https://www.wireshark.org/#download))
shows a successful join request and join reply between Calvin base and
Arduino.

###Calvin base join request

....{"cmd": "JOIN\_REQUEST", "serializers": \["json"\], "id":
"babc045f-5da1-4eb1-a08e-95e6a272cfde", "sid":
"7f6ad070-8c5a-4d31-a90b-a4aee04a5e7a"}

###Arduino join reply

...k{"cmd":"JOIN\_REPLY","id":"calvin-arduino","sid":"7f6ad070-8c5a-4d31-a90b-a4aee04a5e7a","serializer":"json"}