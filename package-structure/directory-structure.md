## Details/POC

The concept for packages mirrors, logically, iOS's structure for .IPA files.

Packages will be distributed in zipped format, like ipas. A custom filename extension should be used for clarity's sake.

The proposed structure of the zip contents is as follows:

```
Package
├── Info.plist
├── Tweak.ext
│   ├── Tweak.dylib
│   └── Info.plist
│
├── TweakResources.bundle
│   ├── Info.plist
│   ├── SomeRandomDB.sqlite
│   └── Example.png
│
└── TweakPrefs.bundle
    ├── TweakPrefs
    ├── Entry.plist
    ├── Info.plist
    ├── Prefs.plist
    └── base.lproj
        └── Prefs.strings
```

Substitute "Tweak" with the name of the package in question.

As the system looks through bundles contained in the Package folder, it will identify and handle them according to info within their Info.plist

### The Tweak: Tweak.ext

.ext is a cosmetic folder name extension indicating the package contained is a runtime extension ('tweak').

The `Tweak.ext` folder should act as a container for the contents of the tweak. The tweak should not assume it's location on disk, nor should it rely on it for anything.

Resources can be stored in the `Tweak.ext` folder for simpler implementations, but Resource Bundles are encouraged.

Presence of the `InjectLibrary` key within `Info.plist` identifies this bundle as a "tweak" to be injected.

Info.plist contents in Tweak.ext:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>InjectLibrary</key>
    <string>Tweak.dylib</string>
    <key>Filter</key>
    <dict>
        <key>Bundles</key>
        <array>
            <string>com.apple.springboard</string>
        </array>
    </dict>
</dict>
</plist>
```

As can be seen here, the contents of the `Filter` dictionary *directly* mirror the bundle filter format. Info.plist doubles as a valid bundle filter that can be easily passed to existing hooking libraries. In the future, this also allows copying the bundle filter info directly into an Info.plist when supporting legacy packages.


### Resources: TweakResources.bundle

This is a common resource bundle for the tweak, which it should have read/write access to. A small method in the injection library should instead be used to dynamically get the directory path at runtime. 

Info.plist contents in TweakResources.bundle:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>FileAPIName</key>
    <string>TweakResources.bundle</string>
</dict>
</plist>
```

FileAPIName tells the injection library to expose this bundle's path to the tweak whenever it asks for it with the name provided.

### Prefs: TweakPrefs.bundle

The current way of packaging prefs involves more work for no reason or benifit. Here, the PreferenceLoader entry.plist is bundled with everything else, and a custom key indicates the relevant .plist.

A better handling of preference loading is being worked on, but is not part of this project, and as such this project will aim to support the current format.

Symlinks will be used and handled by the support library for mitigating current innefficiencies in PreferenceLoader usage.

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>BundleLibrary</key>
    <string>TweakPrefs</string>
	<key>PreferenceBundleInfo</key>
	<dict>
		<key>Entry</key>
		<string>Entry.plist</string>
	</dict>
</dict>
</plist>
```
