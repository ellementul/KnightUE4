// Fill out your copyright notice in the Description page of Project Settings.


#include "HashedTileSet.h"

UHashedTileSet::UHashedTileSet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
	TileSet.Add( FPaperTileInfo() );
}

bool UHashedTileSet::AddPattern(UPaperTileMap* TileMap) {
	bool isAddedTile = 0;

	UE_LOG(LogTemp, Log, TEXT("Size pattern width: %d, height: %d"), TileMap->MapWidth, TileMap->MapHeight);

	for (auto& Layer : TileMap->TileLayers)
	{
		for (int x = 0; x < TileMap->MapWidth; x++)
		{
			for (int y = 0; y < TileMap->MapHeight; y++)
			{
				auto tile = Layer->GetCell(x, y);
				int AddedIndex = AddTile(tile);
				isAddedTile = isAddedTile || AddedIndex;
			}
		}
	}

	return isAddedTile;
}

TArray < TArray< TArray<int> > > UHashedTileSet::HashPattern(UPaperTileMap* TileMap) {
	
	TArray< TArray< TArray<int> > > HashedMap;

	UE_LOG(LogTemp, Log, TEXT("Size pattern width: %d, height: %d"), TileMap->MapWidth, TileMap->MapHeight);

	for (auto& Layer : TileMap->TileLayers)
	{
		TArray< TArray<int> > HashedLayer;

		for (int x = 0; x < TileMap->MapWidth; x++)
		{
			TArray<int> HashedCol;

			for (int y = 0; y < TileMap->MapHeight; y++)
			{
				int TileIndex = GetIndex(Layer->GetCell(x, y));
				HashedCol.Add(TileIndex);
			}

			HashedLayer.Add(HashedCol);
		}

		HashedMap.Add(HashedLayer);
	}

	return HashedMap;
}

int UHashedTileSet::AddTile(FPaperTileInfo tile)
{
	int index;
	if (!TileSet.Find(tile, index)) {
		index = TileSet.Add(tile);
	}

	return index;
}

int UHashedTileSet::GetIndex(FPaperTileInfo Tile)
{
	int Index;
	if (TileSet.Find(Tile, Index)) {
		return Index;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("HashedTileSet: Unknowned tile!"));
		return 0;
	}

}

int UHashedTileSet::GetMaxIndex()
{
	return TileSet.Num();
}

struct FPaperTileInfo UHashedTileSet::GetTile(int Index)
{
	if ( TileSet.IsValidIndex(Index) ) {
		return TileSet[Index];
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("HashedTileSet: Invalid index: %d"), Index);
		return FPaperTileInfo();
	}	
}
