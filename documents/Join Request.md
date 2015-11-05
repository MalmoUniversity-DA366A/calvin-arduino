##Join request

###Terminal commands

-   Activate Calvin virtual enviroment

	`source ~/.virtualenvs/test-calvin/bin/activate`

-   Start a runtime

	`csruntime --host localhost --controlport 5001 --port 5000
    --keep-alive`

-   Stop current process and resume in background

	`ctrl+Z`
	
	`bg`

-   Invite arduino runtime to join a mesh

	`cscontrol http://localhost:5001 nodes add
    calvinip://ip-address:port`

-   If something went wrong and have to start a new runtime

	`sudo killall python`

-   To exit Calvin virtual environment

	`deactivate`

The program [Wireshark](https://www.wireshark.org/#download)
shows a successful join request and join reply between Calvin base and
Arduino. This is followed by a tunnel setup from Arduino. The four starting ascii-characters, before every json-message, are hex-values of the message length.

###Calvin base join request

	....{"cmd": "JOIN_REQUEST", "serializers": 	["json"], "id":
	"babc045f-5da1-4eb1-a08e-95e6a272cfde", "sid":
	"7f6ad070-8c5a-4d31-a90b-a4aee04a5e7a"}

###Arduino join reply and tunnel setup

	...k{"cmd": "JOIN_REPLY","id": "calvin-arduino","sid":
	 "7f6ad070-8c5a-4d31-a90b-a4aee04a5e7a","serializer":
	  "json"}....{"msg_uuid":"MSG-00531ac3-1d2d-454d-964a-7e9573f6ebb6",
	  "from_rt_uuid": "calvin-arduino","to_rt_uuid":
	  "babc045f-5da1-4eb1-a08e-95e6a272cfde","cmd":
	  "TUNNEL_NEW","tunnel_id": "fake-tunnel","policy":{},"type": "token"}
