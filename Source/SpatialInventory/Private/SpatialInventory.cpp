#include "SpatialInventory.h"

DEFINE_LOG_CATEGORY(SpatialInventory);

#define LOCTEXT_NAMESPACE "FSpatialInventory"

void FSpatialInventory::StartupModule()
{
	UE_LOG(SpatialInventory, Warning, TEXT("SpatialInventory module has been loaded"));
}

void FSpatialInventory::ShutdownModule()
{
	UE_LOG(SpatialInventory, Warning, TEXT("SpatialInventory module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSpatialInventory, SpatialInventory)