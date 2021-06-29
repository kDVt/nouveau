# nouveau 

Revision 2  
Draft 1  
June 29 2021

Proposition for a complete restructure of the entire jailbreak, jailbreak subsystem, and components within.

---

The end goal of this is a jailbreak targeted at users. This proposition is not empty words, it is a roadmap for multiple projects in active development.

It properly gives non-developers the option to use a stable, less dangerous, more secure "jailbreak".

---

## Issues with the current structure of jailbreak

The following issues detailed were the motivations that drove this proposal's evolution.

### Security

As [Recent Discoveries](https://www.theiphonewiki.com/wiki/Malware_for_iOS#mainrepo_RAT) have demonstrated, Jailbroken iPhones have time and time again been the target of malware. This malware leverages the unfettered access to a unix subsystem and preexisting injection framework and runs wild.

There is a reason iOS malware in the wild so often uses jailbreak components to operate; Jailbreaks in the past were written to allow malware to run wild, and dump the full responsibility on the user to make sure they only install "good malware".

### Safety

It is far too easy to screw up an iOS install, losing data and possibly even being forced to update your iOS version in the process.

### Forensics/Detection

Due to being built with complete disregard for the existing filesystem's integrity, the current subsystem used in all modern jailbreaks is messy. 

This results in detection of the jailbroken state being made easy, even from within the App sandbox, and even after jailbreaks have been removed.

---

Nouveau aims to uproot the entirety of the current system with the goal of fixing these issues.

The rest of the document details the specifications for changes/limitations to the Jailbreak Subsystem.

Implementation details should be outlined in other documents.

# Jailbreak Subsystem

This proposition targets "Semi-Untethered" jailbreaks, which make use of kernel exploits from within an App Sandbox.

All other "System"s described in this document fall under this.

"Tweaks" will be referred to as "Libraries" in this document. You are free to call them whatever you want. 

Core Philosophies:
* Modifying system behavior should *only ever* be done at runtime, through injection of libraries.
* Users and packages should not have full direct access to root (user or filesystem)
* The Jailbreak will not implement a Unix Subsystem or "bootstrap" the device.
* The Jailbreak Subsystem should handle all "root-required tasks"

---

## Package Limitations

These limitations are imposed on packages available for download from the Package Manager.

### Library limitations

Libraries should have filesystem access sandboxed. This allows many libraries to maintain their current structure and work without modification.

Libraries cannot execute arbitrary bash scripts upon installation.

Libraries cannot modify the behavior of the jailbreak's subsystem itself.

### Application Limitations

Applications will be loaded through hooks in the system as if they were installed via the App Store. 

Applications will be sandboxed in the same way App Store apps are, with the Containers created and managed by the jailbreak.

Applications may not run as root.
* This may be circumvented by manually sideloading the app, and the jailbreak will require on-screen confirmation before elevating the process.

## Package Manager 

The package manager for this jailbreak should aim to mirror the App Store's efforts to display privacy related info about packages.

For Libraries:
The package manager should display and explain the processes libraries will inject into.

Disregard any notion related to "debian archives", "postinst scripts", and likewise. These do not exist here, we are not installing debian linux.

### Package Formats

"Packages" should consist of two seperate types of package:
* Libraries (tweaks), with instructions on where to inject them
* Apps, packaged as .ipas, to be made available to the user

Bootstraps, CLI tools, and everything else will not exist for Nouveau-based jailbreaks.

## Development

The jailbreak should aim to make package development for it painless.

It should not require drastic modifications to theos/dragon

Developer tooling should be disabled by default and enabled via the Jailbreak App.

### Developer tooling
Support for the following developer tools should ship with the jailbreak:
* Flipboard Explorer
* RuntimeExplorer

### Package compilation

Support for the new package formats will be PRed to theos and included in dragon from day one.

### Package Installation

The jailbreak should emulate an openSSH server on port 22 for receiving packages from exiting build tools.

The jailbreak should ensure usbmuxd-based installation methods function as well
