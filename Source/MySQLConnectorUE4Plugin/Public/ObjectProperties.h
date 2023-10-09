// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ObjectProperties.generated.h"


UENUM()
enum ESpawnMask : uint8
{
	NotSpawned				= 0			UMETA(DisplayName="Not spawned"),
	NormalDifficulty		= 0x1		UMETA(DisplayName="Spawned only in normal difficulty"),
	Difficulty1				= 0x2		UMETA(DisplayName="Spawned only in difficulty = 1"),
	Difficulty2				= 0x4		UMETA(DisplayName="Spawned only in difficulty = 2"),
	Difficulty3             = 0x8		UMETA(DisplayName="Spawned only in difficulty = 3"),
	SpawnedAlways			= 0x15		UMETA(DisplayName="Spawned in all versions of maps")
};

//creature_movement and creature_movement_template need to have MovementType set to 2,3,4 for a guid.

//spawn_group / waypoint_path spawns should use 0 for all creatures of that spawn_group.
UENUM()
enum EMoveType : uint8
{
	//No Movement – IdleMovementGenerator
	IDLE_MOTION_TYPE = 0			UMETA(DisplayName="No Movement"),
	
	//Random Movement with spawndist – RandomMovementGenerator
	RANDOM_MOTION_TYPE = 1			UMETA(DisplayName="Random Movement with spawndist"),
	
	//123.123 – WaypointMovementGenerator
	WAYPOINT_MOTION_TYPE = 2  		UMETA(DisplayName="Waypoint Movement"),
	
	//123.123 – Spline Movement, All Waypoints are send in creature update packet (e.g flight paths) – PathMovementGenerator
	PATH_MOTION_TYPE = 3 			UMETA(DisplayName="Spline Movement"),
	
	//123.21.23.21.23.21 – WaypointMovementGenerator
	LINEAR_WP_MOTION_TYPE = 4 		UMETA(DisplayName="Linear Waypoint Movement"),	
};

UCLASS()
class UGameobjectInfo : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "GUID"))
	uint64 GUID;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Entry from gameobject_template"))
	uint32 ID;
	
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "ID of current Map"))
	uint32 MapID;
	
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Phasemask set by Auras"))
	uint32 PhaseMask;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "SpawnMask"))
	TEnumAsByte<ESpawnMask> SpawnMask;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Position"))
	FVector3f Position;

	/*The orientation. (North = 0, South = 3.14159)*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Orientation"))
	float Orientation;

	/*GameObject Rotation: X Y Z W*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation"))
	FQuat4f Rotation;
	

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation"))
	int32 SpawnTimeSecsMin;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation"))
	int32 SpawnTimeSecsMax;	
};

UCLASS()
class UCreatureInfo : public UObject
{
	GENERATED_BODY()
	
public:
		
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "GUID"))
	uint64 GUID;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Entry from gameobject_template"))
	uint32 ID;
	
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "ID of current Map"))
	uint32 MapID;
	
	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Phasemask set by Auras"))
	uint32 PhaseMask;	

	UPROPERTY(EditAnywhere, meta = (DisplayName = "SpawnMask"))
	TEnumAsByte<ESpawnMask> SpawnMask;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Position"))
	FVector3f Position;
	
	/*The orientation. (North = 0, South = 3.14159)*/
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Orientation"))
	float Orientation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation"))
	int32 SpawnTimeSecsMin;
	
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation"))
	int32 SpawnTimeSecsMax;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Spawn distance"))
	float SpawnDist;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Movement Type"))
	TEnumAsByte<EMoveType> MovementType;
};
