# Concepts for implementation at every level

Main goals for the proposed implementations:
* All changes at every stage should be entirely optional.
* The projects should always be built with the assumption that there will be people who do not adopt it.
    * For example, a repo should be able to handle a legacy format package, and should be able to provide one in legacy format to older Package Managers
* The projects should always aim to support non-adoption of the standard at any level.

---

There are 5 major steps in the chain from Developer to User which will require changes to support the new format.

## Tweak Development

The new format, for the sake of furthering the goals of privacy and better development, also makes some changes to how tweaks should assume they're installed.

**Adopting Developers:**
* Your package should not rely on it's filesystem location whatsoever.
* Resource bundles should be used by passing the name to the documented api exposed by nsupport-inject or the injection library.
* Install/Removal scripts don't exist in the new format, and packages must be entirely self contained.
* For tools needing root filesystem access, or the ability to run install scripts as root, user permission will be requested. Avoid this.

**Handling Non-adopting Developers:**
* When ipkg or other tools handle a legacy .deb package, it should be converted and installed in the new format.
* When a .deb has packager scripts (preinst, postrm, etc), Package Managers should prompt the user before running, and allow previewing contents of the script.
* When a filesystem location is used by a .deb package for some files, and a suitable conversion cannot be found, installation should prompt the user before installing it there.
    * This is done by a resource bundle which tells support tools to symlink a specific directory to it.

## Tweak Builders and Packagers

DragonBuild support is nearly done already.

Instructions and templates for usage in XCode should be provided.

### Theos

The changes required to theos are minimal. A Variable or flag should be added that allows building the old, new, or both formats of the package. 

Substituting dpkg for ipkg is benificial for theos in other areas, as well, and should cover theos' implementation needs.

A fork will be maintained that accomplishes this, and hopefully a PR will be made after discussion with theos developers. 

## Repositories

Repositories, especially larger ones, will be the largest roadblock. They are businesses, typically. The benifits to a repo, and to the community, moreover, should be made clear.

**Adopting Repos:**
Nothing whatsoever about the repo format will change, except for the filename regarding `Packages`. The new one will point to new packages, and `Packages` will provide legacy interoperability. 

ipkg, along with javascript, python, and possibly php snippets to handle working with package formats should be provided.

**Non-adopting Repos:**
It's critical that adoption by a repo isn't expected at any level. 

## Package Managers

This will be by far the most time-consuming part. While I'm willing to write the entirety of code required, it regardless needs discussed with each package manager's development team. 

**Adopting Package Managers:**
The benifits of the format are clear. Providing code capable of supporting both apt and the new format is critical, and should be drop-in. 

**Handling Non-adopting Package Managers:**
Support packages should be created for non-adopting package managers that shim the apt wrapper, allowing the interoperability adopting package managers have.

## Tweak Injection 

Due to how Tweak Injection works, if none of the existing projects adopt it, everything will still work perfectly. 

**Adopting Tweak Injectors:**
individually detailed here: https://github.com/kDVt/nouveau/blob/master/contributions.md#tweak-injection

**Handling Non-adopting Tweak Injectors:**
A support library (nsupport-inject) is provided, installed in the legacy location. This handles the new format.
