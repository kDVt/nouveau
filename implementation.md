# Steps for implementation at every level

All changes at every stage should be entirely optional.

The projects should always be built with the assumption that there will be people who do not adopt it.

The projects should always aim to support non-adoption of the standard at any level.

## Tweak Development

Optional Adoption:
* An API will be exposed via the injection library allowing usage. 

## Theme Packaging

A packaging utility, written in python.

## Tweak Builders and Packagers

DragonBuild support is nearly done already.

Instructions and templates for usage in XCode should be provided.

### Theos

The changes required to theos are minimal. A Variable or flag should be added that allows building the old, new, or both formats of the package. 

A replacement python script (this will cover **every** operating system) for dpkg that takes the same arguments as dpkg should be created.

A fork will be maintained that accomplishes this, and hopefully a PR will be made after discussion with theos developers. 

## Repositories

