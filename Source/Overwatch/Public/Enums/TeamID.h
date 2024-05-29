#pragma once

UENUM(BlueprintType)
enum class ETeamID : uint8
{
	ETI_None UMETA(Hidden, DisplayName = "None"),
	ETI_Team1 UMETA(DisplayName = "Team1"),
	ETI_Team2 UMETA(DisplayName = "Team2"),
	ETI_Max UMETA(Hidden, DisplayName = "Max")
};
