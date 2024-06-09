#include "AssetStore.h"
#include <SDL_Image.h>
#include "../Logger/Logger.h"

AssetStore::AssetStore() {}

AssetStore::~AssetStore() { ClearAssets(); }

void AssetStore::ClearAssets() {
	// Clear textures
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();

	// Clear fonts
	for (auto font : fonts) {
		TTF_CloseFont(font.second);
	}
	fonts.clear();
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) const {
	if (textures.at(assetId) == NULL) Logger::Warn("Couldn't find texture asset: " + assetId);
	return textures.at(assetId);
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	// Get texture & Add to texture map
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
	textures.emplace(assetId, texture);
}

TTF_Font* AssetStore::GetFont(const std::string& assetId) const {
	if (fonts.at(assetId) == NULL) Logger::Warn("Couldn't find font asset: " + assetId);
	return fonts.at(assetId);
}

void AssetStore::AddFont(const std::string assetId, const std::string& filePath, int fontSize) {
	fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}