# calvin-arduino

This is a port to Arduino of a minimal implementaion of [Ericsson's Calvin platform](http://www.ericsson.com/research-blog/cloud/open-source-calvin/).
The port is implemented as an arduino library and should be called during `setup()` within the sketch.

###Repository

We decided to setup our Github repository with 3 infinite lifetime branches, develop, master and release. Which is based upon "A successful Git branching model" (http://nvie.com/posts/a-successful-git-branching-model/) by Vincent Driessen.

There are also several temporary feature branches named with appropriate names created from the develop branch. These feature branches are created for each new feature set and killed upon the completion of the task. The progress from the feature task is merged to the develop branch once the code has passed testing. Finished code from the develop branch is merged with the release branch after a full system testing and documentation. The branches are accessed through MalmoUniversity-DA366A/calvin-arduino repository.

The branches are then divided into several folders. More information about the repository can be found in the documents folder.
