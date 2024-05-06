#include "AssetStore.h"
#include <SDL_Image.h>

AssetStore::AssetStore() {}

AssetStore::~AssetStore() { ClearAssets(); }

void AssetStore::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) const {
	return textures.at(assetId);
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
	// Get texture & Add to texture map
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
	textures.emplace(assetId, texture);
}

