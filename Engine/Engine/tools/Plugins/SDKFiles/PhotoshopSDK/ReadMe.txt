Welcome to the Adobe Photoshop CS
Software Development Kit
Macintosh and Windows
Version CS ( 8.0 ) (10/20/2003)

_______________________________________________________________________________
What’s new in CS?

Photoshop now runs on OS X only. This means all of your plug ins must be 
Carbonized. See the section below Quick Start to Carbonizing Plug Ins. 
Photoshop will not load Non-Carbon plug ins.

All shipping plug-ins are using the packaging method. You see one file that 
looks like a plug-in but it is actually a package with the Carbon plug ins. 
See the section below Packaging Plug Ins.

Windows XP changes. Your plug in must tell the OS that it is XP aware.
 1. Download the latest Microsoft Windows Platform SDK from 
    <http://www.microsoft.com/msdownload/platformsdk/sdkupdate>
 2. Launch MSDEV. From the menus: Tools->Options->Directories.
    Add a path to the new includes as the first search path. 
    Example: E:\Program Files\Microsoft SDK\Include
 3. In your plug in project settings add ISOLATION_AWARE_ENABLED 
    C/C++ Tab -> Category = General -> Preprocessor definitions
 4. Rebuild your plug in.
 NOTE: This is not required for ADM, Adobe Dialog Manager, based plug ins.

The Scripting Support plug in developed for Illustrator is now available for 
Photoshop. This plug-in comes installed by default. You can now create 
JavaScript, AppleScript, and Visual Basic scripts.

Photoshop File Formats document is updated to reflect new options in version CS.

New properties available, see PIProperties.h and the Propetizer example: 
propDontShowAgainCount.

New PiPL properties:
	File Format modules can add XMPRead and XMPWrite properties to allow the 
	file browser quick access to the XMP data inside your file.
	
	File Format modules can add FileProperties and LosslessRotate to allow the
	file browser quickly rotate these files without opening the document.
	
	Automation plug-ins can add FileBrowserAware to add a menu item in the file
	browser dialog.
	
	Plug-ins can specify the maximum and minimum document sizes they work with.
	PlugInMaxSize and PlugInMinSize.
	
	Plug-ins can specify NoAboutBox. This will cause Photoshop to not add an 
	entry in the About menu.

_______________________________________________________________________________
Quick Start to Carbonizing Plug Ins

Get the Carbon SDK from the Apple web site. http://www.apple.com. You don’t 
need to install the header files if you are using Metrowerks Pro 8.0.

Create new projects, cloning your existing projects, for Carbon Debug and 
Carbon Release

In your settings for the Carbon versions include PIRezMacCarbon.h as the 
Prefix file for Rez files. Include PIDebugCarbon.h or PICarbon.h for the 
Prefix file for your C/C++ files.

Add the following to your PiPL resource replacing the CodePowerPC{...}. 
If you are using CodeFragmentPPC the new selector would be CodeFragmentCarbonPPC.

CodeCarbonPowerPC { 0, 0, "" },

Remove the Non-Carbonized libraries from your Carbon projects. Include CarbonLib
and any other Carbonized libraries you may need. Compile and Link your projects.

NOTE: 
Photoshop CS will not load Non-Carbonized plug ins.

_______________________________________________________________________________
Packaging Plug Ins

Plug ins that ship with Photoshop CS are now inside packages. Below is the 
directory layout for the Dissolve Release plug in. The CodeWarrior project 
settings are given below.

Dissolve:Contents:MacOSClassic::Dissolve.8bf
Dissolve:Contents:PkgInfo
Dissolve:Dissolve.8bf

The first item is the actual plug ins. The second item is the package info 
created by Metrowerks and the third item is an alias to the classic version of 
the plug in. All of this was done by simply enabling the Mac OS Packager in the 
Metrowerks projects.

Carbon Release project for Dissolve. Mac OS Packager Panel
Turn on Use Mac OS Packager and select the Release:Dissolve: folder to package.
Turn off Create alias to Classic executable.
Turn on Create PkgInfo file and use the same creator/type settings for your plug 
in.

Build both projects and ensure that the folder layout is the same as above.

_______________________________________________________________________________
Debugging Plug Ins on OS X via Terminal

Detailed information for debugging a Photoshop session is available by launching 
the applicatoin via the Terminal program. Use the following commands in your 
Terminal window to launch Photoshop. I am using abosulte paths, you may need to 
modify.

