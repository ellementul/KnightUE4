// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerationTileMapComponent.h"

using TArrayInt3D = TArray < TArray< TArray<int> > >;

UGenerationTileMapComponent::UGenerationTileMapComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	return;
}

void UGenerationTileMapComponent::Generation()
{	
	HashTiles = NewObject<UHashedTileSet>();
	GenMap   = NewObject<UGeneretionHashMap>();

	HashTiles->AddPattern(TileMap);
	TArrayInt3D HashedMap = HashTiles->HashPattern(TileMap);

	TArray<TArrayInt3D> HashedPatterns;
	for (auto& Pattern : TilePatterns) {
		if (Pattern != nullptr)
		{
			HashTiles->AddPattern(Pattern);
			HashedPatterns.Add( HashTiles->HashPattern(Pattern) );
		}
	}

	// UE_LOG(LogTemp, Log, TEXT("Tile number: %d"), HashTiles->GetMaxIndex());

	MakeTileMapEditable();

	HashedMap = GenMap->Generation(HashedMap, HashedPatterns);

	for (int l = 0; l < HashedMap.Num(); l++) {
		auto Layer = HashedMap[l];

		for (int x = 0; x < Layer.Num(); x++) {
			auto Colmn = Layer[x];

			for (int y = 0; y < Colmn.Num(); y++) {

				int TileIndex = Layer[x][y];
				FPaperTileInfo Tile = ErrorTile;

				if (TileIndex > -1)
					 Tile = HashTiles->GetTile(TileIndex);
				else
					UE_LOG(LogTemp, Error, TEXT("x: %d, y: %d, cell: %d"), x, y, Layer[x][y]);

				SetTile(x, y, l, Tile);
			}
		}
	}

	RebuildCollision();
}