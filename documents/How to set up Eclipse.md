#Eclipse

In order for the development of project to be OS agnostic we choose to use Eclipse Juno CDT as IDE. The main reason for this is it’s flexibility and extended support. Most of the team members will use Windows, but since we are working with Calvin that runs in a Linux environment, having an IDE that can run under both Linux and Windows further eases the development. An Additional reason for us to use the Eclipse IDE is it’s vast array of available plugins.

In order for Eclipse to support development for Arduino Due the IDE needs a few tools and extra settings. The steps required are shown in the following guide:

##How to set up the Eclipse

###Clone the repo

Clone from

git clone Arduino Due Template project

###Setting up Eclipse for linux and mac

Follow [this](http://gnuarmeclipse.livius.net/blog/install/) guide to set up the project for linux and mac.

###Setting up Eclipse for Windows

1. Download and install the latest Java SE Dev Kit

2. Download and install the latest version of Eclipse IDE for C/C++ Developers
[https://eclipse.org/downloads/](https://eclipse.org/downloads/) 
*(Eclipse mars was used when cerating this project)*

3. Install GNU ARM Eclipse Plug-ins
Open Eclipse.
Help -> Install New Software -> Add...
Name: GNU ARM Eclipse Plug-ins
Location: http://gnuarmeclipse.sourceforge.net/updates

Select everything and install

4. Download GNU Tools for ARM Embedded Processors
https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q2-update/+download/gcc-arm-none-eabi-4_8-2014q2-20140609-win32.exe
Install and UNCHECK "Launch gccvar.bat" and "Add path to environment variable"

5. Download Cross Build Tools
http://sourceforge.net/projects/gnuarmeclipse/files/Build%20Tools/
Extract to C:/Program Files/Cross Build Tools/
Remove "cs-" prefix on make and rm. (It should now say rm.exe and make.exe)

6. In Eclipse import the template project to your workspace.

#Setting up Bossac


###Bossac for Eclipse (Windows)

Download Bossac from 
[https://mega.co.nz/#!rdoAVagB!HzJXs4KximVynrSuvzPDnOMizIqJoo8zhQZ23KHCyYQ](https://mega.co.nz/#!rdoAVagB!HzJXs4KximVynrSuvzPDnOMizIqJoo8zhQZ23KHCyYQ)

Extract these to C:\Program Files (x86)\BOSSA\

In Eclipse Run -> External tools -> External Tools Configurations
Right click on Program select New

Add Location path to BossacArduinoDue.bat
C:\Program Files (x86)\BOSSA\bat\BossacArduinoDue.bat

Add Argument
"C:\Program Files (x86)\BOSSA\bat\bossac.exe" "${project_loc}/Debug/${project_name}.bin"

