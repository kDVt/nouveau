# Proposal for a complete overhaul of iOS Extension Packaging

While every aspect of this document has been expanded upon, fleshed out, and organized elsewhere in this repo, this document aims to organize and summarize the proposal, explain why it's needed, and how it can be done.

--- 

Getting right into it, Debian's packaging tools are out of place on iOS, and provide no real advantage other than "working". Dragging an entirely different operating system's tools, and then modifying them partially to work on Darwin, adds another layer of confusion. There is genuinely no problem apt on iOS solves, and it's usage seems akin to porting Homebrew to Windows. Why?

This is a proposal with a full proof-of-concept, for an entirely new standard, based on how Darwin, the system we're operating on, handles packages. iOS ships with a built in package manager distributing IPA files, and using a similar format for Jailbreak packages makes perfect sense. I've detailed just a few of the glaring issues with APT. The presented concept below then details what changes would be made at every level of the process (not many!). I have also provided a proof-of-concept, including a full explanation of each aspect, implementation of the new package format, and clear details on implementation at every level.

This project assumes that it will never see full adoption within the community. Every proposal made considers and allows for:
* Deb packages to still be installable
* Repos to still distribute deb packages
* Developers to create deb packages

There is no intention to hard-deprecate APT on iOS. The hope, though, is that the benefits are obvious to everyone, and a gradual adoption can be made. APT needs to go, but it can always stop by for coffee :)

## Why?: The problems with APT/dpkg based packaging

The decision to base a new format on iOS standards has exposed plenty of issues with the current methods, and provided solutions to problems that weren't even in consideration when creating this.

These are just a few issues. A full list could blot out the sun.

* Xcode cannot natively handle creating a .deb package. iOS isn't debian, why would it?
* Certain package managers cannot (easily) coexist on devices due to conflicts/disagreements over APT implementations.
* Extracting and working with .deb files is a huge, unneccessary pain. `ar` compression of files (outside of linking) is not common, and overcomplicates extraction of anything, especially without a `dpkg` binary.
* Giving any installed closed source package immediate, simple access to the entire filesystem, with the ability to run any scripts as root upon install is *incredibly irresponsible*
    * While this proposal doesn't fully fix this issue, it creates (and details) a path for holding tweaks to the same privacy standards as apps. Not doing this is naive and lazy, there's no good excuse.
* The dpkg format adds extra, unneeded dependencies for packaging. On operating systems without dpkg avaliable (Windows, till recently, for example), packaging a theme required booting another OS.
    * Due to dpkg issues on Centos 8, I was forced to painfully reimplement some major dpkg functionality in python solely for the sake of generating a `Packages.bz2` file.
* Using an unconventional, non-Apple-esque packaging format leads to developers having issues or neglecting to understand the package format. 
* An issue with an apt installation can end up forcing a rootfs restore. On checkra1n, it actually breaks the button to trigger a rootfs restore. A user or developer could easily manage to cause this by adding a seperate bootstrap repo with a newer APT build.
* Adding a repository for access via apt-get (the standard, apt way) is not equivelant to adding the repo within the package manager, nor is there an intuitive way to do so from a shell. 

Before anyone bothers saying it, blaming any of this on inexperienced developers not learning every detail of an API or tool is beyond naive, and this continued mindset has caused a myriad of problems as time has gone on. It's the responsibility of those creating the tools to build tools that people can sanely use.

## Proposal for a new package format, designed from the ground up, based on the existing native implementation

Abandon the concept of apt, dpkg, debians, full filesystem layouts, and the rest. 

**The Package Format**:
* Packages will be distributed as zip compressed files, mirroring the process for turning an iOS `.app` bundle into an `.ipa`. 
    * The name `Package` will be used for the root folder, instead of `Payload`
    * An `Info.plist` at the root of the `Package` folder will replace the control file format
    * The `Package` folder can contain multiple bundles. The package manager should then process these bundles accordingly
        * The `Info.plist` within each bundle should indicate to the package manager what should be done 
        * The indication of a bundle's purpose should not be redundant. A bundle with an InjectLibrary key is treated as a tweak, for example.

**Changes to Repositories**:
* All changes should be entirely optional, and where Repositories don't support it, the Package Manager should handle it accordingly
* A Support Script is provided allowing converting between the new and old format (both ways)
    * This should also handle generating legacy style info from new packages
    * More details on this are provided below
* A file identical in format to the current `Packages` standard named `Repo` (subject to change) will be looked for by the Package Manager, which should point to new format packages instead of debs
* Repositories supporting the new format should use the script to allow full support for legacy devices and apt installations.

**Changes to the Package Manager**:
* Support within a package manager is vital
* Removal of the dependency on APT/DPKG as a backend
    * Replacement of this with logic to convert old packages to the new format

**Changes to how packages are installed**:
* Packages should be installed in a directory detirmined by the package manager
    * The directory location should not affect the operation of the tweak whatsoever
    * A proposal is needed for Packages that need more extensive access to the filesystem. Cr4shed comes to mind.
* Theme bundles when detected are auto-linked to the correct location required by theme engines

**Changes to tweak injectors (SubstrateLoader, TweakInject, etc)**:
* No immediate changes are required. A support library that acts as a replacement Tweak Injector will be injected via Substrate or equivelant.
    * The support library will then process the configured directory and inject tweaks from there as needed. 
* Ideally, if adoption is successful, changes can be made to these injectors that cut out the need for a middleman.

## What this will change

### How this affects your average tweak developer

tldr: It should not whatsoever.

Theos, DragonBuild, or XCode (which is **fully** compatible with the new format natively) should handle almost the entirety of using the new system, via a toggle, envar, or similar switch.

99.9% of developers should never have to worry about this, and from experience, many aren't very familiar with what goes on during the build and package process anyways. Leading off of that, this packaging method is simpler and more inline with other Apple development, meaning developers trying to debug issues can easily and quickly understand what's going on. 

### How this affects theme creators

As theme creators are somewhat of a neglected group of people when it comes to packaging, this solution makes perfect sense for them. An extremely basic template is provided, that can be modified, and packaged by zipping the directory and sending it off. 

### How this affects server/repo maintainers

The current format of repos has no glaring problems, and there's no reason to change anything about it here. 

Repos wishing to support the new format can use the provided script which can:
* Handle processing solely the new package format, for addition to an existing system
* Create .deb packages from the new format, for full-range compatibility
* Generate the standard `Packages` file and the new `Repo` file, to allow supporting both natively
* Generate the new package format from .deb files, if that becomes needed at any point.

There is no obligation or expectation of repository owners to support this, and compliant package managers should be able to handle a fully legacy repository indefinitely.

The new package format, however, does provide several benifits to repository owners if they choose to support it:
* Creating scripts to process, read, modify, or otherwise work with packages no longer requires complex extraction via `ar` and other tools. 
* Although the new package format abandons control files, the root Info.plist contains all needed information, and the provided support script can process this into control file format.
* The "sandboxed" design allows a bit more peace of mind regarding packages with malicious intentions. Malware can't be stopped, but it can be mitigated.

## Details/POC

https://github.com/kDVt/nouveau/blob/master/package-structure.md#detailspoc