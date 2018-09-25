#include "AudioM.h"

bool AudioM::openBgm = true;
bool AudioM::openSound = true;
string AudioM::curMusic = "";
// const string AudioM::mySuffix = ".mp3";
const string AudioM::mySuffix = ".ogg";

void AudioM::preloadAudio()
{
	SimpleAudioEngine * audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("audio/music/BaoDao.ogg");
	audio->preloadBackgroundMusic("audio/music/Canon.ogg");
	audio->preloadBackgroundMusic("audio/music/XingGuang.ogg");
    audio->preloadBackgroundMusic("audio/music/Win.ogg");
}

void AudioM::playBGM(string s, bool loop)
{
	if (curMusic != s && openBgm) {
		curMusic = s;
		SimpleAudioEngine * audio = SimpleAudioEngine::getInstance();
		audio->playBackgroundMusic((s + mySuffix).c_str(), loop);
	}
}

void AudioM::playSound(string s, bool loop)
{
	if (openSound) {
		SimpleAudioEngine * audio = SimpleAudioEngine::getInstance();
		audio->playEffect((s + mySuffix).c_str(), loop);
	}
}

void AudioM::turnBgm(bool on)
{
	if (openBgm != on) {
		openBgm = on;
		if (on) {
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		} else {
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
	}
}

void AudioM::turnSound(bool on)
{
	openSound = on;
}

bool AudioM::hasBgm()
{
	return openBgm;
}

bool AudioM::hasSound()
{
	return openSound;
}

void AudioM::resumeBGM()
{
	if (openBgm) {
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	if (openSound) {
		SimpleAudioEngine::getInstance()->resumeAllEffects();
	}
}
