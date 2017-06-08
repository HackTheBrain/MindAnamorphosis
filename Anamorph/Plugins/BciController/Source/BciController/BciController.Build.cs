// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    using System.IO; // ToDo: Replace with standard mechenism

    public class BciController : ModuleRules
    {
        public BciController(TargetInfo Target)
        {
            PCHUsage = PCHUsageMode.NoSharedPCHs;

            // ... add public include paths required here ...
            PublicIncludePaths.AddRange(new string[] {
            "BciController/Public",
            "BciController/Classes",
        });

            // ... add other private include paths required here ...
            PrivateIncludePaths.AddRange(new string[] {
            "BciController/Private",
            "C:\\workspace\\BCI\\vrpn"
        });

            // ... add other public dependencies that you statically link with here ...
            PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",      // Provides Actors and Structs
            "Engine",           // Used by Actor
            "Slate",            // Used by InputDevice to fire bespoke FKey events
            "InputCore",        // Provides LOCTEXT and other Input features
            "InputDevice",      // Provides IInputInterface
        });

            // ... add private dependencies that you statically link with here ...
            PrivateDependencyModuleNames.AddRange(new string[] {
        });

            PublicAdditionalLibraries.Add(Path.Combine("C:\\workspace\\BCI\\vrpn\\PC_64\\Release", "vrpn.lib"));

            // ... add any modules that your module loads dynamically here ...
            DynamicallyLoadedModuleNames.AddRange(new string[] {
        });

            // !!!!!!!!!! UNCOMMENT THIS IF YOU WANT TO CALL A LIBRARY !!!!!!!!!!
            //LoadYourThirdPartyLibraries(Target);
        }

        public bool LoadYourThirdPartyLibraries(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            // This will give oyu a relitive path to the module ../BciController/
            //string ModulePath = Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));

            RulesAssembly r;
            FileReference CheckProjectFile;
            UProjectInfo.TryGetProjectForTarget("Anamorphosis", out CheckProjectFile);

            r = RulesCompiler.CreateProjectRulesAssembly(CheckProjectFile);
            FileReference f = r.GetModuleFileName(this.GetType().Name);
            //File.WriteAllText("c:/temp/qqq2.txt", f.CanonicalName );

            string ModulePath = Path.GetDirectoryName(f.CanonicalName);


            // This will give you a relative path to ../BciController/ThirdParty/"LibraryDirName"/
            string MyLibraryPath = Path.Combine(ModulePath, "ThirdParty", "LibraryDirName");

            // Use this to keep Win32/Win64/e.t.c. library files in seprate subdirectories
            string ArchitecturePath = "";

            // When you are building for Win64
            if (Target.Platform == UnrealTargetPlatform.Win64 &&
                WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2013)
            {
                // We will look for the library in ../BciController/ThirdParty/MyLibrary/Win64/VS20##/
                ArchitecturePath = Path.Combine("Win64", "VS" + WindowsPlatform.GetVisualStudioCompilerVersionName());

                isLibrarySupported = true;
            }
            // When you are building for Win32
            else if (Target.Platform == UnrealTargetPlatform.Win32 &&
                WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2013)
            {
                // We will look for the library in ../BciController/ThirdParty/MyLibrary/Win32/VS20##/
                ArchitecturePath = Path.Combine("Win32", "VS" + WindowsPlatform.GetVisualStudioCompilerVersionName());

                isLibrarySupported = true;
            }
            // Add mac/linux/mobile support in much the same way

            // If the current build architecture was supported by the above if statements
            if (isLibrarySupported)
            {
                // Add the architecture spacific path to the library files
                PublicAdditionalLibraries.Add(Path.Combine(MyLibraryPath, "lib", ArchitecturePath, "MyLibrary.lib"));
                // Add a more generic path to the include header files
                PublicIncludePaths.Add(Path.Combine(MyLibraryPath, "include"));
            }

            // Defination lets us know whether we successfully found our library!
            Definitions.Add(string.Format("WITH_MY_LIBRARY_PATH_USE={0}", isLibrarySupported ? 1 : 0));

            return isLibrarySupported;
        }
    }
    /*
        public class SlateRemote : ModuleRules
        {
            public SlateRemote(TargetInfo Target)
            {
                PublicDependencyModuleNames.AddRange(
                    new string[] {
                        "Networking",
                    }
                ); 

                PrivateDependencyModuleNames.AddRange(
                    new string[] {
                        "Core",
                        "CoreUObject",
                        "InputCore",
                        "Slate",
                        "SlateCore",
                        "Sockets",
                    }
                );

                PrivateIncludePathModuleNames.AddRange(
                    new string[] {
                        "Messaging",
                        "Settings",
                    }
                );

                PrivateIncludePaths.AddRange(
                    new string[] {
                        "SlateRemote/Private",
                        "SlateRemote/Private/Server",
                        "SlateRemote/Private/Shared",
                    }
                );
            }
        }*/
}
