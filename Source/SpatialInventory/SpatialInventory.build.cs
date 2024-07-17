using System.IO;
using UnrealBuildTool;
 
public class SpatialInventory : ModuleRules
{
	public SpatialInventory(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "SlateCore", "CoreUObject", "Slate", "InputCore", "InteractionSystem", "StructUtils"});
	}
}