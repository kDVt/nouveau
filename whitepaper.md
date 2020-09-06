# Proposal for a complete overhaul of iOS Extension Packaging

This paper attempts to summate the motivations, benifits, and central ideas behind this proposal.

--- 

This is a proposal with a full proof-of-concept, for an entirely new standard, based on how Darwin, the system we're operating on, handles packages. iOS ships with a built in package manager distributing IPA files, and using a similar format for Jailbreak packages is common sense.

Previous proposals for packaging standards have lacked a purpose, direction, or have not solved any of the issues present with current implementations. Furthermore, a failure to support the legacy format while still providing the benifits makes adoption impossible. 

Thus:

**Core Concepts**
* Adopting the new format should not interfere with non-adopting developers
* Non-adopting projects should not interfere with the benifits of adopting packages
* The amount of change required for adoption at any level should be as minimal as possible.

Nobody at any level should be, or feel, forced to change. The hope is that the benefits are obvious to everyone, and those wanting to take advantage of them can adopt the new format.

## APT/dpkg problems solved by this proposal

The decision to base a new format on iOS standards has exposed plenty of issues with the current methods, and provided solutions to problems that weren't even in consideration when creating this.

These are just a few issues. A full list could blot out the sun.

* Xcode cannot natively handle creating a .deb package. iOS isn't debian, why would it?
* Certain package managers cannot (easily) coexist on devices due to conflicts/disagreements over APT implementations.
* Extracting and working with .deb files is a huge, unneccessary pain. `ar` compression of files (outside of linking) is not common, and overcomplicates extraction of anything, especially without a `dpkg` binary.
* Giving any installed closed source package immediate, simple access to the entire filesystem, with the ability to run any scripts as root upon install is *incredibly irresponsible*
    * While this proposal doesn't fully fix this issue, it creates (and details) a path for holding tweaks to the same privacy standards as apps. Not doing this is naive and lazy, there's no good excuse.
* The dpkg format adds extra, unneeded dependencies for packaging. On operating systems without dpkg avaliable (Windows, till recently, for example), packaging a theme required booting another OS.
    * Using dpkg-deb on Centos 8 required re-implementing dpkg-deb in python for the sake of creating a Packages file for a repo.
* Using an unconventional, non-Apple-esque packaging format leads to developers having issues or neglecting to understand the package format. 
    * Expecting theme developers to figure it out is an even further stretch.
* An issue with an apt installation can end up forcing a rootfs restore. On checkra1n, it actually breaks the button to trigger a rootfs restore. A user or developer could easily manage to cause this by adding a seperate bootstrap repo with a newer APT build.
* Adding a repository for access via apt-get (the standard, apt way) is not equivelant to adding the repo within the package manager, nor is there an intuitive way to do so from a shell. 
