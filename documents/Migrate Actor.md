##Migrate Actor

###Terminal commands

-   Activate Calvin virtual enviroment:

	`source \~/.virtualenvs/test-calvin/bin/activate` 

-   Start a runtime
   
	`csruntime --host localhost --controlport 5001 --port 5000 --keep-alive` 

-   Stop current process:

	`ctrl+Z`

-   Resume process in background

	`bg`

-	Invite arduino runtime to join a mesh:

	`cscontrol http://localhost:5001 nodes add calvinip://<ip-address>:<port>`

    
-   Step into the calvin-base folder
    
	`cscontrol http://localhost:5001 deploy <scriptname.calvin>`

-   Start a new command window and repeat step 1

-   List all active actors

	`cscontrol http://localhost:5001 actor list` 

-	Info about actor _(used to find out which actor id corresponds to which type of actor)_

	`cscontrol http://localhost:5001 actor info <actor-id>` 

-	Migrate actor to remote Calvin runtime:

	`cscontrol http://localhost:5001 actor migrate <actor-id> <arduino-id>`