setenv CFMDebug 1
setenv CFMDebugFull 1
cd '/Volumes/Photoshop/Adobe Photoshop CS/Adobe Photoshop CS/Contents/MacOS/'
/System/Library/Frameworks/Carbon.framework/Versions/A/Support/LaunchCFMApp 'Adobe Photoshop CS'

_______________________________________________________________________________
Metrowerks debugging

Open the Runtime Settings panel in the project. 
Set the Host application to be Photoshop. 

NOTE: CW has a bug where [sometimes] you'll have to close and re-open your 
project after changing the host)

Make a shortcut in the Photoshop plug-in's folder to your plug-in.
Set some break points and choose debug from CodeWarrior

_______________________________________________________________________________
To install

Double-click the installer file.
Documentation requires Acrobat
The documentation included on this kit requires, at minimum, Acrobat Reader to 
view it. To download a copy of Acrobat Reader free of charge, go to 
http://www.adobe.com/.

_______________________________________________________________________________
Metrowerks CodeWarrior

All the Macintosh projects in this SDK were tested in the latest CodeWarrior 
environment, CW Pro 8.0, Release Edition, IDE 5.1.1. 

_______________________________________________________________________________
Microsoft Visual Studio

All the Windows projects in this SDK were tested in the latest Visual Studio 
environment, .NET 2003 v7.1. The custom build step for the .r file usually gets
extra quotation's during the conversion from MSDEV 6.0 projects. Use the 
project properties to fix this conversion error.

_______________________________________________________________________________
Memory

Always ask Photoshop for your memory when you dynamically allocate storage space. 
This can be accomplished by including the PIUNew.cpp file in your project work 
space. This file over rides operator new and delete to call Photoshop memory 
routines. The only dependency is that the SSPBasic* is defined globally as 
sSPBasic.

NOTE: 
If you are running on Photoshop version 4.0 or earlier this is not a valid solution.

_______________________________________________________________________________
C++ exceptions

If you turn on C++ exceptions in your plug in project make sure you wrap all 
of your call backs with try/catch blocks. You do not want to throw back into 
Photoshop. This will cause Photoshop to disappear without a warning to the user. 
Make sure your try/catch blocks are around your PluginMain routine and any OS 
proc, say for your Dialog routine.

_______________________________________________________________________________
Support

Support for the Adobe Photoshop Plug-In API is provided to members of the 
Adobe Solutions Network (ASN) Program.

In the U.S. and Canada, and outside Europe,
the Middle East, Africa, and Japan, contact:
Adobe Solutions Network
Adobe Systems Incorporated
P.O. Box 609
Klamath Falls, OR 97601

For program information:
Tel: +1-800-685-3510(US/Canada only) or +1-206-675-6145
Fax: +1-800-955-1610 or 1+541-880-5075

For membership questions:
E-mail: asndeveloper@adobe.com
Web: http://partners.adobe.com

In Europe, the Middle East, and Africa, contact:
Adobe Solutions Network
Edinburgh EH11 4GJ
Scotland, United Kingdom

For program information:
Tel: +44 131 458 6800
Fax: +44 131 458 6801

For membership questions:
E-mail: euroasn@adobe.com
Web: http://partners.adobe.com

In Japan, contact:
Adobe Solutions Network
Adobe Systems Co., Ltd.
P.O. Box 43, Bunkyo Green Court Post Office
2-28-8 Hon-Komagome, Bunkyo-ku
Tokyo 113-0021 Japan

For program information:
Tel: +81-(0)3-5740-2620
Fax: +81-(0)3-5740-2621

For membership questions:
E-mail: asnjp@adobe.co.jp
Web: http://partners.adobe.com/asn/japan/developer/benefits.html

Development and SDK questions, e-mail: gapdevsup@adobe.com


As always, any plug-ins you create with this SDK can be sold or 
distributed royalty free.
_______________________________________________________________________________

Adobe, Adobe Acrobat, Adobe Photoshop, Adobe PageMaker, Adobe PhotoDeluxe, 
Adobe After Effects, Adobe Premiere, and Adobe Illustrator are trademarks of 
Adobe Systems Incorporated and may be registered in certain jurisdictions. 
Metrowerks and CodeWarrior are registered trademarks of Metrowerks Incorporated. 
Microsoft, Windows, Windows 95, Windows 98, Windows NT, Developer Studio, and 
Visual C++ are trademarks or registered trademarks of Microsoft Corporation. 
Apple is a registered trademark of Apple Computer Incorporated. All other brand 
and product names are trademarks or registered trademarks of their respective 
holders.

Copyright 1989-2003 Adobe Systems Incorporated. All Rights Reserved.

