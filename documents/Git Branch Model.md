# Why Github?

We chose Github as version control so that all the members are able to work on the same project at the same time on different machines. With Github we will be synchronized to the remote repository on all machines.

##Branch Model

We decided to setup our Github repository with 3 infinite lifetime branches, develop, master and release. Which is based upon "[A successful Git branching model](http://nvie.com/posts/a-successful-git-branching-model/)" by Vincent Driessen.

There are also several temporary feature branches named with appropriate names created from the develop branch. These feature branches are created for each new feature set and killed upon the completion of the task. The progress from the feature task is merged to the develop branch once the code has passed testing. Finished code from the develop branch is merged with the release branch after a full system testing and documentation. The branches are accessed through MalmoUniversity-DA366A/calvin-arduino repository.

##How-To

This describes how to get an remote repository to an existing local directory. First a Github shell needs to be installed. A program called Github Desktop contains both a desktop client and a shell.

* Download and install [Github Desktop](https://desktop.github.com/) (Installation automatically creates a Github folder in Documents)

* Start Git Shell program (Program starts automatically in your Github folder)

The command git clone which gives the user a full copy of the data from the remote repository on Github.

###Calvin-Arduino
`git clone https://github.com/MalmoUniversity-DA366A/calvin-arduino.git`

###Calvin-Base
`git clone https://github.com/MalmoUniversity-DA366A/calvin-base.git`

###Calvin-Mini
`git clone https://github.com/MalmoUniversity-DA366A/calvin-mini.git`

### Step into calvin-arduino folder
`cd calvin-arduino`

###Branch a new feature from _develop_ branch
- If not already in develop, step into develop branch

`git checkout develop`

- Create new feature branch from develop branch

`git checkout -b newBranchName`

###Check branch updates/changes
- Display uncommitted added files

`git status`

###Download latest up-to-date version of branch
`git pull`

###Commit local work to branch
- Add all files

`git add --all`

- Always write a message

`git commit -m "message describing what you have done"`

###Push work to repository
`git push`

###Merge feature branch with develop branch
- Step into develop branch before merging

`git checkout develop`
	
- Merge myNewBranchName with develop branch and no default fast forward mode to keep branch history

`git merge --no-ff myNewBranchName`

###Kill an obsolete merged feature branch
`git branch –d myNewBranchName`

###Remove file from Github repo but not from local computer
`git rm -r --cached filename`

`git commit -m "file removed"`

###Ignore and/or delete the hidden Mac file .DS_Store
- Show hidden files/folders

`defaults write com.apple.finder AppleShowAllFiles -bool YES`

- Add this line to the hidden .gitignore file in the repository

`.DS_Store`

- Remove .DS_Store file if already committed

`find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch`

- Add changes to .gitignore file and commit

`git add .gitignore` 

`git commit -m ".DS_Store banished!"`

- Hide hidden files/folders

`defaults write com.apple.finder AppleShowAllFiles -bool NO`
