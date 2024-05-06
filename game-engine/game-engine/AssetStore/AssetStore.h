#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <SDL.h>

class AssetStore {
	private:
		std::map<std::string, SDL_Texture*> textures;
		// Can add on fonts, sounds, etc. later here

	public:
		AssetStore();
		~AssetStore();

		void ClearAssets();
		SDL_Texture* GetTexture(const std::string& assetId) const;
		void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
};

#endif